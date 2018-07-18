#-------------------------------------------------
#
# Project created by QtCreator 2018-06-26T16:02:40
#
#-------------------------------------------------

QT       += core gui
QT       += sql

QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CloundSafeWindows
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


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    loginview.cpp \
    encryptionitem.cpp \
    decryptionitem.cpp \
    encryptionbtnview.cpp \
    decryptionbtnview.cpp \
    decryptionviewcontroller.cpp \
    connectsql.cpp \
    encryptionviewcontroller.cpp \
    encryption.cpp \
    encryptfile.cpp \
    requestrecthread.cpp \
    connectionpool.cpp \
    finishdecryptionitem.cpp \
    finishencryptionitem.cpp \
    finishviewcontroller.cpp \
    finishviewcontroller2.cpp \
    friendinputdlg.cpp
    friendinputdlg.cpp

HEADERS += \
        mainwindow.h \
    loginview.h \
    encryptionitem.h \
    decryptionitem.h \
    encryptionbtnview.h \
    decryptionbtnview.h \
    decryptionviewcontroller.h \
    connectsql.h \
    encryptionviewcontroller.h \
    encryption.h \
    encryptfile.h \
    requestrecthread.h \
    connectionpool.h \
    finishdecryptionitem.h \
    finishencryptionitem.h \
    finishviewcontroller.h \
    finishviewcontroller2.h \
    friendinputdlg.h
    friendinputdlg.h

FORMS += \
        mainwindow.ui \
    loginview.ui

DISTFILES +=
