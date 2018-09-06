#ifndef INFORMATIONTHREAD_H
#define INFORMATIONTHREAD_H
#include <QThread>
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QtSql/QSqlError>
#include <synchapi.h>
#include <QtSql/QSqlRecord>
#include<QtSql/QSqlQuery>
#include <connectionpool.h>
#include <mainwindow.h>
extern int informationNum;
extern int FriendArrayIndex;
extern int FriendCount;//废弃变量 无作用
extern int FriendStatusNum;//记录status为0的记录数量
extern int Agr_fiendCount;
class InformationThread : public QThread
{
    Q_OBJECT
protected:
    void run();//执行线程函数
public:
    explicit InformationThread(QObject *parent = 0);
    QSqlDatabase inforDB;
    int getInfNum();//获取数据库条目数量
    void listenInfNum();//监听数据库文件数量
    void numDown();
    int getFriendNum();//获取好友数量
    int AgreeFriendNum();//获取已同意请求好友数量
signals:
    void InformationChanged();
    void NewFriendRequest();
    void NewAgreeFriend();

};

#endif // INFORMATIONTHREAD_H
