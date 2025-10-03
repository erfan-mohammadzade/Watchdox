#include "mainwindow.h"
#include <QFile>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file(":/Resource/style.qss");
    file.open(QFile::ReadOnly | QFile::Text);
    QString style = file.readAll();

    MainWindow w;
    w.show();
    a.setStyleSheet(style);
    a.setWindowIcon(QIcon(":/Resource/icon.ico"));
    return a.exec();
}
