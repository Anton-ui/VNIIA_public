QT       += core gui widgets

TARGET = XimeaCamera
TEMPLATE = app

#DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += main.cpp \
        mainwindow.cpp \
        Sources/xiApiPlusOcv.cpp \
        cameraclass.cpp

HEADERS += mainwindow.h \
        Sources/xiApiPlusOcv.hpp \
        Sources/cv_qt_converter.h \
        cameraclass.h

FORMS += mainwindow.ui

INCLUDEPATH += /usr/local/include \
               /opt/XIMEA/include \


DEPENDPATH  += $${INCLUDEPATH}

LIBS += -L/usr/local/lib -lopencv_core -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc -lopencv_calib3d -lopencv_features2d -lopencv_highgui \
         -L/usr/lib -lm3api

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
