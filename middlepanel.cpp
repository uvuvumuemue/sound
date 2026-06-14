#include "middlepanel.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QStackedWidget>
#include <QListWidget>
#include <QPushButton>

MiddlePanel::MiddlePanel(QWidget *parent) : QWidget(parent) {
    setupUi();
}

void MiddlePanel::setupUi() {
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet("background-color: #222020;");
    QVBoxLayout *layout = new QVBoxLayout(this);

    m_currentPlaylistTitle = new QLabel("Kliknij tu aby szukac utworu", this);
    m_currentPlaylistTitle->setStyleSheet("color: white; font-size: 14px; border: 1px solid blue; padding: 5px;");
    m_currentPlaylistTitle->setAlignment(Qt::AlignCenter);
    
    m_songSearchEdit = new QLineEdit(this);
    m_songSearchEdit->setPlaceholderText("Wpisz nazwe pliku");
    m_songSearchEdit->setVisible(false);
    m_songSearchEdit->setStyleSheet("color: white; background-color: #333030;");
    layout->addWidget(m_currentPlaylistTitle);
    layout->addWidget(m_songSearchEdit);

    m_songsStack = new QStackedWidget(this);
    m_playlistSongsList = new QListWidget(m_songsStack);
    m_playlistSongsList->setStyleSheet("color: white; background-color: #373434; font-size: 14px;");

    m_searchedSongsList = new QListWidget(m_songsStack);
    m_searchedSongsList->setStyleSheet("color: white; background-color: #484545; font-size: 14px;");
    
    m_songsStack->addWidget(m_playlistSongsList);
    m_songsStack->addWidget(m_searchedSongsList);
    layout->addWidget(m_songsStack);

    m_sortBtn = new QPushButton("Sortuj utwory alfabetycznie (STL)", this);
    m_sortBtn->setStyleSheet("color: white; background-color: #444; padding: 5px;");
    layout->addWidget(m_sortBtn);
}