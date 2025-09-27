#ifndef DIALOGENTERPASSWORD_H
#define DIALOGENTERPASSWORD_H

#include <QDialog>
#include <QMessageBox>
#include <QCryptographicHash>
namespace Ui {
class DialogEnterPassword;
}

class DialogEnterPassword : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEnterPassword(QWidget *parent = nullptr);
    ~DialogEnterPassword();

    bool getUserValidated() const;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogEnterPassword *ui;
    bool m_userValidated = false;

    bool validatePassword(const QString &password);

};

#endif // DIALOGENTERPASSWORD_H
