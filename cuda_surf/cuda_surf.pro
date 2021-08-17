#-------------------------------------------------
#
# Project created by QtCreator 2019-12-04T16:07:20
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = cuda_surf
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        cudasurf.cpp \
        Sources/cameraclass.cpp \
        Sources/xiApiPlusOcv.cpp \
    cuda_surf.cpp

HEADERS += \
        cudasurf.h \
        Sources/cameraclass.h \
        Sources/cv_qt_converter.h \
        Sources/xiApiPlusOcv.hpp \
    cuda_surf.h

FORMS += \
        cudasurf.ui

INCLUDEPATH += /usr/local/include \
               /opt/XIMEA/include \

LIBS += -L/usr/local/lib -lopencv_calib3d -lopencv_cudafeatures2d -lopencv_xfeatures2d -lopencv_flann -lopencv_imgproc -lopencv_core \
        -lopencv_imgcodecs -lopencv_highgui -lopencv_cudaimgproc -lopencv_objdetect -lopencv_cudaobjdetect \
        -L/usr/lib -lm3api
