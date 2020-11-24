#-------------------------------------------------
#
# Project created by QtCreator 2020-02-05T16:00:30
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = IK
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
        ICamera/pcapinterface.cpp \
        ICamera/tmoderator.cpp \
        ICamera/triplebuf.cpp \
        main.cpp \
        ik.cpp \

HEADERS += \
        ICamera/defines.h \
        ICamera/pcapinterface.h \
        ICamera/tmoderator.h \
        ICamera/triplebuf.h \
        ik.h \

FORMS += \
        ik.ui

INCLUDEPATH += "C:/Program Files (x86)/WinPcap/WpdPack/Include"
INCLUDEPATH += "C:/Program Files (x86)/WinPcap/WpdPack/Lib"
LIBS += -L"C:/Program Files (x86)/WinPcap/WpdPack/Lib" -lwpcap
win32:LIBS += -lIphlpapi

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
