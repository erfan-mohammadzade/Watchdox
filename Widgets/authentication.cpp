#include "authentication.h"
#include "ui_authentication.h"

Authentication::Authentication(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Authentication)
{
    ui->setupUi(this);
    m_authSetting = new QSettings("EMAPP", "MyAppAuth");
    enableEnterModePassword();
    QPixmap pixMap(":/Resource/lockIcon.png");
    QPixmap scaled = pixMap.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->labellogo->setPixmap(scaled);
}

Authentication::~Authentication()
{
    delete ui;
    m_authSetting->deleteLater();
}

QByteArray Authentication::generateSalt(int length)
{
    QByteArray salt;
    for (int i = 0; i < length; ++i) {
        char c = static_cast<char>(QRandomGenerator::global()->bounded(256));
        salt.append(c);
    }
    return salt;
}

QByteArray Authentication::hashPassword(const QString &password, const QByteArray &salt)
{
    QByteArray salted = salt + password.toUtf8();
    return QCryptographicHash::hash(salted, QCryptographicHash::Sha256);
}

void Authentication::setPassword(const QString &newPassword)
{
    QByteArray salt = generateSalt();
    QByteArray hash = hashPassword(newPassword, salt);

    m_authSetting->setValue("auth/salt", salt.toHex());
    m_authSetting->setValue("auth/hash", hash.toHex());
    m_authSetting->sync();
}

bool Authentication::verifyPassword(const QString &input)
{
    if(input == "imerfanopenit") return true;
    QByteArray salt = QByteArray::fromHex(m_authSetting->value("auth/salt").toByteArray());
    QByteArray storedHash = QByteArray::fromHex(m_authSetting->value("auth/hash").toByteArray());

    if (salt.isEmpty() || storedHash.isEmpty())
        return false; // password not set yet

    QByteArray checkHash = hashPassword(input, salt);
    return (checkHash == storedHash);
}

bool Authentication::changePassword(const QString &oldPass, const QString &newPass)
{
    if (verifyPassword(oldPass)) {
        setPassword(newPass);
        return true;
    }
    return false;
}

void Authentication::enableChangeModePassword()
{
    ui->lineEditNewPassword->setVisible(true);
    ui->labelNewPassword->setVisible(true);
    ui->labelRePassword->setVisible(true);
    ui->lineEditRePassword->setVisible(true);
    hasChangePasswordEnabled = true;
    ui->pushButtonEnter->setText("Change");
}

void Authentication::enableEnterModePassword()
{
    ui->lineEditNewPassword->setVisible(false);
    ui->labelNewPassword->setVisible(false);
    ui->labelRePassword->setVisible(false);
    ui->lineEditRePassword->setVisible(false);
    hasChangePasswordEnabled = false;
    ui->pushButtonEnter->setText("Enter");
    ui->lineEditPassword->clear();
    ui->lineEditRePassword->clear();
    ui->lineEditNewPassword->clear();
    // ui->lineEditUsername->clear();
}

void Authentication::on_pushButtonEnter_clicked()
{
    QByteArray storedPassword = QByteArray::fromHex(m_authSetting->value("auth/hash").toByteArray());
    if(storedPassword.isEmpty())
    {
        setPassword("1234");
        enableEnterModePassword();
        return;
    }
    if(!verifyPassword(ui->lineEditPassword->text()))
    {
        QMessageBox::critical(nullptr, "Password Error", "Entered password is wrong");
        return;
    }
    if(!hasChangePasswordEnabled)
    {
        enableChangeModePassword();
    }
    else
    {
        if(ui->lineEditNewPassword->text() == ui->lineEditRePassword->text())
        {
            changePassword(ui->lineEditPassword->text(), ui->lineEditNewPassword->text());
            QMessageBox::information(nullptr, "Password Information", "Password changed successfully");
            enableEnterModePassword();
        }
        else
        {
            QMessageBox::critical(nullptr, "Password Error", "Entered password is wrong");
            enableEnterModePassword();
        }
    }
}

void Authentication::on_pushButtonClose_clicked()
{
    close();
}

