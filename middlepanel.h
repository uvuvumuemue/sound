#ifndef MIDDLEPANEL_H
#define MIDDLEPANEL_H

#include <QWidget>

class QLabel;
class QLineEdit;
class QStackedWidget;
class QListWidget;
class QPushButton;

class MiddlePanel : public QWidget {
    Q_OBJECT
public:
    explicit MiddlePanel(QWidget *parent = nullptr);

private:
    void setupUi();

    QLabel *m_currentPlaylistTitle;
    QLineEdit *m_songSearchEdit;
    QStackedWidget *m_songsStack;
    QListWidget *m_playlistSongsList;
    QListWidget *m_searchedSongsList;
    QPushButton *m_sortBtn;
};

#endif // MIDDLEPANEL_H