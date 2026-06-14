#include <QPushButton>
#include <QLabel>
#include "mainwindow.h"
#include "leftpanel.h"
#include "middlepanel.h"
#include "rightpanel.h"
#include "playlistmanager.h"
#include "bottombar.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QFileDialog>
#include <QSlider>
#include <QMediaPlayer>
#include <QUrl>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QUrl>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    m_player = new QMediaPlayer(this);
    m_player->setVolume(100);
    setupUi();
}
MainWindow::~MainWindow() {}
void MainWindow::setupUi() {
    m_networkManager = new QNetworkAccessManager(this);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &MainWindow::uploadFinished);
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    centralWidget->setStyleSheet("background-color: #1b1818;");

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QHBoxLayout *panelsLayout = new QHBoxLayout();
    panelsLayout->setSpacing(0);

    m_leftPanel = new LeftPanel(this);
    m_middlePanel = new MiddlePanel(this);
    m_rightPanel = new RightPanel(this);
    m_bottomBar = new BottomBar(this);

    panelsLayout->addWidget(m_leftPanel);
    panelsLayout->addWidget(m_middlePanel, 1);
    panelsLayout->addWidget(m_rightPanel);

    mainLayout->addLayout(panelsLayout, 1);
    mainLayout->addWidget(m_bottomBar);
    connect(m_rightPanel, &RightPanel::addFileRequested, this, &MainWindow::showAddFileDialog);
    connect(m_rightPanel, &RightPanel::addServerRequested, this, &MainWindow::showAddServerDialog);
    connect(m_rightPanel, &RightPanel::removeServerRequested, this, &MainWindow::showRemoveServerDialog);
    connect(m_leftPanel, &LeftPanel::playlistSelected, m_middlePanel, &MiddlePanel::showPlaylist);
    connect(m_middlePanel, &MiddlePanel::playSongRequested, [this](const QString& url) {
        m_currentPlaylistName = "";
        m_currentSongIndex = -1;

        m_player->setMedia(QUrl(url));
        m_player->play();

        QString filename = QUrl(url).fileName();
        filename = QUrl::fromPercentEncoding(filename.toUtf8());
        m_bottomBar->getSongTitleLabel()->setText(filename);
        m_bottomBar->getPlayButton()->setText("⏸️");
    });
    connect(m_middlePanel, &MiddlePanel::playlistSongRequested, [this](const QString& playlistName, int index) {
        playSongFromPlaylist(playlistName, index);
    });
    connect(m_bottomBar->getPlayButton(), &QPushButton::clicked, [this]() {
        if (m_player->state() == QMediaPlayer::PlayingState) {
            m_player->pause();
            m_bottomBar->getPlayButton()->setText("▶️");
        } else {
            m_player->play();
            m_bottomBar->getPlayButton()->setText("⏸️");
        }
    });
    connect(m_bottomBar->getVolumeSlider(), &QSlider::valueChanged, [this](int value) {
        m_player->setVolume(value);
    });
    connect(m_bottomBar->getLoopButton(), &QPushButton::clicked, [this]() {
        m_isLooping = !m_isLooping;
        if (m_isLooping) {
            m_bottomBar->getLoopButton()->setStyleSheet("font-size: 38px; color: white; background-color: #333333; border: 1px solid blue; border-radius: 10px;");
        } else {
            m_bottomBar->getLoopButton()->setStyleSheet("font-size: 38px; color: white; background: transparent; border: none;");
        }
    });
    connect(m_bottomBar->getShuffleButton(), &QPushButton::clicked, [this]() {
        m_isShuffling = !m_isShuffling;
        if (m_isShuffling) {
            m_bottomBar->getShuffleButton()->setStyleSheet("font-size: 38px; color: white; background-color: #333333; border: 1px solid blue; border-radius: 10px;");
        } else {
            m_bottomBar->getShuffleButton()->setStyleSheet("font-size: 38px; color: white; background: transparent; border: none;");
        }
    });
    auto playNextSong = [this]() {
        if (m_currentPlaylistName.isEmpty() || m_currentSongIndex == -1) return;

        size_t count = PlaylistManager::getInstance().getPlaylistCount();
        for (size_t i = 0; i < count; ++i) {
            Playlist* p = PlaylistManager::getInstance().getPlaylist(i);
            if (p && p->name == m_currentPlaylistName) {
                int totalSongs = p->songs.size();
                if (totalSongs == 0) return;

                int nextIndex = m_currentSongIndex;
                if (m_isShuffling) {
                    nextIndex = rand() % totalSongs;
                } else {
                    nextIndex = (m_currentSongIndex + 1) % totalSongs;
                }
                playSongFromPlaylist(m_currentPlaylistName, nextIndex);
                break;
            }
        }
    };
    connect(m_bottomBar->getNextButton(), &QPushButton::clicked, playNextSong);
    connect(m_bottomBar->getPrevButton(), &QPushButton::clicked, [this]() {
        if (m_currentPlaylistName.isEmpty() || m_currentSongIndex == -1) return;

        size_t count = PlaylistManager::getInstance().getPlaylistCount();
        for (size_t i = 0; i < count; ++i) {
            Playlist* p = PlaylistManager::getInstance().getPlaylist(i);
            if (p && p->name == m_currentPlaylistName) {
                int totalSongs = p->songs.size();
                if (totalSongs == 0) return;

                int prevIndex = m_currentSongIndex;
                if (m_isShuffling) {
                    prevIndex = rand() % totalSongs;
                } else {
                    prevIndex = (m_currentSongIndex - 1 + totalSongs) % totalSongs;
                }
                playSongFromPlaylist(m_currentPlaylistName, prevIndex);
                break;
            }
        }
    });
    connect(m_player, &QMediaPlayer::mediaStatusChanged, [this, playNextSong](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            if (m_isLooping) {
                m_player->setPosition(0);
                m_player->play();
            } else {
                playNextSong();
            }
        }
    });
    connect(m_player, &QMediaPlayer::durationChanged, [this](qint64 duration) {
        m_bottomBar->getProgressSlider()->setRange(0, duration);
    });
    connect(m_player, &QMediaPlayer::positionChanged, [this](qint64 position) {
        m_bottomBar->getProgressSlider()->blockSignals(true);
        m_bottomBar->getProgressSlider()->setValue(position);
        m_bottomBar->getProgressSlider()->blockSignals(false);
    });
    connect(m_bottomBar->getProgressSlider(), &QSlider::sliderMoved, [this](int position) {
        m_player->setPosition(position);
    });
    connect(m_leftPanel, &LeftPanel::playlistSelected, m_middlePanel, &MiddlePanel::showPlaylist);
    connect(m_leftPanel, &LeftPanel::playlistDeleted, [this](const QString& deletedPlaylistName) {
        if (m_currentPlaylistName == deletedPlaylistName) {
            m_player->stop();
            m_currentPlaylistName = "";
            m_currentSongIndex = -1;
            m_bottomBar->getSongTitleLabel()->setText("Brak utworu");
            m_bottomBar->getPlayButton()->setText("▶️");
        }
        m_middlePanel->showPlaylist("");
    });
}
void MainWindow::showAddFileDialog() {
    QDialog dialog(this);
    dialog.setWindowTitle("Wgraj Plik na Serwer FTP");
    dialog.setStyleSheet("background-color: #333333; color: white;"
                         "QLineEdit { background-color: #555; border: 1px solid #777; }"
                         "QPushButton { background-color: #007bff; border: none; padding: 5px; }");

    QFormLayout form(&dialog);

    QLineEdit *ip = new QLineEdit("127.0.0.1", &dialog);
    QLineEdit *port = new QLineEdit("21", &dialog);
    QLineEdit *user = new QLineEdit("test", &dialog);
    QLineEdit *pass = new QLineEdit(&dialog);
    pass->setEchoMode(QLineEdit::Password);

    QPushButton *fBtn = new QPushButton("Wybierz plik...", &dialog);
    QLabel *pathL = new QLabel("Brak wybranego pliku", &dialog);
    QString selectedFilePath = "";

    connect(fBtn, &QPushButton::clicked, [&]() {
        QString path = QFileDialog::getOpenFileName(this, "Wybierz plik", "", "Pliki audio (*.mp3 *.wav)");
        if(!path.isEmpty()) {
            selectedFilePath = path;
            pathL->setText(path);
        }
    });

    form.addRow("Plik:", fBtn);
    form.addRow("", pathL);
    form.addRow("IP:", ip);
    form.addRow("Port:", port);
    form.addRow("User:", user);
    form.addRow("Pass:", pass);

    QPushButton *sub = new QPushButton("Rozpocznij wysyłanie", &dialog);
    form.addRow(sub);

    connect(sub, &QPushButton::clicked, &dialog, &QDialog::accept);
    if (dialog.exec() == QDialog::Accepted) {
        if (selectedFilePath.isEmpty()) {
            QMessageBox::warning(this, "Błąd", "Nie wybrano pliku!");
            return;
        }
        m_uploadFile = new QFile(selectedFilePath);
        if (!m_uploadFile->open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, "Błąd", "Nie można otworzyć pliku z dysku.");
            delete m_uploadFile;
            m_uploadFile = nullptr;
            return;
        }

        QFileInfo fileInfo(selectedFilePath);
        QString fileName = fileInfo.fileName();
        QUrl ftpUrl;
        ftpUrl.setScheme("ftp");
        ftpUrl.setHost(ip->text());
        ftpUrl.setPort(port->text().toInt());
        ftpUrl.setUserName(user->text());
        ftpUrl.setPassword(pass->text());
        ftpUrl.setPath("/" + fileName);
        QNetworkRequest request(ftpUrl);
        m_networkManager->put(request, m_uploadFile);
        m_bottomBar->getSongTitleLabel()->setText("Trwa wysyłanie: " + fileName + "...");
    }
}

void MainWindow::showAddServerDialog() {
    QDialog dialog(this); dialog.setWindowTitle("Add Server"); QFormLayout form(&dialog);
    QLineEdit *srv = new QLineEdit(&dialog); form.addRow("Adres serwera:", srv);
    QPushButton *sub = new QPushButton("Dodaj", &dialog); form.addRow(sub);
    connect(sub, &QPushButton::clicked, &dialog, &QDialog::accept);
    dialog.exec();
}

void MainWindow::showRemoveServerDialog() {
    QDialog dialog(this); dialog.setWindowTitle("Remove Server"); QFormLayout form(&dialog);
    QLineEdit *srv = new QLineEdit(&dialog); form.addRow("Adres do usuniecia:", srv);
    QPushButton *sub = new QPushButton("Usun", &dialog); form.addRow(sub);
    connect(sub, &QPushButton::clicked, &dialog, &QDialog::accept);
    dialog.exec();
}
void MainWindow::playSongFromPlaylist(const QString& playlistName, int index) {
    size_t count = PlaylistManager::getInstance().getPlaylistCount();
    for (size_t i = 0; i < count; ++i) {
        Playlist* p = PlaylistManager::getInstance().getPlaylist(i);
        if (p && p->name == playlistName) {
            if (index >= 0 && index < static_cast<int>(p->songs.size())) {
                m_currentPlaylistName = playlistName;
                m_currentSongIndex = index;

                QString fullUrl = p->get_song(index);
                m_player->setMedia(QUrl(fullUrl));
                m_player->play();
                QString filename = QUrl(fullUrl).fileName();
                filename = QUrl::fromPercentEncoding(filename.toUtf8());
                m_bottomBar->getSongTitleLabel()->setText(filename);
                m_bottomBar->getPlayButton()->setText("⏸️");
            }
            break;
        }
    }
}
void MainWindow::uploadFinished(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QMessageBox::information(this, "Sukces", "Plik został pomyślnie wgrany na serwer FTP!");
        m_bottomBar->getSongTitleLabel()->setText("Wysyłanie zakończone.");
    } else {
        QMessageBox::critical(this, "Błąd FTP", "Nie udało się wgrać pliku:\n" + reply->errorString());
        m_bottomBar->getSongTitleLabel()->setText("Błąd wysyłania.");
    }
    if (m_uploadFile) {
        m_uploadFile->close();
        m_uploadFile->deleteLater();
        m_uploadFile = nullptr;
    }
    reply->deleteLater();
}