#include "informationthread.h"
//int informationNum = 0;
InformationThread::InformationThread(QObject *parent):QThread(parent)
{

}

void InformationThread::run(){
    inforDB = ConnectionPool::openConnection();
    qDebug()<<"inforThreadBegin";
    listenInfNum();

}

int InformationThread::getInfNum(){
    QSqlQuery query(inforDB);
    int num=0;
    bool success = query.exec("select * from Decryption where emp_id='"+User_ID+"' and status =2");
    if(!success){
        qDebug() << "Thread:查询user失败";
    }else{
         while(query.next()){
             num++;
         }         
    }
    return num;
}

void InformationThread::listenInfNum(){
    informationNum = getInfNum();
    int num = 0;
    while(1){
        num = getInfNum();
        if(num>informationNum){
            emit InformationChanged();
            qDebug()<<"send infor";
            informationNum = getInfNum();
        }
        else{
            Sleep(2000);
        }
    }
}
////槽函数 收到信号变量减一
//void InformationThread::numDown(){
//    qDebug()<<"success numDown";
//    informationNum--;
//    qDebug()<<informationNum;
//}
