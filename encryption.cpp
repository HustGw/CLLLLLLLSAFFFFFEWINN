#include "encryption.h"
#include "downloadoss.h"//测试下载
extern QString User_ID;
extern QFileInfo openFileInfo;
extern QString orfileUuid;
extern QString yzipfileUuid;
extern QString User_qqNum;
int dir_Flag = 0;
encryption::encryption()
{
    conn = ConnectionPool::openConnection();
    //创建加密目录
    //创建CloundSafe 主目录
    if (dir_Flag != 0 ){
        QString qq_Path = "C://CloundSafe//"+User_qqNum;
        QDir dir;
        dir.cd(qq_Path);  //进入某文件夹
        if(!dir.exists(qq_Path))//判断需要创建的文件夹是否存在
        {
            dir.mkpath(qq_Path); //创建文件夹
        }
        qq_Path = qq_Path+"//encrypt";
        //创建encrypt子目录
        dir.cd(qq_Path);  //进入某文件夹
        if(!dir.exists(qq_Path))//判断需要创建的文件夹是否存在
        {
            dir.mkpath(qq_Path); //创建文件夹
        }
        qq_Path = qq_Path+"//yKey";
        //创建yKey子目录
        dir.cd(qq_Path);  //进入某文件夹
        if(!dir.exists(qq_Path))//判断需要创建的文件夹是否存在
        {
            dir.mkpath(qq_Path); //创建文件夹
        }
        qq_Path = "C://CloundSafe//"+User_qqNum+"//encrypt//yZip";
        //创建yZip子目录
        dir.cd(qq_Path);  //进入某文件夹
        if(!dir.exists(qq_Path))//判断需要创建的文件夹是否存在
        {
            dir.mkpath(qq_Path); //创建文件夹
        }
        //encrypt();
    }
    dir_Flag ++;

}
void encryption::connect(){
    //连接数据库
    connectSql *con = new connectSql;
    con->setDbName("Cloud_Encryption");
    con->setPort(3306);
    con->setSIP("119.23.138.181");
    con->setSPwd("F103-backup");
    con->setUserName("root");
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");//选择要连接的数据库驱动
    db.setHostName(con->getSIP());
    db.setPort(con->getPort());
    db.setDatabaseName(con->getDbName());
    db.setUserName(con->getUserName());
    db.setPassword(con->getSPwd());
    bool ok = db.open();
    if(ok)
    {
        qDebug() << "connect MySql success!";
    }
    else // 打开数据库失败
    {

        //QMessageBox::information(this, tr("提示"), tr("MySql数据库连接失败！"), tr("确定"));
        qDebug() <<"error_MySql:\n" << db.lastError().text();
    }
}

int encryption::encrypt(){

    oss_PutKey_Flag=2;
    oss_PutFile_Flag=2;
    //drawItem(50);
    fInfo=openFileInfo;
    QTextCodec *codec = QTextCodec::codecForName("utf8");
    originalFileName = codec->fromUnicode(fInfo.fileName()).data();
    //originalFileName=fInfo.fileName();
    //originalFilePath=fInfo.filePath();
    originalFilePath = codec->fromUnicode(fInfo.filePath()).data();
    //originalFilePath = originalFilePath.toLocal8Bit().constData();
    qint64 fSize;
    fSize=fInfo.size();
    originalFileSize = (double)(fSize/1024.);
    //拆分原文件路径

    qDebug()<<originalFilePath ;
    QStringList file_part = originalFilePath.split("/");
    originalFilePath = file_part[0];
    for(int i=1;i<file_part.size();i++)
    {
        qDebug()<<file_part[i];
        originalFilePath = originalFilePath + "\\" + file_part[i];
    }
    //生成用户唯一标识
    QString userID = User_ID;
    //生成源文件的唯一标识
    //QUuid orfile_id =QUuid::createUuid();
    QString orFileID = orfileUuid;
    //加密
    //生成密文唯一标识
    QString enfile_id =QUuid::createUuid().toString();
    std::string enFileID = enfile_id.toStdString();
    //生成密钥唯一标识
    QString enkey_id = QUuid::createUuid().toString();
    std::string enKeyID = enkey_id.toStdString();

    /////////////////////////进行加密
    ///
    ///
    //设置密钥地址
    QString ykeyAbPath = "C://CloundSafe//"+User_qqNum+"//encrypt//yKey//"+ QString::fromStdString(enKeyID)+".ykey";
    //设置密文地址
    QString yzipAbPath = "C://CloundSafe//"+User_qqNum+"//encrypt//yZip//"+originalFileName;
    QString yPath = "C://CloundSafe//"+User_qqNum+"//encrypt//yZip//";

    QString uuPath = "C://CloundSafe//"+User_qqNum+"//encrypt//yZip//"+enfile_id;
    encryptfile *enfile = new encryptfile();
    //文件加密
    //enfile->encryptFile(originalFilePath ,ykeyAbPath,yzipAbPath,0,orFileID,userID);
    MsgBox *msgbox01 ;
    MsgBox *msgbox02;
    MsgBox *msgbox03;
    //QString path = fInfo.filePath();
    switch (enfile->encryptFile(originalFilePath ,ykeyAbPath,uuPath,0,originalFileName,userID)) {
    case 41:
        //QMessageBox::critical(NULL,"错误","打开源文件失败！",QMessageBox::Yes,NULL);
        msgbox01 = new MsgBox(2,QStringLiteral("打开源文件失败！"));
        msgbox01->exec();
        return 2;
        break;
    case 42:
        //QMessageBox::critical(NULL,"错误","加密失败！",QMessageBox::Yes,NULL);
        msgbox02 = new MsgBox(2,QStringLiteral("加密失败！"));
        msgbox02->exec();
        return 2;
        break;
    case 43:
        //QMessageBox::critical(NULL,"错误","加密失败！",QMessageBox::Yes,NULL);
        msgbox03 = new MsgBox(2,QStringLiteral("加密失败！"));
        msgbox03->exec();
        return 2;
        break;
    }
    //EncryptionItem *I1 = new EncryptionItem();


    //drawItem();

    QByteArray yKey_oss_Path = ykeyAbPath.toLatin1();
    QByteArray yFile_oss_Path = uuPath.toLatin1();


    //conn.close();
    QSqlQuery query(conn);
    QDateTime time = QDateTime::currentDateTime();
    QString time_str = time.toString("yyyy-MM-dd hh:mm:ss");
    qDebug()<<originalFileName;
    //密钥上传OSS
    uploadoss *upKey = new uploadoss;
    upKey->OBJECT_NAME=enKeyID.c_str();
    upKey->BUCKET_NAME="cloudsafe-pc-ykey";
    upKey->filepath=yKey_oss_Path.data();
    oss_PutKey_Flag = upKey->put_object_from_file();
    if (oss_PutKey_Flag==0){
        //QMessageBox::warning(this,"Success","申请成功请等待！",QMessageBox::Yes);
        //QMessageBox::critical(NULL,"错误","密钥上传错误",QMessageBox::Yes,NULL);
        MsgBox *msgbox = new MsgBox(2,QStringLiteral("网络连接错误！"));
        msgbox->exec();
        qDebug()<<"网络错误！";
        return 2;
    }else if (oss_PutKey_Flag==1){
        //密钥上传成功
        //将用户唯一标识、源文件名、密文密钥唯一标识存入数据库
        //上传至密钥信息表
        //将用户唯一标识、源文件名、密文密钥唯一标识存入数据库
        QString savesql_key = QString ("INSERT INTO vfile(file_id,file_uploadtime,file_name,emp_id,emp_phone)");
        savesql_key+=QString("VALUES ('"+ enkey_id +"','"+time_str+"','"+enkey_id+"','"+userID+"','"+111+"')");
        //QSqlQuery query;

        bool aok=query.exec(savesql_key);
        if(aok){
          qDebug()<<"ok";
        }
        else{
          qDebug()<<"error";
        }

        QFile file(ykeyAbPath);
        if (file.exists())
        {
            file.remove();
        }

    }else if (oss_PutKey_Flag == 3) {
        MsgBox *msgbox = new MsgBox(2,QStringLiteral("文件打开错误！"));
        msgbox->exec();
    }




    //密文上传OSS

    uploadoss *upFile = new uploadoss;
    upFile->OBJECT_NAME=enFileID.c_str();
    upFile->BUCKET_NAME="cloudsafe-pc-yfile";
    upFile->filepath=yFile_oss_Path.data();
    //upFile->filepath = codec->fromUnicode(yFile_oss_Path.data()).data();
    oss_PutFile_Flag = upFile->put_object_from_file();
    if (oss_PutFile_Flag==0){
        //QMessageBox::warning(this,"Success","申请成功请等待！",QMessageBox::Yes);
        //QMessageBox::critical(NULL,"错误","密钥上传错误",QMessageBox::Yes,NULL);
        qDebug()<<"网络错误！";
        return 2;
    }else if (oss_PutFile_Flag==1){
        //QMessageBox::warning(this,"Success","申请成功请等待！",QMessageBox::Yes);
        //QMessageBox::Ok(NULL,"错误","密文上传错误",QMessageBox::Yes,NULL);
        //密文上传成功
        //上传至密文信息表
        //将用户唯一标识、源文件名、密文密钥唯一标识存入数据库
        char article_status = '0';
        QString article_size=QString::number(originalFileSize,10,2);
        QString savesql_efile = QString ("INSERT INTO varticle(article_id,article_uploadtime,article_name,emp_id,emp_phone,article_address,article_size,article_status,key_id)");
        savesql_efile+=QString("VALUES ('"+enfile_id+"','"+time_str+"','"+fInfo.fileName()+"','"+userID+"','"+111+"','"+yzipAbPath+"','"+article_size+"',"+ article_status +",'"+enkey_id+"')");
        bool isok=query.exec(savesql_efile);
        if(isok){
          qDebug()<<"ok";
        }
        else{
          qDebug()<<"error";
        }
    }else if (oss_PutFile_Flag == 3) {
        MsgBox *msgbox = new MsgBox(2,QStringLiteral("文件打开错误！"));
        msgbox->exec();
    }

    //QFile file(yzipAbPath);
    QString newName = yzipAbPath;
    QFile yZipFile(yFile_oss_Path);
    QFile file(yzipAbPath);
    if (file.exists())
    {
        //若有重复文件
        newName =yPath+ "副本 1 "+originalFileName ;
        int count = 2;
        bool yes = yZipFile.rename(newName);
        qDebug()<<yes;
        while (!yes) {

            QString num = QString::number(count);
            newName = yPath+"副本 "+num+" "+originalFileName ;
            yes = yZipFile.rename(newName);
            count++;
        }

    }else {
        bool yes = yZipFile.rename(yzipAbPath);
        qDebug()<<yes;
    }


    ConnectionPool::closeConnection(conn);
    return 1;

    /////////////////////////////////密文下载测试
    /// 密钥下载
    /// 密文下载
//    QString downPath = "E://CloundSafe//download_test//"+enkey_id;
//    QByteArray down_oss_Path = downPath.toLatin1();

//    downloadoss *downKey=new downloadoss;
//    downKey->OBJECT_NAME=enFileID.c_str();
//    downKey->BUCKET_NAME="cloudsafe-pc-ykey";
//    downKey->download_filePath=down_oss_Path.data();
//    downKey->get_object_to_file();
}

//void encryption::drawItem(){
//    //delete encryptionViewController->layout();
//    QTextCodec *codec = QTextCodec::codecForName("GB18030");
//    QString fName,fPath;
//    qint64 fSize;
//    double mfSize;
//    fName=fInfo.fileName();
//    fPath=fInfo.filePath();
//    fSize=fInfo.size();
//    mfSize=(double)(fSize/1024.);//字节转换为KB
//    EncryptionItem *v1 = new EncryptionItem();
//    v1->fileName->setText(fName);
//    v1->fileSize->setText(QString::number( mfSize)+codec->toUnicode(" KB"));
//    v1->fileIcon->setText(fName);
//    v1->fileDescription->setText("密钥正在上传...");
//    v1->progressBar->setValue(100);
//    //EncryptionViewController *encryptionViewController = new EncryptionViewController();
//    encryptionViewController->vbox->addWidget(v1);

//    delete encryptionViewController->layout();
//    QWidget *newItemWidget = new QWidget();
//    QScrollArea *newScrollArea = new QScrollArea();
//    newItemWidget->setLayout(encryptionViewController->vbox);
//    newScrollArea->setWidget(newItemWidget);
//    QVBoxLayout *newVbox = new QVBoxLayout();
//    newVbox->addWidget(newScrollArea);
//    encryptionViewController->setLayout(newVbox);
//}
