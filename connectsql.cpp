#include "mainwindow.h"
#include <connectsql.h>

#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QtSql/QSqlError>

//class connectSql{
//private:
//    QString SIP;
//    QString DbName;
//    int Port;
//    QString UserName;
//    QString SPwd;


//public:
     //connectSql consql = new connectSql;
    void connectSql::setSIP(QString sip)  {
        //consql.SIP=SIP;
        this->SIP=sip;
    }
    void connectSql::setDbName(QString dbname){
        this->DbName=dbname;
    }
    void connectSql::setPort(int port){
       this->Port=port;
    }
    void connectSql::setUserName(QString username){
        this->UserName=username;
    }
    void connectSql::setSPwd(QString spwd){
        this->SPwd=spwd;
    }

    QString connectSql::getSIP(){
        //consql.SIP=SIP;
        return this->SIP;
    }
    QString connectSql::getDbName(){
       return this->DbName;
    }
    int connectSql::getPort(){
       return this->Port;
    }
    QString connectSql::getUserName(){
        return this->UserName;
    }
    QString connectSql::getSPwd(){
        return  this->SPwd;
    }
 //   connectSql *con = new connectSql();
//};

//
//void encryption()
//{
//    connectSql *con = new connectSql();
//    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");//选择要连接的数据库驱动
//    db.setHostName(con->getSIP());
//    db.setPort(con->getPort());
//    db.setDatabaseName(con->getDbName());
//    db.setUserName(con->getUserName());
//    db.setPassword(con->getSPwd());
//    bool ok = db.open();
//    if(ok)
//        {
//            qDebug() << "connect MySql success!";

//        }
//        else // 打开数据库失败
//        {

//            //QMessageBox::information(this, tr("提示"), tr("MySql数据库连接失败！"), tr("确定"));
//            qDebug() <<"error_MySql:\n" << db.lastError().text();
//        }
//}
