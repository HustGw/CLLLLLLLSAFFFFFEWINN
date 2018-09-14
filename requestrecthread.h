#ifndef REQUESTRECTHREAD_H
#define REQUESTRECTHREAD_H
#include <QThread>
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QtSql/QSqlError>
#include <synchapi.h>
#include <QtSql/QSqlRecord>
#include<QtSql/QSqlQuery>
#include <connectionpool.h>
#include "mainwindow.h"
extern int RequestNum;
extern int RequsetAllowNum;
extern QString Mac_address;
class RequestRecThread : public QThread
{
    Q_OBJECT
protected:
    void run();//执行线程函数

public:
    explicit RequestRecThread(QObject *parent = 0);
    QSqlDatabase dd;
    int getReqNum();//获取数据库条目数量
    void listenReqNum();//监听数据库文件数量
    int ReqAllowNum();
    int ReqIgnoreNum();
    int forceDisconnect();
signals:
    void numChanged();
    void ReqIsAlliowed();
    void thread_Disconnected();
    void ReqIsIgnored();
    void forceShutDown();
};

#endif // REQUESTRECTHREAD_H
