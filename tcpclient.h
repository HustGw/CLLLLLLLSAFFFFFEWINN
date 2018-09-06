#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QtNetwork>
#include <QMessageBox>
#include <QNetworkReply>
#include <QDialog>
#include <QMainWindow>
#include <QFont>
#include <QPoint>
#include <QMouseEvent>
#include <QtMath>
#include <QPainter>
#include <QSettings>

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
    bool remeberPasswd;
    QString usersname;
    QString passwd;
    void savecfg();


protected:
    void init();
    void connectServer();
    QPoint move_point;                                    //移动的距离
    bool mouse_press;                                    //鼠标按下
    void mousePressEvent(QMouseEvent *qevent);            //鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *qevent);         //鼠标释放事件
    void mouseMoveEvent(QMouseEvent *qevent);             //鼠标移动事件
    void paintEvent(QPaintEvent *event);

private slots:

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

    void on_Rem_Passwd_clicked();

private:
    Ui::TcpClient *ui;
    QTcpSocket *tcpSocket;
};

#endif // TCPCLIENT_H
