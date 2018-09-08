#include "decryptionthread.h"
QString Denkey_id = NULL;
QString Dfile_id = NULL;
QString Dfile_name = NULL;
DecryptionThread::DecryptionThread(QObject *parent):QThread(parent)
{
    downKey=new downloadoss;
   // connect(downKey,SIGNAL(progressBarValue(double)),this,SLOT(RecProgressValue(double)));
}

void DecryptionThread::run(){
    QString downPath = User_qqPath+"//Decrypt//ykey//"+Denkey_id;
    QByteArray down_oss_Path = downPath.toLatin1();
    std::string enKeyID = Denkey_id.toStdString();
    downKey->OBJECT_NAME=enKeyID.c_str();
    downKey->BUCKET_NAME="cloudsafe-pc-ykey";
    downKey->download_filePath=down_oss_Path.data();
    downKey->get_object_to_file();
    //下载完成后开始解密
    DecryptionFile *fileD = new DecryptionFile();

    QString contentPath = User_qqPath+"//Decrypt//content//"+Dfile_id;
    QString filePath = User_qqPath+"//Decrypt//file//"+Dfile_name;

//    QString contentPath = "D://CloundSafeWindows//content//"+Dfile_id;
//    QString filePath = "D://CloundSafeWindows//file//"+Dfile_name;
    QString filePath_c = "C:/CloundSafe/"+User_qqNum+"/Decrypt/file/"+Dfile_name;
    for(int i=1;i<50;i++){
        QFileInfo d_file(filePath_c);
        if(d_file.isFile()){
            filePath_c = User_qqPath+"//Decrypt//file//"+Dfile_name.section(".",0,0)+"("+QString::number(i,10)+")."+Dfile_name.section(".",1,1);
            filePath = User_qqPath+"//Decrypt//file//"+Dfile_name.section(".",0,0)+"("+QString::number(i,10)+")."+Dfile_name.section(".",1,1);
        }else{
            break;
        }
    }
   if((fileD->decryptFile(downPath,contentPath,filePath))==54){
        qDebug()<<"success";
        //解密成功后删除本地密文和密钥文件
        //QFile::remove(contentPath);//删除密文
        //QFile::remove(downPath);//删除密钥
    }//解密函数
   else{
       emit decryptionFailed();
   }
}

void DecryptionThread::DecryptionThread_RecvID(QString enkey_id, QString file_id, QString file_name){
   Denkey_id = enkey_id;
   Dfile_id = file_id;
   Dfile_name = file_name;
}
void DecryptionThread::RecProgressValue(double recValue){
    qDebug()<<recValue;
}
