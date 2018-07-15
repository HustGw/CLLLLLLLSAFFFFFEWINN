#include "requestrecthread.h"
int RequestNum = 0;
QString user_ID = "123";
RequestRecThread::RequestRecThread(QObject *parent):QThread(parent)
{
//    QSqlDatabase database = QSqlDatabase::addDatabase("QMYSQL","B");
//           database.setHostName("119.23.138.181");
//           //database.setPort(3306);
//           database.setDatabaseName("Cloud_Encryption");
//           database.setUserName("root");
//           database.setPassword("F103-backup");
//           bool ok = database.open();
//        if(ok)
//        {
//            qDebug() << "Thread:connect MySql success!";
//        }
//        else // 打开数据库失败
//        {
//            qDebug() <<"error_MySql:\n" << database.lastError().text();
//        }
}

void RequestRecThread::run(){
    dd = ConnectionPool::openConnection();
    qDebug()<<"the Thread begin!";
    listenReqNum();
}

int RequestRecThread::getReqNum(){
    //查询数据库
    QSqlQuery query(dd);
    int num=0;
    bool success = query.exec("select * from Decryption where oemp_id="+user_ID+"");
    if(!success){
        qDebug() << "Thread:查询user失败";
    }else{
         qDebug()<<"Thread:查询成功！";
         while(query.next()){
             num++;
         }
    }
    return num;
}


void RequestRecThread::listenReqNum(){
    RequestNum = getReqNum();
    int num= 0;
    while(1){
        num = getReqNum();
        if(num!=RequestNum){
            emit numChanged();
            RequestNum = getReqNum();
        }
        else{
           Sleep(1000);
        }

    }
}


