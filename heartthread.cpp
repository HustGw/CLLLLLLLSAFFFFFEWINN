#include "heartthread.h"
heartThread::heartThread(QObject *parent):QThread(parent)
{
    m_accessManagerHeart = new QNetworkAccessManager(this);
}
void heartThread::run()
{
    work();
}
void heartThread::work()
{
    sendHeart();
}
void heartThread::sendHeart()
{
    qDebug()<<"run a time"+UserPhoneNum;
    QNetworkRequest *request = new QNetworkRequest;
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request->setUrl(QUrl("http://www.yunjiami1.com/cloud/Login/heartcheck.do"));
    QByteArray postData;
    postData.append("emp_phone=");//参数
    postData.append(UserPhoneNum);//参数
    m_accessManagerHeart->post(*request,postData);//发送http的post请求
}
