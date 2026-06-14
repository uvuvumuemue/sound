#include "rightpanel.h"
#include <QVBoxLayout>
#include <QPushButton>

RightPanel::RightPanel(QWidget *parent) : QWidget(parent) {
    setupUi();
}

void RightPanel::setupUi() {
    setAttribute(Qt::WA_StyledBackground, true);
    setFixedWidth(130);
    setStyleSheet("background-color: #1b1818; border-left: 1px solid blue;");
    QVBoxLayout *layout = new QVBoxLayout(this);

    QPushButton *addFileBtn = new QPushButton("add file", this);
    QPushButton *addServerBtn = new QPushButton("add server", this);
    QPushButton *removeServerBtn = new QPushButton("remove server", this);
    
    QString btnStyle = "color: #BBBBBB; font-size: 13px; padding: 8px; border: 1px solid blue; margin-bottom: 5px;";
    addFileBtn->setStyleSheet(btnStyle); 
    addServerBtn->setStyleSheet(btnStyle); 
    removeServerBtn->setStyleSheet(btnStyle);
    
    layout->addWidget(addFileBtn); 
    layout->addWidget(addServerBtn); 
    layout->addWidget(removeServerBtn);
    layout->addStretch();
}