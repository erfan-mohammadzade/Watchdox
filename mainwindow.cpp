#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_setting = new Setting();
    m_authentication = new Authentication();
    connect(m_setting, &Setting::sigSettingUpdated, this, &MainWindow::sltSettingUpdated);
    connect(m_setting, &Setting::sigOpenAuth, this, &MainWindow::sltOpenAuth);

    m_autoStartTimer = new QTimer(this);
    m_autoStartTimer->setSingleShot(true);
    connect(m_autoStartTimer, &QTimer::timeout, this, &MainWindow::sltAutoStartTimer);
    if(m_setting->settingInfo().hasAutoStartTimerValid())
    {
        ui->textEdit->append("-Auto start timer activated");
        m_autoStartTimer->start(m_setting->settingInfo().autoStartTimer);
    }
    else
    {
        ui->textEdit->append("-Auto start timer not valid");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    m_setting->deleteLater();
    m_authentication->deleteLater();
    m_autoStartTimer->stop();
    m_autoStartTimer->deleteLater();
}

void MainWindow::on_pushButtonStart_clicked(bool checked)
{
    QString filePath = ui->lineEditPath->text();
    if(filePath.isEmpty())
    {
        filePath = m_setting->settingInfo().exePath;
        ui->lineEditPath->setText(filePath);
    }

    // Check for valid path
    if (filePath.isEmpty() || !QFile::exists(filePath)) {
        QMessageBox::warning(this, "Invalid Path", "Please provide a valid file path.");
        ui->pushButtonStart->setChecked(false);
        return;
    }

    if (checked) {
        // Start
        ui->pushButtonStart->setText("Stop");

        // Prevent multiple watchdogs
        if (!m_watchDog)
        {
            m_watchDog = new Watchdog(filePath, this);
            connect(m_watchDog, &Watchdog::sidSendLog, this, &MainWindow::sltAppendLog);
            m_watchDog->setSettingInfo(m_settingInfo);
            m_watchDog->startProgram();
            ui->textEdit->append("Proceess started");
        }
    } else {
        // Stop
        ui->pushButtonStart->setText("Start");

        if (m_watchDog) {
            m_watchDog->deleteLater();
            m_watchDog = nullptr;
            ui->textEdit->append("Proceess stoped");
        }
    }
}

void MainWindow::sltAppendLog(const QString &text)
{
    ui->textEdit->append(text);
}

void MainWindow::sltSettingUpdated()
{
    m_settingInfo = m_setting->settingInfo();
    if(m_watchDog) m_watchDog->setSettingInfo(m_settingInfo);
    ui->lineEditPath->setText(m_settingInfo.exePath);
    ui->textEdit->append("Setting updated");
    ui->lineEditPath->setText(m_settingInfo.exePath);
}

void MainWindow::sltOpenAuth()
{
    m_authentication->show();
}

void MainWindow::sltAutoStartTimer()
{
    on_pushButtonStart_clicked(true);
    ui->pushButtonStart->setChecked(true);
    ui->pushButtonStart->setText("Stop");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(m_settingInfo.hasGetClosePasswordRequested)
    {
        DialogEnterPassword password;
        if(password.exec()==QDialog::Accepted)
        {
            if(!m_authentication->verifyPassword(password.password()))
                event->ignore();
        }
        else
            event->ignore();
    }
}



void MainWindow::on_pushButtonSetting_clicked()
{
    DialogEnterPassword password;
    if(password.exec()==QDialog::Accepted)
    {
        if(m_authentication->verifyPassword(password.password()))
            m_setting->showSetting();
    }
}


