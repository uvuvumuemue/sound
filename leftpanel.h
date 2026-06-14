#ifndef LEFTPANEL_H
#define LEFTPANEL_H

#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;
class QListWidget;

class LeftPanel : public QWidget {
    Q_OBJECT
public:
    explicit LeftPanel(QWidget *parent = nullptr);

private:
    void setupUi();

    QLabel *m_nickLabel;
    QLineEdit *m_nickEdit;
    QPushButton *m_newPlaylistButton;
    QLineEdit *m_newPlaylistEdit;
    QListWidget *m_playlistsListWidget;
    QPushButton *m_saveBtn;
    QPushButton *m_loadBtn;
};

#endif // LEFTPANEL_H