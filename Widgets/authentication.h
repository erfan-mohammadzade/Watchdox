#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <QWidget>
#include <QPixmap>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QSettings>
#include <QString>
#include <QByteArray>
#include <QMessageBox>
namespace Ui {
class Authentication;
}

class Authentication : public QWidget
{
    Q_OBJECT

public:
    explicit Authentication(QWidget *parent = nullptr);
    ~Authentication();
    bool verifyPassword(const QString &input);

private slots:
    void on_pushButtonEnter_clicked();

    void on_pushButtonClose_clicked();

private:
    Ui::Authentication *ui;
    QSettings *m_authSetting = nullptr;
    bool hasChangePasswordEnabled = false;

private:
    QByteArray generateSalt(int length = 16);
    QByteArray hashPassword(const QString &password, const QByteArray &salt);
    void setPassword(const QString &newPassword);
    bool changePassword(const QString &oldPass, const QString &newPass);
    void enableChangeModePassword();
    void enableEnterModePassword();
};

#endif // AUTHENTICATION_H
