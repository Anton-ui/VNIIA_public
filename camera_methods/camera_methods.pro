#-------------------------------------------------
#
# Project created by QtCreator 2019-10-04T09:34:26
#
#-------------------------------------------------

QT       += core gui widgets printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = camera_methods
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
    Sources/cameraclass.cpp \
    Sources/xiApiPlusOcv.cpp \
    Sources/qcustomplot.cpp \
    main.cpp \
    cam.cpp \
    method_xml.cpp \
    method_surf.cpp \
    method_template.cpp \
    method_akaze.cpp \
    threads_manager.cpp \

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

LIBS += -L/usr/local/lib
LIBS += -lopencv_core        \
        -lopencv_videoio     \
        -lopencv_objdetect   \
        -lopencv_imgcodecs   \
        -lopencv_imgproc     \
        -lopencv_calib3d     \
        -lopencv_features2d  \
        -lopencv_xfeatures2d \
        -lopencv_highgui     \
        -lopencv_flann       \

LIBS += -L/usr/lib -lm3api

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
