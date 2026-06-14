#include "leftpanel.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>

LeftPanel::LeftPanel(QWidget *parent) : QWidget(parent) {
    setupUi();
}

void LeftPanel::setupUi() {
    setAttribute(Qt::WA_StyledBackground, true);
    setFixedWidth(200);
    setStyleSheet("background-color: #1b1818; border-right: 1px solid blue;");
    QVBoxLayout *layout = new QVBoxLayout(this);

    m_nickLabel = new QLabel("6tongranitu (Kliknij by zmienic)", this);
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
        m_newPlaylistEdit->setVisible(false); m_newPlaylistButton->setVisible(true);
    });
}