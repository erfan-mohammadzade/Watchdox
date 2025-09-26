#include <watchdog.h>

Watchdog::Watchdog(const QString &programPath, QObject *parent)
    : QObject(parent), program(programPath) {
    startProgram();
}

Watchdog::~Watchdog()
{
    process->terminate();
    process->deleteLater();
}

void Watchdog::onFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    Q_EMIT sidSendLog("Program finished with code" + QString::number(exitCode));

    // Restart if crashed or abnormal exit
    if (exitStatus == QProcess::CrashExit || exitCode != 0) {
        Q_EMIT sidSendLog("Restarting program...");
        QTimer::singleShot(2000, this, &Watchdog::startProgram); // restart after 2s
    } else {
        Q_EMIT sidSendLog("Program exited normally. Not restarting.");
        QCoreApplication::quit();
    }
}

void Watchdog::startProgram() {
    if (process) {
        process->deleteLater();
    }
    process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &Watchdog::onFinished);
    process->start(program);
}
