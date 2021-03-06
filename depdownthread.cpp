#include "depdownthread.h"
#include <QThread>
#include <QDebug>
QTime dpwnLoadTimer;
QTime dec_Timer;
double down_time;
double dec_time;
DepDownThread::DepDownThread(QObject *parent):QThread(parent)
{

}

void DepDownThread::run(){
    qDebug()<<"downloadThread Begin111111111111!";
    QString downPath = User_qqPath+"//Decrypt//content//"+dekey_id;
    QByteArray down_oss_Path = downPath.toLatin1();
    std::string enKeyID = dekey_id.toStdString();
    downloadoss *downKey=new downloadoss();
    //下载开始计时
    dpwnLoadTimer.start();
    downKey->OBJECT_NAME=enKeyID.c_str();
    downKey->BUCKET_NAME="cloudsafe-pc-yfile";
    downKey->download_filePath=down_oss_Path.data();
    downKey->get_object_to_file();
    down_time=dpwnLoadTimer.elapsed()/1000.0;
    qDebug()<<"depdownthread此时的fileId为："+d_id;
    emit sendTime(d_id,down_time);
    emit ChangeBtnText(d_id);

}

void DepDownThread::DownTread_RecvID(QString enkey_id,QString file_id,QString file_name){

    QString downPath = User_qqPath+"//Decrypt//ykey//"+enkey_id;
    QByteArray down_oss_Path = downPath.toLatin1();
    std::string enKeyID = enkey_id.toStdString();
    downloadoss *downKey=new downloadoss;
    downKey->OBJECT_NAME=enKeyID.c_str();
    downKey->BUCKET_NAME="cloudsafe-pc-ykey";
    downKey->download_filePath=down_oss_Path.data();
    dec_Timer.start();
    downKey->get_object_to_file();
    //下载完成后开始解密
    DecryptionFile *fileD = new DecryptionFile();
    QString contentPath = User_qqPath+"//Decrypt//content//"+file_id;
    QString filePath = User_qqPath+"//Decrypt//file//"+file_name;
   if((fileD->decryptFile(downPath,contentPath,filePath,100))==54){
        qDebug()<<"success";
        dec_time=dec_Timer.elapsed()/1000.0;
    };//解密函数
}

void DepDownThread::DownContent(QString id,QString enkey_id){
      dekey_id = enkey_id;
      qDebug()<<"depdownthread接收到的id为"+id;
      d_id = id;
}
