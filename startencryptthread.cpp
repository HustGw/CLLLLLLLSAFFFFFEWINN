#include "startencryptthread.h"

startEncryptThread::startEncryptThread(QObject *parent):QThread(parent)
{

}

void startEncryptThread::run(){
    qDebug()<<"开始加密";
    int resultNum;
    resultNum = encpt->encrypt();

    emit encptResult(resultNum,encpt->debugTime,encpt->uploadTime);
}
