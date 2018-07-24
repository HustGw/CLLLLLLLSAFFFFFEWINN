#ifndef ENITEMTHREAD_H
#define ENITEMTHREAD_H
#include <QThread>

class enItemThread : public QThread
{
    Q_OBJECT
public:
    enItemThread();
    explicit enItemThread(QObject *parent = 0);

protected:
    void run() ;
signals:
    void resultReady(int value);

};

#endif // ENITEMTHREAD_H
