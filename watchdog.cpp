#include <watchdog.h>

#include <QFileInfo>

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
    if (!QFile::exists(program)) {
        Q_EMIT sidSendLog("Program path invalid: " + program);
        return;
    }

    if (process) {
        process->deleteLater();
    }

    process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &Watchdog::onFinished);

    connect(process, &QProcess::errorOccurred, this, [this](QProcess::ProcessError error) {
        Q_EMIT sidSendLog("QProcess error: " + QString::number(error));
    });

    process->setWorkingDirectory(QFileInfo(program).absolutePath());
    Q_EMIT sidSendLog("Starting program: " + program);
    process->start(program);

    if (!process->waitForStarted()) {
        Q_EMIT sidSendLog("Failed to start process. Error: " + process->errorString());
    }
}
