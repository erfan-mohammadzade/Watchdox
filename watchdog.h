#include <QCoreApplication>
#include <QProcess>
#include <QTimer>
#include <QDebug>
#include <QFile>

class Watchdog : public QObject {
    Q_OBJECT

public:
    Watchdog(const QString &programPath, QObject *parent = nullptr);
    ~Watchdog();

private slots:
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    void startProgram();

    QString program;
    QProcess *process = nullptr;
Q_SIGNALS:
    void sidSendLog(const QString& text);
};
