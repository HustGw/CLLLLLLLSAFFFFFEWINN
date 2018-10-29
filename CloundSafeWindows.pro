 #-------------------------------------------------
#
# Project created by QtCreator 2018-06-26T16:02:40
#
#-------------------------------------------------

QT       += core gui
QT       += sql

#RC_FILE += icotest.rc

QT       += sql
QT       += network
LIBS += -lWs2_32

LIBS += -Loss-c-sdk/lib/Release/ -lcurl
LIBS += -Loss-c-sdk/lib/Release/ -llibapr-1
LIBS += -Loss-c-sdk/lib/Release/ -llibaprutil-1
LIBS += -Loss-c-sdk/lib/Release/ -lmxml1

#LIBS += -LF:/QTProject/CLLLLLLLSAFFFFFEWINN/oss-c-sdk/lib/Release/ -lcurl
#LIBS += -LF:/QTProject/CLLLLLLLSAFFFFFEWINN/oss-c-sdk/lib/Release/ -llibapr-1
#LIBS += -LF:/QTProject/CLLLLLLLSAFFFFFEWINN/oss-c-sdk/lib/Release/ -llibaprutil-1
#LIBS += -LF:/QTProject/CLLLLLLLSAFFFFFEWINN/oss-c-sdk/lib/Release/ -lmxml1

#LIBS += -LC:/Users/Administrator/Desktop/CLLLLLLLSAFFFFFEWINN-master/CLLLLLLLSAFFFFFEWINN/oss-c-sdk/lib/Release/ -lcurl
#LIBS += -LC:/Users/Administrator/Desktop/CLLLLLLLSAFFFFFEWINN-master/CLLLLLLLSAFFFFFEWINN/oss-c-sdk/lib/Release/ -llibapr-1
#LIBS += -LC:/Users/Administrator/Desktop/CLLLLLLLSAFFFFFEWINN-master/CLLLLLLLSAFFFFFEWINN/oss-c-sdk/lib/Release/ -llibaprutil-1
#LIBS += -LC:/Users/Administrator/Desktop/CLLLLLLLSAFFFFFEWINN-master/CLLLLLLLSAFFFFFEWINN/oss-c-sdk/lib/Release/ -lmxml1


#LIBS += -L$$quote(E:/CloundSafe/cloundsafetwentyfour/CLLLLLLLSAFFFFFEWINN-master/oss-c-sdk/lib/Release)



LIBS += -L$$quote(C:/Users/Administrator/Desktop/CLLLLLLLSAFFFFFEWINN-master/CLLLLLLLSAFFFFFEWINN/oss-c-sdk/lib/Release)

#LIBS += -L$$quote(F:/QTProject/CLLLLLLLSAFFFFFEWINN/oss-c-sdk/lib/Release)

#LIBS += -L$$quote(C:/Users/Administrator/CLLLLLLLSAFFFFFEWINN/oss-c-sdk/lib/Release)






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
    encryptfile.cpp \
    requestrecthread.cpp \
    connectionpool.cpp \
    finishdecryptionitem.cpp \
    finishencryptionitem.cpp \
    finishviewcontroller.cpp \
    finishviewcontroller2.cpp \
    senddialog.cpp\
    friendinputdlg.cpp \
    mylabel.cpp \
    informationdlg.cpp \
    informationitem.cpp \
    informationthread.cpp \
    aos_buf.c \
    aos_crc64.c \
    aos_fstack.c \
    aos_http_io.c \
    aos_log.c \
    aos_status.c \
    aos_string.c \
    aos_transport.c \
    aos_util.c \
    oss_auth.c \
    oss_bucket.c \
    oss_define.c \
    oss_live.c \
    oss_multipart.c \
    oss_object.c \
    oss_resumable.c \
    oss_util.c \
    oss_xml.c \
    downloadoss.cpp \
    uploadoss.cpp \
    encryption.cpp \
    decryptionfile.cpp \
    depdownthread.cpp \
    enitemthread.cpp \
    encryptthread.cpp \
    decryptionthread.cpp \
    registerdialog.cpp \
    tcpclient.cpp \
    delinkdialog.cpp  \
    sharedialog.cpp \
    BitBuffer.cpp \
    QrCode.cpp \
    QrSegment.cpp \
    groupsenddialog.cpp \
    msgbox.cpp \
    resetDialog.cpp \
    groupsharedialog.cpp \
    newdownloaddialog.cpp \
    decryptprobarthread.cpp \
    heartthread.cpp




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
    encryptfile.h \
    requestrecthread.h \
    connectionpool.h \
    finishdecryptionitem.h \
    finishencryptionitem.h \
    finishviewcontroller.h \
    finishviewcontroller2.h \
    senddialog.h\
    friendinputdlg.h \
    mylabel.h \
    informationdlg.h \
    informationitem.h \
    informationthread.h \
    aos_buf.h \
    aos_crc64.h \
    aos_define.h \
    aos_fstack.h \
    aos_http_io.h \
    aos_list.h \
    aos_log.h \
    aos_status.h \
    aos_string.h \
    aos_transport.h \
    aos_util.h \
    apr.h \
    apr_allocator.h \
    apr_atomic.h \
    apr_dso.h \
    apr_errno.h \
    apr_file_info.h \
    apr_file_io.h \
    apr_general.h \
    apr_global_mutex.h \
    apr_inherit.h \
    apr_md5.h \
    apr_network_io.h \
    apr_pools.h \
    apr_portable.h \
    apr_proc_mutex.h \
    apr_queue.h \
    apr_sha1.h \
    apr_shm.h \
    apr_strings.h \
    apr_tables.h \
    apr_thread_mutex.h \
    apr_thread_pool.h \
    apr_thread_proc.h \
    apr_time.h \
    apr_user.h \
    apr_want.h \
    apr_xlate.h \
    apu.h \
    curl.h \
    curlbuild.h \
    curlrules.h \
    curlver.h \
    easy.h \
    multi.h \
    mxml.h \
    mylabel.h \
    oss_api.h \
    oss_auth.h \
    oss_define.h \
    oss_resumable.h \
    oss_util.h \
    oss_xml.h \
    downloadoss.h \
    uploadoss.h \
    encryption.h \
    decryptionfile.h \
    depdownthread.h \
    enitemthread.h \
    encryptthread.h \
    decryptionthread.h \
    registerdialog.h \
    tcpclient.h \
    delinkdialog.h \
    sharedialog.h \
    BitBuffer.hpp \
    QrCode.hpp \
    QrSegment.hpp \
    groupsenddialog.h \
    msgbox.h \
    resetDialog.h \
    groupsharedialog.h \
    newdownloaddialog.h \
    decryptprobarthread.h \
    heartthread.h




FORMS += \
        mainwindow.ui \
    loginview.ui \
    senddialog.ui \
    registerdialog.ui \
    tcpclient.ui \
    sharedialog.ui \
    groupsenddialog.ui \
    resetDialog.ui \
    groupsharedialog.ui \
    newdownloaddialog.ui

DISTFILES += \
    Makefile \
    qt_zh_CN.qm \
    widgets.qm

RESOURCES += \
    pictures.qrc

include(QZXing/QZXing.pri)
