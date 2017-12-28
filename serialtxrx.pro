#-------------------------------------------------
#
# Project created by QtCreator 2017-06-12T11:05:17
#
#-------------------------------------------------

QT       += core gui


#繁體中文  這是以相對路徑
TRANSLATIONS+=./res/lan/ct.ts
#英文
TRANSLATIONS+=./res/lan/en.ts
#簡體中文
TRANSLATIONS+=./res/lan/cn.ts

greaterThan(QT_MAJOR_VERSION, 4) {
    QT       += widgets serialport
} else {
    include($$QTSERIALPORT_PROJECT_ROOT/src/serialport/qt4support/serialport.prf)
}

TARGET = serialtxrx
TEMPLATE = app



# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    language.cpp \
    settings.cpp \
    serial_drive.cpp \
    disp_data.cpp \
    serial_app.cpp \
    arithmetic.cpp

HEADERS  += mainwindow.h \
    language.h \
    settings.h \
    cfg.h \
    serial_drive.h \
    disp_data.h \
    serial_app.h \
    arithmetic.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc \
    image.qrc






