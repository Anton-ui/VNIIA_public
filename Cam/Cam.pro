#-------------------------------------------------
#
# Project created by QtCreator 2019-09-18T09:45:37
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Cam
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
    Sources/cameraclass.cpp \
        main.cpp \
        cam.cpp \
    Sources/xiApiPlusOcv.cpp \

HEADERS += \
    Sources/cameraclass.h \
    Sources/cv_qt_converter.h \
        cam.h \
    Sources/xiApiPlusOcv.hpp \

FORMS += \
        cam.ui

INCLUDEPATH += /usr/local/include/opencv4 \
               /opt/XIMEA/include \


DEPENDPATH  += $${INCLUDEPATH}

LIBS += -L/usr/local/lib
LIBS += -lopencv_core \
        -lopencv_videoio \
        -lopencv_objdetect \
        -lopencv_imgcodecs \
        -lopencv_imgproc \
        -lopencv_calib3d \
        -lopencv_features2d \
        -lopencv_highgui \

LIBS += -L/usr/lib -lm3api

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
