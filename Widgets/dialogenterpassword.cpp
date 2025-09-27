#include "dialogenterpassword.h"
#include "ui_dialogenterpassword.h"

DialogEnterPassword::DialogEnterPassword(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogEnterPassword)
{
    ui->setupUi(this);
    ui->lineEditPassword->setEchoMode(QLineEdit::Password);
}

DialogEnterPassword::~DialogEnterPassword()
{
    delete ui;
}

void DialogEnterPassword::on_buttonBox_accepted()
{
    QString entered = ui->lineEditPassword->text();

    if (validatePassword(entered)) {
        m_userValidated = true;
        accept();
    } else {
        m_userValidated = false;
        QMessageBox::warning(this, tr("Wrong password"), tr("The password you entered is incorrect."));
    }
}

bool DialogEnterPassword::validatePassword(const QString &password)
{
    // Example: compare SHA256(password) with stored hash.
    // NOTE: Replace storedHashHex with your stored hash (hex string).
    // For demo we compute the hash of "secretpass" and compare:
    const QByteArray storedHashHex = QCryptographicHash::hash(
                                         QByteArrayLiteral("1234"), QCryptographicHash::Sha256).toHex();

    QByteArray enteredHashHex = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();

    return (enteredHashHex == storedHashHex);
}

bool DialogEnterPassword::getUserValidated() const
{
    return m_userValidated;
}

