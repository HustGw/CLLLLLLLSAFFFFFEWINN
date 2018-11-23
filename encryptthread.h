#ifndef ENCRYPTTHREAD_H
#define ENCRYPTTHREAD_H
#include "QThread"
#include "encryption.h"
#include "QDebug"
#include "startencryptthread.h"

class encryptthread : public QThread
{
    Q_OBJECT
protected:
    void run();//执行线程函数
public:
    encryptthread();
    explicit encryptthread(QObject *parent = 0);
    encryption *encpt= new encryption();
    QString item ;
    startEncryptThread *ecry;
private slots:
    void handEncptResult(int resultNum,QString itemName,double debug_Time,double upload_Time);



signals:
    void result(int value,QString itemName,double debugTime,double uploadTime);
};

#endif // ENCRYPTTHREAD_H
