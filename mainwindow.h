#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <watchdog.h>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonSelectExe_clicked();

    void on_pushButtonStart_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    Watchdog *m_watchDog = nullptr;
    void sltAppendLog(const QString& text);
};
#endif // MAINWINDOW_H
