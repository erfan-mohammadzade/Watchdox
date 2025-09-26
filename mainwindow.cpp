#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
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
    if(ui->lineEditPath->text().isEmpty() || !QFile(ui->lineEditPath->text()).exists()) return;
    if(checked)
    {
        ui->pushButtonStart->setText("Stop");
        QString filePath = ui->lineEditPath->text();
        if(!QFile(filePath).exists())
            return;
        m_watchDog = new Watchdog(filePath);
        connect(m_watchDog, &Watchdog::sidSendLog, this, &MainWindow::sltAppendLog);
    }
    else
    {
        ui->pushButtonStart->setText("Start");
        m_watchDog->deleteLater();
    }
}

void MainWindow::sltAppendLog(const QString &text)
{
    ui->textEdit->append(text);
}

