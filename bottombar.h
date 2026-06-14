#ifndef BOTTOMBAR_H
#define BOTTOMBAR_H

#include <QWidget>

class QLabel;
class QPushButton;
class QSlider;

class BottomBar : public QWidget {
    Q_OBJECT
public:
    explicit BottomBar(QWidget *parent = nullptr);

private:
    void setupUi();

    QLabel *m_songTitleLabel;
    QPushButton *m_likeButton;
    QPushButton *m_loopButton;
    QPushButton *m_prevButton;
    QPushButton *m_playButton;
    QPushButton *m_nextButton;
    QPushButton *m_shuffleButton;
    QSlider *m_volumeSlider;
};

#endif // BOTTOMBAR_H