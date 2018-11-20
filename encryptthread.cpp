#include "encryptthread.h"
extern QString file_item_name;
encryptthread::encryptthread(QObject *parent):QThread(parent)
{
    item = file_item_name;
}

void encryptthread::run(){
    qDebug()<<"开始加密";
    int ecptFlag;
    ecptFlag = encpt->encrypt();

    int nValue = 21;
    if (ecptFlag==2){

         emit result(0,item,-1,-1);

    }
    else{

        while (nValue < 100)
        {
            // 休眠50毫秒
            msleep(5);
            ++nValue;
            // 准备更新
            emit result(nValue,item,encpt->debugTime,encpt->uploadTime);
        }
    }


}
