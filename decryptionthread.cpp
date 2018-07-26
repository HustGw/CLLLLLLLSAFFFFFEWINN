#include "decryptionthread.h"
QString Denkey_id = NULL;
QString Dfile_id = NULL;
QString Dfile_name = NULL;
DecryptionThread::DecryptionThread(QObject *parent):QThread(parent)
{

}


void DecryptionThread::run(){
    QString downPath = "D://CloundSafeWindows//ykey//"+Denkey_id;
    QByteArray down_oss_Path = downPath.toLatin1();
    std::string enKeyID = Denkey_id.toStdString();
    downloadoss *downKey=new downloadoss;
    downKey->OBJECT_NAME=enKeyID.c_str();
    downKey->BUCKET_NAME="cloudsafe-pc-ykey";
    downKey->download_filePath=down_oss_Path.data();
    downKey->get_object_to_file();
    //下载完成后开始解密
    DecryptionFile *fileD = new DecryptionFile();
    QString contentPath = "D://CloundSafeWindows//content//"+Dfile_id;
    QString filePath = "D://CloundSafeWindows//file//"+Dfile_name;
   if((fileD->decryptFile(downPath,contentPath,filePath))==54){
        qDebug()<<"success";
    };//解密函数
}

void DecryptionThread::DecryptionThread_RecvID(QString enkey_id, QString file_id, QString file_name){
   Denkey_id = enkey_id;
   Dfile_id = file_id;
   Dfile_name = file_name;
}
