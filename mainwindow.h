#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <watchdog.h>
#include <Widgets/setting.h>
#include <QTimer>
#include <QCloseEvent>
#include <Widgets/dialogenterpassword.h>

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

    void on_pushButtonSetting_clicked();

private:
    Ui::MainWindow *ui;
    Watchdog *m_watchDog = nullptr;
    Setting  *m_setting = nullptr;
    SettingInfo settingInfo;
    QTimer *m_autoStartTimer = nullptr;
    void getSetting();
private Q_SLOTS:
    void sltAppendLog(const QString& text);
    void sltSettingUpdated();

public Q_SLOTS:
    void sltAutoStartTimer();

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
