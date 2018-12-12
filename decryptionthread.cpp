#include "decryptionthread.h"
QTime Dec_timer;
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
    Dec_timer.start();
    QString contentPath = User_qqPath+"//Decrypt//content//"+Dfile_id;
    QString filePath = User_qqPath+"//Decrypt//file//"+Dfile_name;

//    QString contentPath = "D://CloundSafeWindows//content//"+Dfile_id;
//    QString filePath = "D://CloundSafeWindows//file//"+Dfile_name;
    QString filePath_c = "C:/CloundSafe/"+User_qqNum+"/Decrypt/file/"+Dfile_name;
    QString temp_Path = "C://CloundSafe//"+User_qqNum+"//temp";
    //创建yZip子目录
    QDir dir;
    dir.cd(temp_Path);  //进入某文件夹
    if(!dir.exists(temp_Path))//判断需要创建的文件夹是否存在
    {
        dir.mkpath(temp_Path); //创建文件夹
    }
    for(int i=1;i<50;i++){
        QFileInfo d_file(filePath_c);
        if(d_file.isFile()){
            filePath_c = User_qqPath+"//Decrypt//file//"+Dfile_name.section(".",0,0)+"("+QString::number(i,10)+")."+Dfile_name.section(".",1,1);
            filePath = User_qqPath+"//Decrypt//file//"+Dfile_name.section(".",0,0)+"("+QString::number(i,10)+")."+Dfile_name.section(".",1,1);
        }else{
            break;
        }
    }
    int file_decrypt_num = Decrypt_num.toInt();
    qDebug()<<"此时的解密Num为："+file_decrypt_num;
   if((fileD->decryptFile(downPath,contentPath,filePath,file_decrypt_num))==54){
        double dectime = Dec_timer.elapsed()/1000.0;
        qDebug()<<"当前的fileUUID为："+DItem_id;
        qDebug()<<"当前的fileID:"+Dfile_id;
        emit sendDecTime(DItem_id,dectime);
        //解密成功后删除本地密文和密钥文件
        //QFile::remove(contentPath);//删除密文
        //QFile::remove(downPath);//删除密钥
    }//解密函数
   else{
       emit decryptionFailed();
   }
}

void DecryptionThread::DecryptionThread_RecvID(QString enkey_id, QString file_id, QString file_name,QString item_id){
   Denkey_id = enkey_id;
   Dfile_id = file_id;
   Dfile_name = file_name;
   DItem_id = item_id;
}
void DecryptionThread::RecProgressValue(double recValue){
    qDebug()<<recValue;
}
