#-------------------------------------------------
#
# Project created by QtCreator 2020-02-26T10:07:32
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UDPtake
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
        ik_class.cpp \
        main.cpp \
        thread_buffer.cpp \
        udptake.cpp

HEADERS += \
        ik_class.h \
        thread_buffer.h \
        udptake.h

FORMS += \
        udptake.ui

INCLUDEPATH += "C:/Program Files (x86)/WinPcap/WpdPack/Include"
INCLUDEPATH += "C:/Program Files (x86)/WinPcap/WpdPack/Lib"
LIBS += -L"C:/Program Files (x86)/WinPcap/WpdPack/Lib" -lwpcap
win32:LIBS += -lIphlpapi

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
