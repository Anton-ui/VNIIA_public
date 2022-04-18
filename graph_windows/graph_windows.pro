QT       += core gui widgets printsupport

CONFIG += c++11

SOURCES += \
    main.cpp \
    graph_windows.cpp \
    qcustomplot.cpp

HEADERS += \
    graph_windows.h \
    qcustomplot.h

FORMS += \
    graph_windows.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
