#include "encryptthread.h"

encryptthread::encryptthread(QObject *parent):QThread(parent)
{

}

void encryptthread::run(){
    qDebug()<<"开始加密";
    int ecptFlag;
    ecptFlag = encpt->encrypt();

    int nValue = 21;
    if (ecptFlag==1){
        while (nValue < 100)
        {
            // 休眠50毫秒
            msleep(5);
            ++nValue;
            // 准备更新
            emit result(nValue);
        }
    }
    else{
        emit result(0);
    }


}
