#ifndef STARTENCRYPTTHREAD_H
#define STARTENCRYPTTHREAD_H
#include "QThread"
#include "encryption.h"
#include "QDebug"

class startEncryptThread: public QThread
{
    Q_OBJECT
protected:
    void run();//执行线程函数
public:
    startEncryptThread();
    explicit startEncryptThread(QObject *parent = 0);
    encryption *encpt= new encryption();

signals:
    void encptResult(int resultNum,double debugTime,double uploadTime);
};

#endif // STARTENCRYPTTHREAD_H
