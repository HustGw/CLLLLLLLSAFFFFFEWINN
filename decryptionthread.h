#ifndef DECRYPTIONTHREAD_H
#define DECRYPTIONTHREAD_H

#include <QThread>
#include <QDebug>
#include "downloadoss.h"
#include "decryptionfile.h"
#include <QMessageBox>
extern QString Denkey_id;
extern QString Dfile_id;
extern QString Dfile_name;

class DecryptionThread : public QThread
{
    Q_OBJECT
protected:
    void run();//执行线程函数

public:
    explicit DecryptionThread(QObject *parent = 0);
private slots:
    void DecryptionThread_RecvID(QString enkey_id,QString file_id,QString file_name);
signals:
    void decryptionFailed();
};

#endif // DECRYPTIONTHREAD_H
