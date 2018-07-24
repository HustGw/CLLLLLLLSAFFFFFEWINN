#include "requestrecthread.h"
int RequestNum = 0;
int RequsetAllowNum = 0;
RequestRecThread::RequestRecThread(QObject *parent):QThread(parent)
{

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
    bool success = query.exec("select * from Decryption where oemp_id='"+User_ID+"'");
    if(!success){
        qDebug() << "Thread:查询user失败";
    }else{
         while(query.next()){
             num++;
         }
    }
    return num;
}


void RequestRecThread::listenReqNum(){
    RequestNum = getReqNum();
    RequsetAllowNum = ReqAllowNum();
    int num= 0;
    int allownum = 0;
    while(1){
        num = getReqNum();
        allownum = ReqAllowNum();
        if(num>RequestNum){
            emit numChanged();
            RequestNum = getReqNum();
        }
        if(allownum>RequsetAllowNum){
            emit ReqIsAlliowed();
            RequsetAllowNum = ReqAllowNum();
        }
        Sleep(2000);
    }
}

int RequestRecThread::ReqAllowNum(){
    QSqlQuery query(dd);
    int num = 0;
    bool success = query.exec("select * from Decryption where oemp_id='"+User_ID+"' and status = 3");
    if(!success){
        qDebug()<<"Thread:allow查询失败";
    }
    else{
        while(query.next()){
            num++;
    }

    }
    return num;
}


