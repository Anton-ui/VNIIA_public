QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Sources/cameraclass.cpp \
    Sources/xiApiPlusOcv.cpp \
    main.cpp \
    cam_test.cpp

HEADERS += \
    Sources/cameraclass.h \
    Sources/cv_qt_converter.h \
    Sources/xiApiPlusOcv.hpp \
    cam_test.h

FORMS += \
    cam.ui

INCLUDEPATH += /usr/local/include/opencv4 \
               /opt/XIMEA/include \

LIBS += -L/usr/local/lib
LIBS += -lopencv_core       \
        -lopencv_videoio    \
        -lopencv_objdetect  \
        -lopencv_imgcodecs  \
        -lopencv_imgproc    \
        -lopencv_calib3d    \
        -lopencv_highgui    \

LIBS += -L/usr/lib -lm3api  \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target