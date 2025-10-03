#ifndef GLOBAL_WATCHDOX_H
#define GLOBAL_WATCHDOX_H
#include <QString>
#include <QFile>
#include <QDataStream>
#define APPVERSION "1.0.1"
struct SettingInfo
{
    QString exePath;
    bool hasAutoStartEnabled = false;
    int autoStartTimer = 2000;
    bool hasCloseOtherAppRequested = false;
    bool hasNormalExitOpenRequested = true;
    bool hasGetClosePasswordRequested = true;

    void clear()
    {
        autoStartTimer = 2000;
        exePath.clear();
        hasAutoStartEnabled = false;
        hasCloseOtherAppRequested = false;
        hasNormalExitOpenRequested = true;
        hasGetClosePasswordRequested = true;
    }

    bool hasAutoStartTimerValid()
    {
        return hasAutoStartEnabled && !exePath.isEmpty() && QFile(exePath).exists();
    }
};

#endif // GLOBAL_WATCHDOX_H
