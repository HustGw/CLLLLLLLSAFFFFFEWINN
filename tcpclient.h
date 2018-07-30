#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QtNetwork>
#include <QMessageBox>
#include <QNetworkReply>
#include <QDialog>
#include <QMainWindow>
#include <QFont>
extern QString LoginUserID;
namespace Ui {
class TcpClient;
}

class TcpClient : public QDialog
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

    void on_codeBtn_clicked();

private:
    Ui::TcpClient *ui;
    QTcpSocket *tcpSocket;
};

#endif // TCPCLIENT_H
