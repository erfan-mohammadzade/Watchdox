#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include <QFileDialog>
#include <QDataStream>
#include <global_watchdox.h>
#include <QDesktopServices>
#include <QUrl>
#include <QSettings>

namespace Ui {
class Setting;
}

class Setting : public QWidget
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = nullptr);
    ~Setting();
    void showSetting();

    SettingInfo settingInfo() const;

private slots:
    void on_pushButtonOk_clicked();

    void on_pushButtonCancel_clicked();

    void on_pushButtonSetDefaultPath_clicked();

    void on_pushButtonGitView_clicked();

    void on_pushButtonChangeOrEnterPassword_clicked();

private:
    Ui::Setting *ui;
    SettingInfo m_settingInfo;
    QSettings *m_setting;
    void saveSetting();
    void loadSetting();
Q_SIGNALS:
    void sigSettingUpdated();
    void sigOpenAuth();

};

#endif // SETTING_H
