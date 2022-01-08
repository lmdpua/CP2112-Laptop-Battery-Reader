QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BatteryCommands.cpp \
    cp2112.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    BatteryCommands.h \
    SLABCP2112.h \
    SMBusConfig.h \
    cp2112.h \
    mainwindow.h \
    types.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/./ -lSLABHIDtoSMBus

INCLUDEPATH += $$PWD/./
DEPENDPATH += $$PWD/./

DISTFILES += \
    SLABHIDDevice.dll \
    SLABHIDtoSMBus.dll \
    SLABHIDtoSMBus.lib

unix:!macx: LIBS += -L$$PWD/./ -lslabhidtosmbus

INCLUDEPATH += $$PWD/./
DEPENDPATH += $$PWD/./

unix:!macx: LIBS += -L$$PWD/./ -lslabhidtosmbus

INCLUDEPATH += $$PWD/./
DEPENDPATH += $$PWD/./

QMAKE_LIBDIR += $$PWD/./
