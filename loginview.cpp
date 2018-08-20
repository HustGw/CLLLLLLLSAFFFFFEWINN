#include "loginview.h"
#include "msgbox.h"
#include "ui_loginview.h"
#include <QMessageBox>

LoginView::LoginView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginView)
{
    ui->setupUi(this);
    //设置窗口大小 窗口大小不可编辑
    this->setFixedSize(this->width(),this->height());
    ui->PassWord->setEchoMode(QLineEdit::Password);
    QFont f("ZYSong18030",12,75);
    ui->Title->setFont(f);
    ui->LoginBtn->setFont(f);
    ui->UserName->setPlaceholderText("请输入用户名/手机号");
    ui->PassWord->setPlaceholderText("请输入密码");
    registerBtn = new Mylabel(this);
    registerBtn->setGeometry(120,280,60,16);
    registerBtn->setText("注册账号");
    connect(registerBtn,SIGNAL(LabelClicked()),this,SLOT(RegisterClicked()));

}

LoginView::~LoginView()
{
    delete ui;
}
//点击登录按钮响应事件 此处可对账号密码进行校验
void LoginView::on_LoginBtn_clicked()
{
    if(ui->UserName->text()==tr("111")&&ui->PassWord->text()==tr("111")){
        accept();
    }
    else{
        MsgBox *msgbox = new MsgBox(2,QStringLiteral("用户名或密码错误！"));
        msgbox->exec();
    }
}

void LoginView::RegisterClicked(){
    qDebug()<<"注册点击";
    tcpWindow  = new TcpClient();
    tcpWindow->show();

}


