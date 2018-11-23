#include "startencryptthread.h"
extern QString file_item_name;
extern QFileInfo file_item_fileInfo;
startEncryptThread::startEncryptThread(QObject *parent):QThread(parent)
{
    //item = file_item_name;
}

void startEncryptThread::run(){
    qDebug()<<"开始加密";
    msleep(1000);
    int resultNum = 26;
//    resultNum = encpt->encrypt();
    double fSize = file_item_fileInfo.size();
        fSize=(double)(fSize/1024.);
        fSize = (int) fSize;
        long ss = fSize/100;
        while (resultNum < 300)
        {
            // 休眠50毫秒
            if (fSize < 6000){
                msleep(50);
                ++resultNum;
            }else{
                msleep(5*ss);
                resultNum += 5;
            }


            // 准备更新

           emit encptResult(resultNum,item,0,0);
        }


    //emit encptResult(resultNum,item,encpt->debugTime,encpt->uploadTime);
}
