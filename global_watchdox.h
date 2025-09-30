#ifndef GLOBAL_WATCHDOX_H
#define GLOBAL_WATCHDOX_H
#include <QString>
#include <QFile>
#include <QDataStream>
struct SettingInfo
{
    QString exePath;
    bool hasAutoStartEnabled = false;
    int autoStartTimer = 2000;
    bool hasCloseOtherAppRequested = false;
    bool hasNormalExitOpenRequested = true;

    void clear()
    {
        autoStartTimer = 2000;
        exePath.clear();
        hasAutoStartEnabled = false;
        hasCloseOtherAppRequested = false;
        hasNormalExitOpenRequested = true;
    }

    bool hasAutoStartTimerValid()
    {
        return hasAutoStartEnabled && !exePath.isEmpty() && QFile(exePath).exists();
    }

    friend QDataStream &operator<<(QDataStream &out, const SettingInfo &myStruct) {
        out << myStruct.exePath
            << myStruct.hasAutoStartEnabled
            << myStruct.autoStartTimer
            << myStruct.hasCloseOtherAppRequested
            << myStruct.hasNormalExitOpenRequested;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, SettingInfo &myStruct) {
        in >> myStruct.exePath>>
            myStruct.hasAutoStartEnabled >>
            myStruct.autoStartTimer >>
            myStruct.hasCloseOtherAppRequested >>
            myStruct.hasNormalExitOpenRequested;
        return in;
    }
};

#endif // GLOBAL_WATCHDOX_H
