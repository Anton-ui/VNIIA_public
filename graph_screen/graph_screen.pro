QT += core gui widgets printsupport

CONFIG += c++11

SOURCES += \
    main.cpp \
    graph_screen.cpp \
    qcustomplot.cpp

HEADERS += \
    graph_screen.h \
    qcustomplot.h

FORMS += \
    graph_screen.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
