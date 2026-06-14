#ifndef BOTTOMBAR_H
#define BOTTOMBAR_H

#include <QWidget>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
class QLabel;
class QPushButton;
class QSlider;

class BottomBar : public QWidget {
    Q_OBJECT
public:
    explicit BottomBar(QWidget *parent = nullptr);
    QPushButton* getPlayButton() const { return m_playButton; }
    QSlider* getVolumeSlider() const { return m_volumeSlider; }
    QSlider* getProgressSlider() const { return m_progressSlider; }
    QLabel* getSongTitleLabel() const { return m_songTitleLabel; }
    QPushButton* getLoopButton() const { return m_loopButton; }
    QPushButton* getShuffleButton() const { return m_shuffleButton; }
    QPushButton* getNextButton() const { return m_nextButton; }
    QPushButton* getPrevButton() const { return m_prevButton; }
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
    QSlider *m_progressSlider;
};

#endif // BOTTOMBAR_H