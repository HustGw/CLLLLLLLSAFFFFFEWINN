#ifndef ENCRYPTTHREAD_H
#define ENCRYPTTHREAD_H
#include "QThread"
#include "encryption.h"
#include "QDebug"

class encryptthread : public QThread
{
    Q_OBJECT
protected:
    void run();//执行线程函数
public:
    encryptthread();
    explicit encryptthread(QObject *parent = 0);
    encryption *encpt= new encryption();



signals:
    void result(int value);
};

#endif // ENCRYPTTHREAD_H