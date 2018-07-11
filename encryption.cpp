#include "encryption.h"
#include <connectsql.h>
#include <encryptfile.h>
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QtSql/QSqlError>
#include <QUuid>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>


encryption::encryption()
{

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

void encryption::encrypt(){
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
    QUuid user_id =QUuid::createUuid();
    QString userID = user_id.toString();
    //生成源文件的唯一标识
    QUuid orfile_id =QUuid::createUuid();
    QString orFileID = orfile_id.toString();
    //加密
    //生成密文唯一标识
    QUuid enfile_id =QUuid::createUuid();
    QString enFileID = enfile_id.toString();
    //生成密钥唯一标识
    QUuid enkey_id = QUuid::createUuid();
    QString enKeyID = enkey_id.toString();
    /////////////////////////进行加密
    ///
    ///
    //设置密钥地址
    QString ykeyAbPath = "E://CloundSafe//ykey//"+enKeyID;
    //设置密文地址
    QString yzipAbPath = "E://CloundSafe//yzip//"+enFileID;
    encryptfile *enfile = new encryptfile();
    enfile->encryptFile(originalFilePath ,ykeyAbPath,yzipAbPath,0,orFileID,userID);



    //上传至密钥信息表
    //将用户唯一标识、源文件名、密文密钥唯一标识存入数据库
    QSqlQuery query;
    query.exec("select * from employee");
    query.seek(1);
    QSqlRecord record=query.record();
    QString phone=record.value("emp_phone").toString();
    qDebug()<<"phone"<<phone;
}
