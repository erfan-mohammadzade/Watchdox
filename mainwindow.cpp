#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_setting = new Setting();
    connect(m_setting, &Setting::sigSettingUpdated, this, &MainWindow::sltSettingUpdated);

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
    m_autoStartTimer->stop();
    m_autoStartTimer->deleteLater();
}

void MainWindow::on_pushButtonSelectExe_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(nullptr,
                                                    "Select EXE File of your app",
                                                    QDir::homePath(),                   // default directory
                                                    "Executable Files (*.exe)");
    if(filePath.isEmpty())
        return;
    else
        ui->lineEditPath->setText(filePath);
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
        if (!m_watchDog) {
            m_watchDog = new Watchdog(filePath, this); // Set parent to avoid leaks
            connect(m_watchDog, &Watchdog::sidSendLog, this, &MainWindow::sltAppendLog);
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
    getSetting();
}

void MainWindow::sltAutoStartTimer()
{
    on_pushButtonStart_clicked(true);
    ui->pushButtonStart->setChecked(true);
    ui->pushButtonStart->setText("Stop");
}

void MainWindow::getSetting()
{
    settingInfo = m_setting->settingInfo();
    ui->lineEditPath->setText(settingInfo.exePath);
}

void MainWindow::on_pushButtonSetting_clicked()
{
    m_setting->showSetting();
}

