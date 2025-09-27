QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Widgets/dialogenterpassword.cpp \
    main.cpp \
    mainwindow.cpp \
    Widgets/setting.cpp \
    watchdog.cpp

HEADERS += \
    Widgets/dialogenterpassword.h \
    global_watchdox.h \
    mainwindow.h \
    Widgets/setting.h \
    watchdog.h

FORMS += \
    Widgets/dialogenterpassword.ui \
    mainwindow.ui \
    Widgets/setting.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
