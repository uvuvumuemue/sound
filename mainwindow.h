#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
class LeftPanel;
class MiddlePanel;
class RightPanel;
class BottomBar;
class QMediaPlayer;
class QAudioOutput;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void uploadFinished(QNetworkReply *reply);
private slots:
    void showAddFileDialog();
    void showAddServerDialog();
    void showRemoveServerDialog();

private:
    void setupUi();
    QNetworkAccessManager *m_networkManager;
    QFile *m_uploadFile = nullptr;
    LeftPanel *m_leftPanel;
    MiddlePanel *m_middlePanel;
    RightPanel *m_rightPanel;
    BottomBar *m_bottomBar;
    QMediaPlayer *m_player;
    bool m_isLooping = false;
    bool m_isShuffling = false;
    QString m_currentPlaylistName = "";
    int m_currentSongIndex = -1;
    void playSongFromPlaylist(const QString& playlistName, int index);
};

#endif // MAINWINDOW_H