#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QMainWindow>
#include <QtNetwork>
#include <QMessageBox>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

namespace Ui {
class TcpClient;
}

class TcpClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit TcpClient(QWidget *parent = 0);
    ~TcpClient();

    int flag;  //步骤

protected:
    void init();
    void connectServer();

private slots:
    void on_sendBtn_clicked();

    void displayError(QAbstractSocket::SocketError);

    void on_signBtn_clicked();

    void readMessages();
    void finishedSlot(QNetworkReply *reply);

    void on_huoquyanzhengmaBtn_clicked();

private:
    Ui::TcpClient *ui;
    QTcpSocket *tcpSocket;
};

#endif // TCPCLIENT_H
