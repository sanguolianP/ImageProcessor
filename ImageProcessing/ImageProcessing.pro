#-------------------------------------------------
#
# Project created by QtCreator 2016-06-15T08:45:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageProcessing
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dlgspatialfilter.cpp \
    dlgtransform.cpp \
    ratioc.cpp \
    ratiome.cpp \
    randintensity.cpp \
    threshold.cpp \
    mypixmapitem.cpp \
    imageprocesser.cpp \
    sixdim.cpp \
    matrix.cpp

HEADERS  += mainwindow.h \
    dlgspatialfilter.h \
    dlgtransform.h \
    ratioc.h \
    ratiome.h \
    randintensity.h \
    threshold.h \
    mypixmapitem.h \
    imageprocesser.h \
    sixdim.h \
    matrix.h

FORMS    += mainwindow.ui \
    dlgspatialfilter.ui \
    dlgtransform.ui \
    ratioc.ui \
    ratiome.ui \
    randintensity.ui \
    threshold.ui
