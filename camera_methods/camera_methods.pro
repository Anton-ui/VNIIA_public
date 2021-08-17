#-------------------------------------------------
#
# Project created by QtCreator 2019-10-04T09:34:26
#
#-------------------------------------------------

QT       += core gui widgets printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = camera_methods
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    Sources/cameraclass.cpp \
    Sources/xiApiPlusOcv.cpp \
    Sources/qcustomplot.cpp \
    Sources/cameraclass.cpp \
    main.cpp \
    cam.cpp \
    method_xml.cpp \
    method_surf.cpp \
    method_template.cpp \
    method_akaze.cpp \
    threads_manager.cpp \
    Sources/qcustomplot.cpp

HEADERS += \
    Sources/cameraclass.h \
    Sources/cv_qt_converter.h \
    Sources/xiApiPlusOcv.hpp \
    Sources/qcustomplot.h \
    cam.h \
    method_xml.h \
    method_surf.h \
    method_template.h \
    method_akaze.h \
    threads_manager.h \

FORMS += \
        cam.ui

INCLUDEPATH += /usr/local/include/opencv4 \
               /opt/XIMEA/include \

LIBS += -L/usr/local/lib -lopencv_core -lopencv_videoio -lopencv_objdetect -lopencv_imgcodecs -lopencv_imgproc -lopencv_calib3d \
         -lopencv_features2d -lopencv_xfeatures2d -lopencv_highgui -lopencv_flann  -L/usr/lib -lm3api

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
