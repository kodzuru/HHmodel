#-------------------------------------------------
#
# Project created by QtCreator 2017-02-27T08:26:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = HHmodel
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
    current_plot.cpp \
    file_worker.cpp \
    help_browser.cpp \
    HH_model_class.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    current_plot.h \
    file_worker.h \
    help_browser.h \
    HH_model_class.h \
    qcustomplot.h

FORMS    += mainwindow.ui \
    current_plot.ui

RESOURCES += \
    help_browser.qrc \
    icon_about.qrc \
    icon_curr.qrc \
    icon_save.qrc

DISTFILES += \
    HHmodel.pro.user \
    076-512.png \
    about.png \
    Treetog-I-Floppy-Small.ico
