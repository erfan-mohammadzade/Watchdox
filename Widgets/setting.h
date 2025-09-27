#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include <QFileDialog>
#include <QDataStream>
#include <global_watchdox.h>

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

private:
    Ui::Setting *ui;
    SettingInfo m_settingInfo;
    void saveSetting();
    void loadSetting();
    void loadSettingToUI();
Q_SIGNALS:
    void sigSettingUpdated();
};

#endif // SETTING_H
