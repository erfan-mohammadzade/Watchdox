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
    m_password = ui->lineEditPassword->text();
}

QString DialogEnterPassword::password() const
{
    return m_password;
}




