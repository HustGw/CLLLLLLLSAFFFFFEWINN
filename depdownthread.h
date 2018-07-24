#ifndef DEPDOWNTHREAD_H
#define DEPDOWNTHREAD_H

#include <QThread>
#include <QDebug>
#include "downloadoss.h"
#include "decryptionfile.h"
#include <QMessageBox>
extern QString dekey_id;
class DepDownThread : public QThread
{
    Q_OBJECT
protected:
    void run();//执行线程函数

public:
    explicit DepDownThread(QObject *parent = 0);
private slots:
    void DownTread_RecvID(QString enkey_id,QString file_id,QString file_name);
    void DownContent(QString enkey_id);
signals:
    void ChangeBtnText(QString fileID);

};

#endif // DEPDOWNTHREAD_H
