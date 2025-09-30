#include <QCoreApplication>
#include <QProcess>
#include <QTimer>
#include <QDebug>
#include <QFile>
#include <global_watchdox.h>
#include <windows.h>
#include <tlhelp32.h>
#include <QStringList>
#include <QFileInfo>
#include <QTimer>

class Watchdog : public QObject {
    Q_OBJECT

public:
    Watchdog(const QString &programPath, QObject *parent = nullptr);
    ~Watchdog();

    void setSettingInfo(const SettingInfo &newSettingInfo);

private slots:
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);

public:
    void startProgram();

private:
    // void killAllAppsExceptMine();  // (Windows only version now)
    void killAllExceptMyself(const QString &myExeName);
    void closeAllWindowsExceptMine();

    static QString getProcessName(DWORD pid);
    static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM);

    QString program;
    QProcess *process = nullptr;
    SettingInfo m_settingInfo;
    QStringList m_exceptionList;  // <--- moved into class

Q_SIGNALS:
    void sidSendLog(const QString& text);
};
