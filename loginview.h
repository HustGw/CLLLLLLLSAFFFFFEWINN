#ifndef LOGINVIEW_H
#define LOGINVIEW_H

#include <QDialog>
#include <QDebug>
#include "mylabel.h"
#include "tcpclient.h"
namespace Ui {
class LoginView;
}

class LoginView : public QDialog
{
    Q_OBJECT

public:
    explicit LoginView(QWidget *parent = 0);
    ~LoginView();
    Mylabel *registerBtn;
    TcpClient *tcpWindow;

private slots:

    void on_LoginBtn_clicked();

    void RegisterClicked();

private:
    Ui::LoginView *ui;
};

#endif // LOGINVIEW_H
