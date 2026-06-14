#ifndef MIDDLEPANEL_H
#define MIDDLEPANEL_H

#include <QWidget>

class QLabel;
class QLineEdit;
class QStackedWidget;
class QListWidget;
class QPushButton;
class QNetworkAccessManager;
class MiddlePanel : public QWidget {
    Q_OBJECT
public:
    explicit MiddlePanel(QWidget *parent = nullptr);
signals:
    void playSongRequested(const QString& songUrl);
    void playlistSongRequested(const QString& playlistName, int index);
public slots:
    void showPlaylist(const QString& playlistName);
private:
    void setupUi();
    void performSearch(const QString& query);
    QString m_currentPlaylistName;
    QListWidget *m_songListWidget;
    QNetworkAccessManager *m_networkManager;
    QLineEdit *m_songSearchEdit;
    QStackedWidget *m_songsStack;
    QListWidget *m_playlistSongsList;
    QListWidget *m_searchedSongsList;
    QPushButton *m_sortBtn;
};

#endif // MIDDLEPANEL_H