#include "requestrecthread.h"
int RequestNum = 0;
int erroFlag = 0;
int RequestIgnNum = 0;
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
        qDebug() << "qThread:查询user失败";
    }else{
         while(query.next()){
             num++;
         }
    }
    return num;
}


void RequestRecThread::listenReqNum(){
    RequestNum = getReqNum();//获取请求数量
    RequsetAllowNum = ReqAllowNum();//获取请求允许数量
    RequestIgnNum = ReqIgnoreNum();//获取请求忽略数量
    int num= 0;
    int allownum = 0;
    int ignoreNum = 0;
    while(1){
        num = getReqNum();
        allownum = ReqAllowNum();
        ignoreNum = ReqIgnoreNum();
        if(num>RequestNum){
            emit numChanged();
            RequestNum = getReqNum();
        }
        if(allownum>RequsetAllowNum){
            emit ReqIsAlliowed();
            RequsetAllowNum = ReqAllowNum();
        }
        if(ignoreNum>RequestIgnNum){
            emit ReqIsIgnored();
            RequestIgnNum = ReqIgnoreNum();
        }
        Sleep(2000);
    }
}

int RequestRecThread::ReqAllowNum(){
    QSqlQuery query(dd);
    int num = 0;
    bool success = query.exec("select * from Decryption where oemp_id='"+User_ID+"' and status = 3");
    if(!success){
        if (!erroFlag)
        {
            emit thread_Disconnected();
            erroFlag = 1;
        }
        qDebug()<<"Thread:allow查询失败";
    }
    else{
        while(query.next()){
            num++;
    }

    }
    return num;
}

int RequestRecThread::ReqIgnoreNum(){
    QSqlQuery query(dd);
    int num = 0;
    bool success = query.exec("select * from Decryption where oemp_id = '"+User_ID+"' and status = 4");
    if(!success){
        if(!erroFlag){
            emit thread_Disconnected();
            erroFlag = 1;
        }
        qDebug()<<"ReqThread:ignore查询失败";
    }
    else{
        while(query.next()){
            num++;
        }
    }
    return num;
}

