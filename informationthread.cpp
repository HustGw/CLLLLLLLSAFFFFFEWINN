#include "informationthread.h"
int FriendStatusNum = 0;
int Agr_fiendCount = 0;
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
    bool success = query.exec("select * from Decryption where emp_id='"+User_ID+"' and status =2 and (is_solved = '0' or is_solved = '2')");
    if(!success){
        qDebug() << "Thread:查询status =2 and (is_solved = '0' or is_solved = '2'失败";
    }else{
         while(query.next()){
             num++;
         }         
    }
    return num;
}

int InformationThread::getFriendNum(){
    QSqlQuery query(inforDB);
    int num = 0;
    bool success = query.exec("select * from friend where friend_id = '"+User_ID+"' and status ='0'");
    if(!success){
        qDebug()<<"Thread:查询好友失败";
    }
    else{
        while(query.next()){
            num++;
        }
    }
    return num;
}
int InformationThread::AgreeFriendNum(){
    QSqlQuery query(inforDB);
    int num = 0;
    bool success = query.exec("select * from friend where user_id = '"+User_ID+"' and status = '1'");
    if(!success){
        qDebug()<<"inforThread :查询好友失败";
    }
    else{
        while(query.next()){
            num++;
        }
    }
    return num;
}
void InformationThread::listenInfNum(){
    informationNum = getInfNum();
    Agr_fiendCount = AgreeFriendNum();
    FriendStatusNum = getFriendNum();
    int num = 0;
    int friendNum = 0;
    int a_friendNum = 0;
    while(1){
        num = getInfNum();
        friendNum = getFriendNum();
        a_friendNum = AgreeFriendNum();
        if(num>informationNum){
            informationNum = getInfNum();
            emit InformationChanged();
            qDebug()<<"send infor";
        }
        if(friendNum>FriendStatusNum){
            FriendStatusNum = getFriendNum();
            emit NewFriendRequest();
            qDebug()<<"new friend";
        }
        if(a_friendNum>Agr_fiendCount){
            emit NewAgreeFriend();
            Agr_fiendCount = AgreeFriendNum();
            qDebug()<<"new friend agree";
        }
        Sleep(2000);
    }
}
////槽函数 收到信号变量减一
//void InformationThread::numDown(){
//    qDebug()<<"success numDown";
//    informationNum--;
//    qDebug()<<informationNum;
//}
