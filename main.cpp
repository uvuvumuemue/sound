#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    srand (time(NULL));
    app.setStyleSheet(
        "QDialog { background-color: #1b1818; }"
        "QDialog QLabel { color: white; }"
        "QDialog QLineEdit { background-color: #333333; color: white; border: 1px solid blue; padding: 3px; }"
        "QDialog QPushButton { background-color: #333333; color: white; border: 1px solid blue; padding: 5px; min-width: 70px; }"
        "QDialog QPushButton:hover { background-color: #444444; }"
        "QDialog QComboBox { background-color: #333333; color: white; border: 1px solid blue; padding: 3px; }"
        "QDialog QComboBox QAbstractItemView { background-color: #333333; color: white; selection-background-color: #0055ff; outline: none; }"
        );
    MainWindow window;
    window.show();
    return app.exec();
}