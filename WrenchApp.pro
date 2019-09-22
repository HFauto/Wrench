#-------------------------------------------------
#
# Project created by QtCreator 2019-07-05T10:28:29
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += printsupport
QT       += multimedia
QT       += multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WrenchApp
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

DEFINES += Qt_NO_CAST_FROM_ASCII

TRANSLATIONS += translation/wrenchApp_cn.ts\
               translation/wrenchApp_en.ts

INCLUDEPATH +=\
            qbytools

SOURCES += \
        camera/camera.cpp \
        code/codeinterface.cpp \
        job/job.cpp \
        jobtem/jobtem.cpp \
        languageDialog/language.cpp \
        main.cpp \
        mainwindow.cpp \
        newjob/newjob.cpp\
        personnel/personnel.cpp \
        qbyjobs/qbyjobs.cpp \
        qbytools/qbytools.cpp \
        tool/toolinterface.cpp

HEADERS += \
        camera/camera.h \
        code/codeinterface.h \
        job/job.h \
        jobtem/jobtem.h \
        languageDialog/language.h \
        mainwindow.h \
        newjob/newjob.h \
        personnel/personnel.h \
        qbyjobs/qbyjobs.h \
        qbytools/qbytools.h \
        tool/toolinterface.h

FORMS += \
        camera/camera.ui \
        code/codeinterface.ui \
        job/job.ui \
        jobtem/jobtem.ui \
        languageDialog/language.ui \
        mainwindow.ui \
        newjob/newjob.ui \
        personnel/personnel.ui \
        qbyjobs/qbyjobs.ui \
        qbytools/qbytools.ui \
        tool/toolinterface.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icon.qrc \
    translation/language.qrc
