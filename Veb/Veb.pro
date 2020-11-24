#-------------------------------------------------
#
# Project created by QtCreator 2019-09-21T15:56:13
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Veb
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
        main.cpp \
        veb.cpp

HEADERS += \
        cv_qt_converter.h \
        veb.h

FORMS += \
        veb.ui

INCLUDEPATH +=  /usr/local/include/opencv4/
                /usr/local/include/opencv4/opencv2/

LIBS += -L/usr/local/lib
LIBS += -lopencv_core \
        -lopencv_imgcodecs \
        -lopencv_imgproc \
        -lopencv_highgui \
        -lopencv_videoio \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
