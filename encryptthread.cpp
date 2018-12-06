#include "encryptthread.h"
#include "startencryptthread.h"
extern QString file_item_name;
extern QFileInfo file_item_fileInfo;

double debugTime,uploadTime;
int nValue = 26;
encryptthread::encryptthread(QObject *parent):QThread(parent)
{
    item = file_item_name;
}

void encryptthread::run(){
    encptResultFlag = 0;
    nValue = 26;
//    debugTime=0;uploadTime=0;
    qDebug()<<"开始加密";
    double fSize = file_item_fileInfo.size();
    fSize=(double)(fSize/1024.);
    fSize = (int) fSize;
    if(fSize > 6000){
        emit result(28,item,encpt->debugTime,encpt->uploadTime);
        startEncryptThread *ecry = new startEncryptThread(this);
        ecry->item = item;
        //msleep(1000);
        connect(ecry,SIGNAL(encptResult(int,QString,double,double)),this,SLOT(handEncptResult(int,QString,double,double)));
        connect(ecry, SIGNAL(finished()), ecry, SLOT(deleteLater()));
        ecry->start();
    }else{
        msleep(100);
        emit result(nValue,item,encpt->debugTime,encpt->uploadTime);
    }

    //int ecptFlag;
    encptResultFlag = encpt->encrypt();

     //msleep(1000);
//    startEncryptThread *ecry = new startEncryptThread(this);
//    connect(ecry,SIGNAL(encptResult(int,QString,double,double)),this,SLOT(handEncptResult(int,QString,double,double)));
//    connect(ecry, SIGNAL(finished()), ecry, SLOT(deleteLater()));
//    ecry->start();
    int num;

    if (encptResultFlag == 2){

            emit result(0,item,-1,-1);

       }
       else if (encptResultFlag == 1){
        if(nValue < 100){
            while (nValue < 100)
            {
                // 休眠50毫秒
                msleep(50);
                nValue += 5;
                // 准备更新
                //qDebug()<<"RRR"<<nValue<<item;
                emit result(nValue,item,encpt->debugTime,encpt->uploadTime);

            }
        }else{
            //qDebug()<<"MMM"<<nValue<<item;
            emit result(nValue,item,encpt->debugTime,encpt->uploadTime);
        }

       }
//    if (ecptFlag==2){

//         emit result(0,item,-1,-1);

//    }
//    else{

//        double fSize = file_item_fileInfo.size();
//        fSize=(double)(fSize/1024.);
//        fSize = (int) fSize;
//        long ss = fSize/100;
//        while (nValue < 300)
//        {
//            if (encptResultFlag==2){

//               break;
//            }else if (encptResultFlag == 1){
//                nValue = 100;
//                emit result(nValue,item,debugTime,uploadTime);
//                break;
//            }else{
//                //double fSize = file_item_fileInfo.size();
//                //fSize=(double)(fSize/1024.);
//                // 休眠50毫秒
//                if (fSize < 6000){
//                    msleep(50);
//                }else{
//                    msleep(2*ss);
//                }

//                ++nValue;
//                // 准备更新

//                if(nValue == 90 && encptResultFlag != 1){
//                    emit result(90,item,-1, -1);
//                }else {
//                    emit result(nValue,item,-1,-1);
//                }
//            }

//        }
//    }
 /*   qDebug()<<"开始加密";
//        int ecptFlag;
//        ecptFlag = encpt->encrypt();

//        int nValue = 26;
//        if (ecptFlag==2){

//             emit result(0,item,-1,-1);

//        }
//        else{

//            while (nValue < 100)
//            {
//                // 休眠50毫秒
//                msleep(5);
//                ++nValue;
//                // 准备更新
//                emit result(nValue,item,encpt->debugTime,encpt->uploadTime);
//            }
//        }

    //encptResultFlag = 0;
    int ecptFlag;
    nValue = 26;
//    debugTime=0;uploadTime=0;
    qDebug()<<"开始加密";
    double fSize = file_item_fileInfo.size();
    fSize=(double)(fSize/1024.);
    fSize = (int) fSize;
    if(fSize > 6000){
        startEncryptThread *ecry = new startEncryptThread(this);
        ecry->item = item;
        msleep(1000);
        connect(ecry,SIGNAL(encptResult(int,QString,double,double)),this,SLOT(handEncptResult(int,QString,double,double)));
        connect(ecry, SIGNAL(finished()), ecry, SLOT(deleteLater()));
        ecry->start();
        encptResultFlag = encpt->encrypt();
    }else{

        ecptFlag = encpt->encrypt();
    }

    //int ecptFlag;


     //msleep(1000);
//    startEncryptThread *ecry = new startEncryptThread(this);
//    connect(ecry,SIGNAL(encptResult(int,QString,double,double)),this,SLOT(handEncptResult(int,QString,double,double)));
//    connect(ecry, SIGNAL(finished()), ecry, SLOT(deleteLater()));
//    ecry->start();
    int num;

    if (encptResultFlag == 2){

            emit result(0,item,-1,-1);

       }
       else {

           while (nValue < 100)
           {
               // 休眠50毫秒
               msleep(50);
               nValue+=5;
               // 准备更新
               qDebug()<<"RRR"<<nValue<<item;
               emit result(nValue,item,encpt->debugTime,encpt->uploadTime);

           }
       }
//    if (ecptFlag==2){

//         emit result(0,item,-1,-1);

//    }
//    else{

//        double fSize = file_item_fileInfo.size();
//        fSize=(double)(fSize/1024.);
//        fSize = (int) fSize;
//        long ss = fSize/100;
//        while (nValue < 300)
//        {
//            if (encptResultFlag==2){

//               break;
//            }else if (encptResultFlag == 1){
//                nValue = 100;
//                emit result(nValue,item,debugTime,uploadTime);
//                break;
//            }else{
//                //double fSize = file_item_fileInfo.size();
//                //fSize=(double)(fSize/1024.);
//                // 休眠50毫秒
//                if (fSize < 6000){
//                    msleep(50);
//                }else{
//                    msleep(2*ss);
//                }

//                ++nValue;
//                // 准备更新

//                if(nValue == 90 && encptResultFlag != 1){
//                    emit result(90,item,-1, -1);
//                }else {
//                    emit result(nValue,item,-1,-1);
//                }
//            }

//        }
//    }
*/
}
void encryptthread::handEncptResult(int resultNum,QString itemName,double debug_Time,double upload_Time){

    //    if (resultNum == 2){
//        encptResultFlag = 2;
//    }
//    if (resultNum == 1){
//        encptResultFlag = 1;
//        debugTime = debug_Time;
//        uploadTime = upload_Time;
//    }
    if (encptResultFlag == 2){

    }else if(encptResultFlag == 1){
//        resultNum = 100;
//        nValue = resultNum;
//        emit result(nValue,item,0,0);
        //qDebug()<<"加密成功"<<itemName;
    }else if (nValue <= 90 && nValue >= 26){
        nValue = resultNum;
        //qDebug()<<"WWW"<<nValue<<itemName;
        emit result(nValue,itemName,0,0);

    }

    //item = itemName;

}
