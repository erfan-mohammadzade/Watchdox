#include "setting.h"
#include "ui_setting.h"

Setting::Setting(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Setting)
{
    ui->setupUi(this);
    loadSetting();
}

Setting::~Setting()
{
    delete ui;
}

void Setting::showSetting()
{
    loadSetting();
    loadSettingToUI();
    show();
}

void Setting::on_pushButtonOk_clicked()
{
    m_settingInfo.exePath = ui->lineEditDefaultPath->text();
    m_settingInfo.hasAutoStartEnabled = ui->checkBoxAutoStart->isChecked();
    m_settingInfo.autoStartTimer = ui->spinBoxSetTimer->value()*1000;
    saveSetting();
    Q_EMIT sigSettingUpdated();
    close();
}


void Setting::on_pushButtonCancel_clicked()
{
    close();
}

SettingInfo Setting::settingInfo() const
{
    return m_settingInfo;
}

void Setting::saveSetting()
{
    QFile settingFile(QDir::currentPath() + QDir::separator() + "enc.bin");
    if(settingFile.open(QIODevice::WriteOnly))
    {
        QDataStream stream(&settingFile);
        stream << m_settingInfo;
    }
}

void Setting::loadSetting()
{
    QFile settingFile(QDir::currentPath() + QDir::separator() + "enc.bin");
    if(settingFile.open(QIODevice::ReadOnly))
    {
        QDataStream stream(&settingFile);
        stream >> m_settingInfo;
    }
    Q_EMIT sigSettingUpdated();
}

void Setting::loadSettingToUI()
{
    ui->lineEditDefaultPath->setText(m_settingInfo.exePath);
    ui->spinBoxSetTimer->setValue(m_settingInfo.autoStartTimer/1000);
    ui->checkBoxAutoStart->setChecked(m_settingInfo.hasAutoStartEnabled);
}


void Setting::on_pushButtonSetDefaultPath_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(nullptr,
                                                    "Select EXE File of your app",
                                                    QDir::homePath(),                   // default directory
                                                    "Executable Files (*.exe)");
    if(filePath.isEmpty())
        return;
    else
    {
        ui->lineEditDefaultPath->setText(filePath);
        m_settingInfo.exePath = filePath;
    }
}



