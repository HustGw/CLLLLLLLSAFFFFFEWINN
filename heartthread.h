#ifndef HEARTTHREAD_H
#define HEARTTHREAD_H

#include <QThread>
#include <QNetworkAccessManager>
#include <QtNetwork>
#include <QNetworkRequest>
extern QString UserPhoneNum;
class heartThread:public QThread
{
    Q_OBJECT
public:
    explicit heartThread(QObject *parent = nullptr);
    QNetworkAccessManager *m_accessManagerHeart;
private:
    void run();
private slots:
    void work();
    void sendHeart();
};

#endif // HEARTTHREAD_H
