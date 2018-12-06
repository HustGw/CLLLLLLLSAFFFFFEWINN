#include "startencryptthread.h"
extern QString file_item_name;
extern QFileInfo file_item_fileInfo;
startEncryptThread::startEncryptThread(QObject *parent):QThread(parent)
{
    //item = file_item_name;
}

void startEncryptThread::run(){
    qDebug()<<"开始加密";
    double fSize = file_item_fileInfo.size();
    fSize=(double)(fSize/1024.);
    fSize = (int) fSize;
    long ss = fSize/100;
    msleep(1000);
    int resultNum = 28;
    msleep(1*ss);
    emit encptResult(resultNum,item,0,0);
//    resultNum = encpt->encrypt();

        while (resultNum < 110)
        {
            // 休眠50毫秒
            if (fSize < 6000){
                //msleep(100);
                //++resultNum;
            }else{
                msleep(5*ss);
                resultNum += 5;
                // 准备更新
                //qDebug()<<"startEncryptThread"<<item;
                if(resultNum <90){
                   emit encptResult(resultNum,item,0,0);
                }else{
                    return ;
                }


            }



        }


    //emit encptResult(resultNum,item,encpt->debugTime,encpt->uploadTime);
}
