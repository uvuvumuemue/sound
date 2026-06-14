#include "leftpanel.h"
#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include "playlistmanager.h"
#include <QFileDialog>
#include <memory>
#include "zapis.h"
#include "odczyt.h"
LeftPanel::LeftPanel(QWidget *parent) : QWidget(parent) {
    setupUi();
}

void LeftPanel::setupUi() {
    setAttribute(Qt::WA_StyledBackground, true);
    setFixedWidth(200);
    setStyleSheet("background-color: #1b1818; border-right: 1px solid blue;");
    QVBoxLayout *layout = new QVBoxLayout(this);

    m_nickLabel = new QLabel("User", this);
    m_nickLabel->setStyleSheet("color: white; border: 1px solid blue; padding: 5px;");
    m_nickLabel->setAlignment(Qt::AlignCenter);
    m_nickLabel->setWordWrap(true);
    m_nickEdit = new QLineEdit(this);
    m_nickEdit->setVisible(false);
    m_nickEdit->setStyleSheet("color: white; background-color: #1b1818;");
    layout->addWidget(m_nickLabel);
    layout->addWidget(m_nickEdit);

    m_newPlaylistButton = new QPushButton("nowa playlista", this);
    m_newPlaylistButton->setStyleSheet("color: #BBBBBB; font-size: 14px; border: 1px solid gray;");
    m_newPlaylistEdit = new QLineEdit(this);
    m_newPlaylistEdit->setPlaceholderText("stworz playliste");
    m_newPlaylistEdit->setVisible(false);
    m_newPlaylistEdit->setStyleSheet("color: white; background-color: #1b1818;");
    layout->addWidget(m_newPlaylistButton);
    layout->addWidget(m_newPlaylistEdit);

    m_playlistsListWidget = new QListWidget(this);
    m_playlistsListWidget->setStyleSheet("color: white; background: transparent; border: none; font-size: 14px;");
    m_playlistsListWidget->addItem("Ulubione");
    PlaylistManager::getInstance().addPlaylist("Ulubione");
    layout->addWidget(m_playlistsListWidget);

    m_saveBtn = new QPushButton("Zapisz Playlistę", this);
    m_loadBtn = new QPushButton("Wczytaj Playlistę", this);
    m_saveBtn->setStyleSheet("color: white; background-color: #333;");
    m_loadBtn->setStyleSheet("color: white; background-color: #333;");
    layout->addWidget(m_saveBtn);
    layout->addWidget(m_loadBtn);
    connect(m_newPlaylistButton, &QPushButton::clicked, [this](){ 
        m_newPlaylistButton->setVisible(false); m_newPlaylistEdit->setVisible(true); m_newPlaylistEdit->setFocus(); 
    });
    connect(m_newPlaylistEdit, &QLineEdit::editingFinished, [this](){
        QString newName = m_newPlaylistEdit->text().trimmed();
        if (!newName.isEmpty()) {
            PlaylistManager::getInstance().addPlaylist(newName);
            m_playlistsListWidget->addItem(newName);
        }
        m_newPlaylistEdit->clear();
        m_newPlaylistEdit->setVisible(false);
        m_newPlaylistButton->setVisible(true);
    });
    connect(m_playlistsListWidget, &QListWidget::itemClicked, [this](QListWidgetItem *item) {
        emit playlistSelected(item->text());
    });
    m_playlistsListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_playlistsListWidget, &QListWidget::customContextMenuRequested, [this](const QPoint &pos) {

        QListWidgetItem *item = m_playlistsListWidget->itemAt(pos);
        if (!item) return;

        QMenu contextMenu(this);
        contextMenu.setStyleSheet("QMenu { background-color: #333333; color: white; border: 1px solid red; }"
                                  "QMenu::item:selected { background-color: #555555; }");

        QAction *removeAction = contextMenu.addAction("❌ Usuń playlistę");
        QAction *selectedAction = contextMenu.exec(m_playlistsListWidget->mapToGlobal(pos));

        if (selectedAction == removeAction) {
            QString playlistName = item->text();
            PlaylistManager::getInstance().removePlaylist(playlistName);
            delete item;
            emit playlistDeleted(playlistName);
        }
    });
    connect(m_saveBtn, &QPushButton::clicked, [this]() {
        QString selectedFilter;
        QString fileName = QFileDialog::getSaveFileName(this, "Zapisz playlisty", "",
                                                        "Plik JSON (*.json);;Plik CSV (*.csv);;Plik XML (*.xml)", &selectedFilter);
        if(fileName.isEmpty()) return;
        std::unique_ptr<Zapis> io = nullptr;
        if (selectedFilter.contains("JSON")) {
            io = std::make_unique<ZapisJSON>();
        } else if (selectedFilter.contains("CSV")) {
            io = std::make_unique<ZapisCSV>();
        } else if (selectedFilter.contains("XML")) {
            io = std::make_unique<ZapisXML>();
        }
        if (io) {
            io->zapisz(fileName);
        }
    });
    connect(m_loadBtn, &QPushButton::clicked, [this]() {
        QString fileName = QFileDialog::getOpenFileName(this, "Wczytaj playlisty", "",
                                                        "Wszystkie formaty (*.json *.csv *.xml);;Plik JSON (*.json);;Plik CSV (*.csv);;Plik XML (*.xml)");

        if(fileName.isEmpty()) return;

        bool success = false;
        if (fileName.endsWith(".json", Qt::CaseInsensitive)) {
            success = wykonajOdczyt<OdczytJSON>(fileName);
        } else if (fileName.endsWith(".csv", Qt::CaseInsensitive)) {
            success = wykonajOdczyt<OdczytCSV>(fileName);
        } else if (fileName.endsWith(".xml", Qt::CaseInsensitive)) {
            success = wykonajOdczyt<OdczytXML>(fileName);
        }
        if (success) {
            m_playlistsListWidget->clear();
            size_t count = PlaylistManager::getInstance().getPlaylistCount();
            for (size_t i = 0; i < count; ++i) {
                Playlist* p = PlaylistManager::getInstance().getPlaylist(i);
                if (p) m_playlistsListWidget->addItem(p->name);
            }
        }
    });
}