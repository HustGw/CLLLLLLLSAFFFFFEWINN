#include "enitemthread.h"

enItemThread::enItemThread(QObject *parent):QThread(parent)
{

}

void enItemThread::run()
{
    //qDebug() << "enItem Run Thread : " << QThread::currentThreadId();
    int nValue = 0;
    while (nValue < 20)
    {
        // 休眠50毫秒
        msleep(5);
        ++nValue;

        // 准备更新
        emit resultReady(nValue);
    }
}
