#ifndef RIGHTPANEL_H
#define RIGHTPANEL_H

#include <QWidget>

class RightPanel : public QWidget {
    Q_OBJECT
public:
    explicit RightPanel(QWidget *parent = nullptr);

signals:
    void addFileRequested();
    void addServerRequested();
    void removeServerRequested();

private:
    void setupUi();
};

#endif // RIGHTPANEL_H