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
signals:
    void InformationChanged();

};

#endif // INFORMATIONTHREAD_H