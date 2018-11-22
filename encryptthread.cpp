#include "encryptthread.h"
#include "startencryptthread.h"
extern QString file_item_name;
extern QFileInfo file_item_fileInfo;
int encptResultFlag ;
double debugTime,uploadTime;
encryptthread::encryptthread(QObject *parent):QThread(parent)
{
    item = file_item_name;
}

void encryptthread::run(){
    encptResultFlag = 0;
    debugTime=0;uploadTime=0;
    qDebug()<<"开始加密";
//    int ecptFlag;
//    ecptFlag = encpt->encrypt();

     msleep(1000);
    startEncryptThread *ecry = new startEncryptThread(this);
    connect(ecry,SIGNAL(encptResult(int,double,double)),this,SLOT(handEncptResult(int,double,double)));
    ecry->start();
    int num;
    int nValue = 26;
//    if (ecptFlag==2){

//         emit result(0,item,-1,-1);

//    }
//    else{

        double fSize = file_item_fileInfo.size();
        fSize=(double)(fSize/1024.);
        fSize = (int) fSize;
        long ss = fSize/100;
        while (nValue < 300)
        {
            if (encptResultFlag==2){

               break;
            }else if (encptResultFlag == 1){
                nValue = 100;
                emit result(nValue,item,debugTime,uploadTime);
                break;
            }else{
                //double fSize = file_item_fileInfo.size();
                //fSize=(double)(fSize/1024.);
                // 休眠50毫秒
                if (fSize < 6000){
                    msleep(50);
                }else{
                    msleep(2*ss);
                }

                ++nValue;
                // 准备更新

                if(nValue == 90 && encptResultFlag != 1){
                    emit result(90,item,-1, -1);
                }else {
                    emit result(nValue,item,-1,-1);
                }
            }

        }
//    }


}
void encryptthread::handEncptResult(int resultNum,double debug_Time,double upload_Time){
    if (resultNum == 2){
        encptResultFlag = 2;
    }
    if (resultNum == 1){
        encptResultFlag = 1;
        debugTime = debug_Time;
        uploadTime = upload_Time;
    }
}
