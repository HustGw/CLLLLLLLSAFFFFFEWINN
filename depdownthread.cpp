#include "depdownthread.h"
#include <QThread>
#include <QDebug>
QString dekey_id = NULL;
DepDownThread::DepDownThread(QObject *parent):QThread(parent)
{

}

void DepDownThread::run(){
    qDebug()<<"downloadThread Begin!";
    QString downPath = "D://CloundSafeWindows//content//"+dekey_id;
    QByteArray down_oss_Path = downPath.toLatin1();
    std::string enKeyID = dekey_id.toStdString();
    downloadoss *downKey=new downloadoss;
    downKey->OBJECT_NAME=enKeyID.c_str();
    downKey->BUCKET_NAME="cloudsafe-pc-yfile";
    downKey->download_filePath=down_oss_Path.data();
    downKey->get_object_to_file();
    emit ChangeBtnText(dekey_id);
}

void DepDownThread::DownTread_RecvID(QString enkey_id,QString file_id,QString file_name){

    QString downPath = "D://CloundSafeWindows//ykey//"+enkey_id;
    QByteArray down_oss_Path = downPath.toLatin1();
    std::string enKeyID = enkey_id.toStdString();
    downloadoss *downKey=new downloadoss;
    downKey->OBJECT_NAME=enKeyID.c_str();
    downKey->BUCKET_NAME="cloudsafe-pc-ykey";
    downKey->download_filePath=down_oss_Path.data();
    downKey->get_object_to_file();
    //下载完成后开始解密
    DecryptionFile *fileD = new DecryptionFile();
    QString contentPath = "D://CloundSafeWindows//content//"+file_id;
    QString filePath = "D://CloundSafeWindows//file//"+file_name;
   if((fileD->decryptFile(downPath,contentPath,filePath))==54){
        qDebug()<<"success";
    };//解密函数
}

void DepDownThread::DownContent(QString enkey_id){
      dekey_id = enkey_id;
}
