#include "setting.h"
#include "ui_setting.h"

Setting::Setting(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Setting)
{
    m_setting = new QSettings("EMAPP", "MyAppGeneral");
    ui->setupUi(this);
    loadSetting();
    Q_EMIT sigSettingUpdated();
    ui->labelAppVersion->setText(APPVERSION);
}

Setting::~Setting()
{
    delete ui;
}

void Setting::showSetting()
{
    loadSetting();
    show();
}

void Setting::on_pushButtonOk_clicked()
{
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
    m_settingInfo.exePath = ui->lineEditDefaultPath->text();
    m_settingInfo.hasAutoStartEnabled = ui->checkBoxAutoStart->isChecked();
    m_settingInfo.autoStartTimer = ui->spinBoxSetTimer->value()*1000;
    m_settingInfo.hasCloseOtherAppRequested = ui->checkBoxCloseOtherApp->isChecked();
    m_settingInfo.hasNormalExitOpenRequested = ui->checkBoxNormalExitRestartProgram->isChecked();
    m_settingInfo.hasGetClosePasswordRequested = ui->checkBoxGetPasswordByClosing->isChecked();

    m_setting->setValue("exePath", m_settingInfo.exePath);
    m_setting->setValue("autoStart", m_settingInfo.hasAutoStartEnabled);
    m_setting->setValue("autoStartTimer", m_settingInfo.autoStartTimer);
    m_setting->setValue("closeOtherAppInStart", m_settingInfo.hasCloseOtherAppRequested);
    m_setting->setValue("normalExitRestart", m_settingInfo.hasNormalExitOpenRequested);
    m_setting->setValue("getClosePassword", m_settingInfo.hasGetClosePasswordRequested);
    m_setting->sync();
}

void Setting::loadSetting()
{
    m_settingInfo.exePath = m_setting->value("exePath").toString();
    m_settingInfo.hasAutoStartEnabled = m_setting->value("autoStart", false).toBool();
    m_settingInfo.autoStartTimer = m_setting->value("autoStartTimer", 2000).toInt();
    m_settingInfo.hasCloseOtherAppRequested = m_setting->value("closeOtherAppInStart", false).toBool();
    m_settingInfo.hasNormalExitOpenRequested = m_setting->value("normalExitRestart", false).toBool();
    m_settingInfo.hasGetClosePasswordRequested = m_setting->value("getClosePassword", true).toBool();

    ui->lineEditDefaultPath->setText(m_settingInfo.exePath);
    ui->spinBoxSetTimer->setValue(m_settingInfo.autoStartTimer/1000);
    ui->checkBoxAutoStart->setChecked(m_settingInfo.hasAutoStartEnabled);
    ui->checkBoxCloseOtherApp->setChecked(m_settingInfo.hasCloseOtherAppRequested);
    ui->checkBoxNormalExitRestartProgram->setChecked(m_settingInfo.hasNormalExitOpenRequested);
    ui->checkBoxGetPasswordByClosing->setChecked(m_settingInfo.hasGetClosePasswordRequested);
}

void Setting::on_pushButtonSetDefaultPath_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(nullptr,
                                                    "Select EXE File of your app",
                                                    QDir::homePath(),                   // default directory
                                                    "Executable Files (*.exe)");
    filePath = QDir::toNativeSeparators(filePath);
    if(filePath.isEmpty())
        return;
    else
    {
        ui->lineEditDefaultPath->setText(filePath);
        m_settingInfo.exePath = filePath;
    }
}




void Setting::on_pushButtonGitView_clicked()
{
    QDesktopServices::openUrl(QUrl("https://github.com/erfan-mohammadzade"));
}


void Setting::on_pushButtonChangeOrEnterPassword_clicked()
{
    Q_EMIT sigOpenAuth();
}

