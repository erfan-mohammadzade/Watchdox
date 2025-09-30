
#include "watchdog.h"


// ------------------- CONSTRUCTOR / DESTRUCTOR -------------------
Watchdog::Watchdog(const QString &programPath, QObject *parent)
    : QObject(parent), program(programPath) {
    m_exceptionList.append("Holter.exe");
    // default exceptions
    // = {"explorer.exe", "notepad.exe"};
}

Watchdog::~Watchdog() {
    if (process) {
        process->terminate();
        process->deleteLater();
    }
}

void Watchdog::setSettingInfo(const SettingInfo &newSettingInfo) {
    m_settingInfo = newSettingInfo;
}

void Watchdog::onFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    Q_EMIT sidSendLog("Program finished with code " + QString::number(exitCode));

    if (exitStatus == QProcess::NormalExit && !m_settingInfo.hasNormalExitOpenRequested)
    {
        Q_EMIT sidSendLog("Program exited normally. Not restarting.");
    }
    else
    {
        Q_EMIT sidSendLog("Restarting program...");
        QTimer::singleShot(2000, this, &Watchdog::startProgram);
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
    QString exeName = program.split("\\").last();
    Q_EMIT sidSendLog("app name: " + exeName);
    process->start(program);

    if (m_settingInfo.hasCloseOtherAppRequested)
        killAllExceptMyself(exeName);

    if (!process->waitForStarted()) {
        Q_EMIT sidSendLog("Failed to start process. Error: " + process->errorString());
    }
}

void Watchdog::killAllExceptMyself(const QString &myExeName) {
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) return;

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnap, &pe)) {
        do {
            QString exe = QString::fromWCharArray(pe.szExeFile);

            // Skip launcher itself
            if (exe.compare(myExeName, Qt::CaseInsensitive) == 0)
                continue;

            // Skip system critical processes (add whitelist if needed)
            if (exe.compare("Watchdog.exe", Qt::CaseInsensitive) == 0)
                continue;

            HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
            if (hProc) {
                TerminateProcess(hProc, 0);
                CloseHandle(hProc);
            }

        } while (Process32Next(hSnap, &pe));
    }
    CloseHandle(hSnap);
}

// ------------------- WINDOWS ENUMERATION -------------------
QString Watchdog::getProcessName(DWORD pid) {
    QString name;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 pe;
        pe.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hSnap, &pe)) {
            do {
                if (pe.th32ProcessID == pid) {
                    name = QString::fromWCharArray(pe.szExeFile);
                    break;
                }
            } while (Process32Next(hSnap, &pe));
        }
        CloseHandle(hSnap);
    }
    return name.toLower();
}

BOOL CALLBACK Watchdog::EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);

    if (!IsWindowVisible(hwnd)) return TRUE;
    if (pid == GetCurrentProcessId()) return TRUE;

    QStringList* list = reinterpret_cast<QStringList*>(lParam);
    QString procName = Watchdog::getProcessName(pid);

    if (list->contains(procName)) return TRUE;

    PostMessage(hwnd, WM_CLOSE, 0, 0);
    return TRUE;
}

void Watchdog::closeAllWindowsExceptMine() {
    EnumWindows(Watchdog::EnumWindowsProc,
                reinterpret_cast<LPARAM>(&m_exceptionList));
}

