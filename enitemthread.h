#ifndef ENITEMTHREAD_H
#define ENITEMTHREAD_H
#include <QThread>
#include <QDebug>

class enItemThread : public QThread
{
    Q_OBJECT
public:
    enItemThread();
    explicit enItemThread(QObject *parent = 0);
    QString item;

protected:
    void run() ;
signals:
    void resultReady(int value,QString itemName,double debugTime,double uploadTime);

};

#endif // ENITEMTHREAD_H
