#-------------------------------------------------
#
# Project created by QtCreator 2019-11-27T13:42:22
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = stereo
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        stereo.cpp \
    Sources/cameraclass.cpp \
    Sources/xiApiPlusOcv.cpp

HEADERS += \
        stereo.h \
    Sources/cameraclass.h \
    Sources/cv_qt_converter.h \
    Sources/xiApiPlusOcv.hpp

FORMS += \
        stereo.ui

INCLUDEPATH += /usr/local/include \
               /opt/XIMEA/include \

LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui \
        -lopencv_calib3d \
        -lopencv_features2d -lopencv_xfeatures2d -lopencv_highgui -lopencv_flann \
        -L/usr/lib -lm3api
