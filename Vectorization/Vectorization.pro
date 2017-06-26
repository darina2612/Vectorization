#-------------------------------------------------
#
# Project created by QtCreator 2017-06-25T22:00:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Vectorization
TEMPLATE = app

INCLUDEPATH += /usr/include/opencv
LIBS += -L/usr/lib -lopencv_core -lopencv_highgui -lopencv_imgproc

SOURCES += main.cpp\
        mainwindow.cpp \
    imageview.cpp \
    imagetranformations.cpp

HEADERS  += mainwindow.h \
    imageview.h \
    imagetranformations.h

FORMS    += mainwindow.ui
