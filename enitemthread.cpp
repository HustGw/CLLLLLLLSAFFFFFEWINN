#include "enitemthread.h"
extern QString file_item_name;

enItemThread::enItemThread(QObject *parent):QThread(parent)
{
    item = file_item_name;
}

void enItemThread::run()
{
    //qDebug() << "enItem Run Thread : " << QThread::currentThreadId();
    int nValue = 0;
    while (nValue < 25)
    {
        // 休眠50毫秒
        msleep(30);
        ++nValue;

        // 准备更新
        //qDebug()<<item;
        emit resultReady(nValue,item,-1,-1);
    }
}
