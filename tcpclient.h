#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QtNetwork>
#include <QMessageBox>
#include <QNetworkReply>
#include <QDialog>
#include <QMainWindow>
#include <QFont>
#include <QPoint>
extern QString LoginUserID;
extern QString UserPhoneNum;
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
    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;



protected:
    void init();
    void connectServer();


private slots:
    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void on_userLineEdit_textChanged();

    void on_userLineEdit_editingFinished();

    void on_passwardLineEdit_textChanged();

    void on_passwardLineEdit_editingFinished();

    void on_sendBtn_clicked();

    void on_forgetBtn_clicked();

    void on_clearBtn_clicked();

    void on_minBtn_clicked();

    void on_closeBtn_clicked();

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
