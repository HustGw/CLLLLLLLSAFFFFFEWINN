#ifndef DECRYPTIONTHREAD_H
#define DECRYPTIONTHREAD_H

#include <QThread>
#include <QDebug>
#include "downloadoss.h"
#include "decryptionfile.h"
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QTime>
#include <QDir>

extern QString Denkey_id;
extern QString Dfile_id;
extern QString Dfile_name;
extern QString User_qqPath;
extern QString User_qqNum;//用户qq_Num
class DecryptionThread : public QThread
{
    Q_OBJECT
protected:
    void run();//执行线程函数

public:
    explicit DecryptionThread(QObject *parent = 0);
    downloadoss *downKey;
    QString DItem_id;
    QString Denkey_id;
    QString Dfile_id;
    QString Dfile_name;
    QString Decrypt_num;
private slots:
    void DecryptionThread_RecvID(QString enkey_id,QString file_id,QString file_name,QString item_id);
    void RecProgressValue(double recValue);
signals:
    void decryptionFailed();
    void sendDecTime(QString fileID,double time);
};

#endif // DECRYPTIONTHREAD_H
