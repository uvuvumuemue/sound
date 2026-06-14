#include "middlepanel.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QStackedWidget>
#include <QListWidget>
#include <QPushButton>
#include <QRegularExpression>
#include "servermanager.h"
#include "playlistmanager.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QRegularExpression>
#include <QMenu>
#include <QAction>
MiddlePanel::MiddlePanel(QWidget *parent) : QWidget(parent) {
    m_networkManager = new QNetworkAccessManager(this);
    setupUi();
}

void MiddlePanel::setupUi() {
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet("background-color: #222020;");
    QVBoxLayout *layout = new QVBoxLayout(this);
    m_songSearchEdit = new QLineEdit(this);
    m_songSearchEdit->setPlaceholderText("Wpisz nazwę utworu i wciśnij Enter...");
    m_songSearchEdit->setStyleSheet("color: white; background-color: #333030; padding: 10px; border: 1px solid blue; font-size: 14px;");
    layout->addWidget(m_songSearchEdit);

    m_songsStack = new QStackedWidget(this);
    m_playlistSongsList = new QListWidget(m_songsStack);
    m_playlistSongsList->setStyleSheet("color: white; background-color: #373434; font-size: 14px;");

    m_searchedSongsList = new QListWidget(m_songsStack);
    m_searchedSongsList->setStyleSheet("color: white; background-color: #484545; font-size: 14px;");

    m_songsStack->addWidget(m_playlistSongsList);
    m_songsStack->addWidget(m_searchedSongsList);
    layout->addWidget(m_songsStack);
    connect(m_songSearchEdit, &QLineEdit::returnPressed, [this]() {
        m_songsStack->setCurrentIndex(1);
        performSearch(m_songSearchEdit->text().trimmed());
    });
    connect(m_searchedSongsList, &QListWidget::itemDoubleClicked, [this](QListWidgetItem *item) {
        QString text = item->text();

        if (!text.startsWith("[") || text.contains("Nie znaleziono") || text.contains("Błąd połączenia")) {
            return;
        }

        int closeBracketPos = text.indexOf(']');
        if (closeBracketPos != -1) {
            QString serverUrl = text.mid(1, closeBracketPos - 1);
            QString filename = text.mid(closeBracketPos + 2);

            int dodanoPos = filename.indexOf(" (Dodano do:");
            if (dodanoPos != -1) {
                filename = filename.left(dodanoPos);
            }

            if (!serverUrl.endsWith("/")) serverUrl += "/";
            QString encodedFilename = QString::fromUtf8(QUrl::toPercentEncoding(filename));
            QString fullUrl = serverUrl + encodedFilename;

            emit playSongRequested(fullUrl);
        }
    });
    m_searchedSongsList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_playlistSongsList, &QListWidget::itemDoubleClicked, [this](QListWidgetItem *item) {
        int row = m_playlistSongsList->row(item);
        emit playlistSongRequested(m_currentPlaylistName, row);
    });
    m_playlistSongsList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_playlistSongsList, &QListWidget::customContextMenuRequested, [this](const QPoint &pos) {

        QListWidgetItem *item = m_playlistSongsList->itemAt(pos);
        if (!item) return;
        QMenu contextMenu(this);
        contextMenu.setStyleSheet("QMenu { background-color: #333333; color: white; border: 1px solid red; }"
                                  "QMenu::item:selected { background-color: #555555; }");

        QAction *removeAction = contextMenu.addAction("❌ Usuń utwór z playlisty");
        QAction *selectedAction = contextMenu.exec(m_playlistSongsList->mapToGlobal(pos));
        if (selectedAction == removeAction) {
            int row = m_playlistSongsList->row(item);
            size_t count = PlaylistManager::getInstance().getPlaylistCount();
            for (size_t i = 0; i < count; ++i) {
                Playlist* p = PlaylistManager::getInstance().getPlaylist(i);

                if (p && p->name == m_currentPlaylistName) {
                    p->remove_song(row);
                    break;
                }
            }
            showPlaylist(m_currentPlaylistName);
        }
    });
    connect(m_searchedSongsList, &QListWidget::customContextMenuRequested, [this](const QPoint &pos) {
        QListWidgetItem *item = m_searchedSongsList->itemAt(pos);
        if (!item) return;
        QString text = item->text();
        if (!text.startsWith("[") || text.contains("Nie znaleziono") || text.contains("Błąd połączenia")) {
            return;
        }
        QMenu contextMenu(this);
        contextMenu.setStyleSheet("QMenu { background-color: #333333; color: white; border: 1px solid blue; }"
                                  "QMenu::item:selected { background-color: #555555; }");

        QAction *header = contextMenu.addAction("Dodaj utwór do playlisty:");
        header->setEnabled(false);
        contextMenu.addSeparator();

        size_t count = PlaylistManager::getInstance().getPlaylistCount();

        if (count == 0) {
            contextMenu.addAction("(Brak playlist w pamięci programu!)")->setEnabled(false);
        } else {

            for (size_t i = 0; i < count; ++i) {
                Playlist* p = PlaylistManager::getInstance().getPlaylist(i);
                if (p && !p->name.isEmpty()) {
                    contextMenu.addAction(p->name);
                }
            }
        }


        QAction *selectedAction = contextMenu.exec(m_searchedSongsList->mapToGlobal(pos));

        if (selectedAction) {
            QString chosenPlaylist = selectedAction->text();

            if (chosenPlaylist == "(Brak playlist w pamięci programu!)") return;

            int closeBracketPos = text.indexOf(']');
            QString serverUrl = text.mid(1, closeBracketPos - 1);
            QString filename = text.mid(closeBracketPos + 2);


            int dodanoPos = filename.indexOf(" (Dodano do:");
            if (dodanoPos != -1) {
                filename = filename.left(dodanoPos);
            }

            if (!serverUrl.endsWith("/")) serverUrl += "/";
            QString encodedFilename = QString::fromUtf8(QUrl::toPercentEncoding(filename));
            QString fullUrl = serverUrl + encodedFilename;

            for (size_t i = 0; i < count; ++i) {
                Playlist* p = PlaylistManager::getInstance().getPlaylist(i);
                if (p && p->name == chosenPlaylist) {
                    p->add_song(fullUrl);
                    break;
                }
            }

            item->setText(text + " (Dodano do: " + chosenPlaylist + ")");
        }


    });

}

void MiddlePanel::performSearch(const QString& query) {
    if (query.isEmpty()) return;

    m_searchedSongsList->clear();

    std::vector<QString> servers = ServerManager::getInstance().getServers();

    if (servers.empty()) {
        m_searchedSongsList->addItem("Brak serwerów! Dodaj serwer w prawym panelu.");
        return;
    }

    m_searchedSongsList->addItem("Pobieranie indeksu plików z serwerów...");

    for (const QString& serverUrl : servers) {

        QNetworkRequest request((QUrl(serverUrl)));

        request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

        QNetworkReply *reply = m_networkManager->get(request);

        connect(reply, &QNetworkReply::finished, [this, reply, serverUrl, query]() {

            if (m_searchedSongsList->count() > 0 && m_searchedSongsList->item(0)->text() == "Pobieranie indeksu plików z serwerów...") {
                m_searchedSongsList->clear();
            }

            if (reply->error() == QNetworkReply::NoError) {
                QByteArray responseData = reply->readAll();
                QString htmlResponse(responseData);

                QRegularExpression regex("href=\"([^\"]+\\.(mp3|wav))\"", QRegularExpression::CaseInsensitiveOption);
                QRegularExpressionMatchIterator i = regex.globalMatch(htmlResponse);

                bool foundAnything = false;

                while (i.hasNext()) {
                    QRegularExpressionMatch match = i.next();
                    QString filename = match.captured(1);

                    filename = QUrl::fromPercentEncoding(filename.toUtf8());

                    if (filename.contains(query, Qt::CaseInsensitive)) {
                        m_searchedSongsList->addItem("[" + serverUrl + "] " + filename);
                        foundAnything = true;
                    }
                }

                if (!foundAnything) {
                    m_searchedSongsList->addItem("[" + serverUrl + "] Nie znaleziono plików pasujących do: " + query);
                }
            } else {
                m_searchedSongsList->addItem("[" + serverUrl + "] Błąd połączenia: " + reply->errorString());
            }

            reply->deleteLater();
        });

    }

}
void MiddlePanel::showPlaylist(const QString& playlistName) {
    m_currentPlaylistName = playlistName;
    m_playlistSongsList->clear();

    size_t count = PlaylistManager::getInstance().getPlaylistCount();
    for (size_t i = 0; i < count; ++i) {
        Playlist* p = PlaylistManager::getInstance().getPlaylist(i);

        if (p && p->name == playlistName) {
            for (size_t j = 0; j < p->songs.size(); ++j) {
                QString fullUrl = p->get_song(j);
                QString filename = QUrl(fullUrl).fileName();
                filename = QUrl::fromPercentEncoding(filename.toUtf8());
                QString displayName = QString::number(j + 1) + ". " + filename;

                QListWidgetItem *item = new QListWidgetItem(displayName);

                item->setData(Qt::UserRole, fullUrl);

                m_playlistSongsList->addItem(item);
            }
            break;
        }
    }

    m_songsStack->setCurrentIndex(0);
}