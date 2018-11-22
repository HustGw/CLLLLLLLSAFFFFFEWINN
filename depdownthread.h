#ifndef DEPDOWNTHREAD_H
#define DEPDOWNTHREAD_H

#include <QThread>
#include <QDebug>
#include "downloadoss.h"
#include "decryptionfile.h"
#include <QMessageBox>
#include <QTime>
extern QString dekey_id;
extern QString d_id;
extern QString User_qqPath;
class DepDownThread : public QThread
{
    Q_OBJECT
protected:
    void run();//执行线程函数

public:
    explicit DepDownThread(QObject *parent = 0);
private slots:
    void DownTread_RecvID(QString enkey_id,QString file_id,QString file_name);
    void DownContent(QString id,QString enkey_id);
signals:
    void ChangeBtnText(QString fileID);
    void sendTime(QString fileID,double time);

};

#endif // DEPDOWNTHREAD_H
