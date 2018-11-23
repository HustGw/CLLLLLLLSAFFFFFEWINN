#include "decryptprobarthread.h"
extern QString DecProFileID;
extern double down_time;
DecryptProBarThread::DecryptProBarThread(QObject *parent):QThread(parent){

}

void DecryptProBarThread::run(){
    qDebug()<<"now the ID is ddd";
    qDebug()<<ItemID;

    int nValue = 21;
    while(nValue<100){
        msleep(5);
        ++nValue;
        emit reslut(nValue,ItemID);
    }

}

void DecryptProBarThread::setItemID(QString id){
    ItemID = id;
}
