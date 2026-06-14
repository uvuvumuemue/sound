#include "bottombar.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>

BottomBar::BottomBar(QWidget *parent) : QWidget(parent) {
    setupUi();
}

void BottomBar::setupUi() {
    setAttribute(Qt::WA_StyledBackground, true);

    setFixedHeight(100);

    setStyleSheet("BottomBar { background-color: #1b1818; border-top: 1px solid blue; }");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 5, 20, 5);
    mainLayout->setSpacing(5);
    m_progressSlider = new QSlider(Qt::Horizontal, this);
    m_progressSlider->setRange(0, 0);
    m_progressSlider->setStyleSheet(
        "QSlider::groove:horizontal { background: #333333; height: 4px; border-radius: 2px; }"
        "QSlider::sub-page:horizontal { background: blue; border-radius: 2px; }"
        "QSlider::handle:horizontal { background: white; width: 12px; margin: -4px 0; border-radius: 6px; }"
        );
    mainLayout->addWidget(m_progressSlider);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(20, 10, 20, 10);
    layout->setSpacing(15);

    m_songTitleLabel = new QLabel("Brak utworu", this);
    m_songTitleLabel->setStyleSheet("color: white; font-size: 18px; border: none; font-weight: bold;");
    m_songTitleLabel->setFixedWidth(200);


    QString iconStyle =
        "QPushButton { font-size: 38px; color: white; background: transparent; border: none; }"
        "QPushButton:hover { background-color: #333333; border-radius: 10px; }";

    m_likeButton = new QPushButton("❤️", this);
    m_loopButton = new QPushButton("🔁", this);
    m_prevButton = new QPushButton("⏮️", this);
    m_playButton = new QPushButton("▶️", this);
    m_nextButton = new QPushButton("⏭️", this);
    m_shuffleButton = new QPushButton("🔀", this);

    m_likeButton->setStyleSheet(iconStyle);
    m_loopButton->setStyleSheet(iconStyle);
    m_prevButton->setStyleSheet(iconStyle);
    m_playButton->setStyleSheet(iconStyle);
    m_nextButton->setStyleSheet(iconStyle);
    m_shuffleButton->setStyleSheet(iconStyle);

    m_volumeSlider = new QSlider(Qt::Horizontal, this);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setValue(100);
    m_volumeSlider->setFixedWidth(120);
    m_volumeSlider->setStyleSheet("QSlider::handle:horizontal { background: blue; width: 15px; margin: -5px 0; border-radius: 7px; }");
    layout->addWidget(m_songTitleLabel);
    layout->addWidget(m_likeButton);
    layout->addStretch();
    layout->addWidget(m_loopButton);
    layout->addWidget(m_prevButton);
    layout->addWidget(m_playButton);
    layout->addWidget(m_nextButton);
    layout->addWidget(m_shuffleButton);
    layout->addStretch();
    layout->addWidget(m_volumeSlider);
    connect(m_playButton, &QPushButton::clicked, [this](){
        if(m_playButton->text() == "▶️") m_playButton->setText("⏸️");
        else m_playButton->setText("▶️");
    });
    mainLayout->addLayout(layout);
}