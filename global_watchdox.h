#ifndef GLOBAL_WATCHDOX_H
#define GLOBAL_WATCHDOX_H
#include <QString>
#include <QFile>
struct SettingInfo
{
    QString exePath;
    bool hasAutoStartEnabled = false;
    int autoStartTimer = 2000;
    void clear()
    {
        autoStartTimer = 2000;
        exePath.clear();
        hasAutoStartEnabled = false;
    }

    bool hasAutoStartTimerValid()
    {
        return hasAutoStartEnabled && !exePath.isEmpty() && QFile(exePath).exists();
    }

    friend QDataStream &operator<<(QDataStream &out, const SettingInfo &myStruct) {
        out << myStruct.exePath << myStruct.hasAutoStartEnabled << myStruct.autoStartTimer;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, SettingInfo &myStruct) {
        in >> myStruct.exePath>> myStruct.hasAutoStartEnabled >> myStruct.autoStartTimer;
        return in;
    }
};

#endif // GLOBAL_WATCHDOX_H
