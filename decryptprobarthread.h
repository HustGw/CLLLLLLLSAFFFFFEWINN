#ifndef DECRYPTPROBARTHREAD_H
#define DECRYPTPROBARTHREAD_H
#include <QThread>
#include <QDebug>

class DecryptProBarThread : public QThread
{
    Q_OBJECT
protected:
public:
    DecryptProBarThread();
    explicit DecryptProBarThread(QObject *parent = 0);
    QString ItemID;
private slots:
    void setItemID(QString id);
    void run();//执行线程函数
signals:
    void reslut(int value,QString itemID);
};

#endif // DECRYPTPROBARTHREAD_H
