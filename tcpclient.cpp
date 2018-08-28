#include "tcpclient.h"
#include "ui_tcpclient.h"

#include "registerdialog.h"
#include "resetdialog.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QPushButton>
#include <QMouseEvent>
QString LoginUserID = NULL;
QString UserPhoneNum = NULL;
QNetworkAccessManager *m_accessManager;

TcpClient::TcpClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    ui->passwardLineEdit->setEchoMode(QLineEdit::Password);  //密码方式显示文本
    ui->userLineEdit->setPlaceholderText(tr("请输入用户名/手机号"));//设置用户名提示信息
    ui->passwardLineEdit->setPlaceholderText(tr("请输入密码"));//设置密码提示信息

    ui->signBtn->setStyleSheet(                 //调整注册账号按钮样式
                "QPushButton{border:0px;color:rgb(102,102,102);}"
                "QPushButton:hover{border:0px;color:rgb(57,140,255);}");
    ui->forgetBtn->setStyleSheet(               //调整忘记密码按钮样式
                "QPushButton{border:0px;color:rgb(57,140,255);}"
                "QPushButton:hover{border:0px;color:rgb(85,170,255);}");
    ui->sendBtn->setStyleSheet(                 //调整登录按钮样式
                "QPushButton{background-color: rgb(57, 140, 255);font-size:18pt;font-weight:bold;border-radius:4px;color:white;}"
                "QPushButton:hover{background-color: rgb(85, 170, 255);font-size:18pt;font-weight:bold;border-radius:4px;color:white;}");
    ui->clearBtn->setStyleSheet(                //调整清除密码账号按钮样式
                "QPushButton{ background-image: url(:/new/login/pictures/login_clear_password.png);border:0px;}"
                "QPushButton:hover{background-image: url(:/new/mainwindow/pictures/delete_button_hover.png);border:0px;}");

    ui->signBtn->setCursor(QCursor(Qt::PointingHandCursor));

    ui->forgetBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->sendBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->clearBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->closeBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->minBtn->setCursor(QCursor(Qt::PointingHandCursor));


    flag = 0;


    m_accessManager = new QNetworkAccessManager(this);
    QObject::connect(m_accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));

}

TcpClient::~TcpClient()
{
    delete ui;
}


void TcpClient::init()
{
    //socket 初始化
    tcpSocket=new QTcpSocket(this);
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(displayError(QAbstractSocket::SocketError)));   //发生错误时执行displayError函数
}

void TcpClient::connectServer()
{
    //链接服务器
    tcpSocket->abort();   //取消已有的连接
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessages()));
}

void TcpClient::on_forgetBtn_clicked()
{   //点击忘记密码按钮的响应

    resetDialog *resDlg = new resetDialog(this);
    resDlg->exec();
    return;
}

void TcpClient::on_sendBtn_clicked()
{
    //点击登录按钮的响应

    QString userName=ui->userLineEdit->text();//获得对话框中用户名
    QString passward=ui->passwardLineEdit->text();//获得对话框中密码
    if(userName=="" || passward==""){//判断用户名密码是否为空，为空弹出警告
        QMessageBox::information(this,"警告","输入不能为空",QMessageBox::Ok);
    }else{

        //先判断，手机号是否已经注册， 调用judge_phone.do
        //然后再调用登录接口，登录，调用接口employee.do
        QNetworkRequest request;
        //request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data");
        flag = 3; //步骤：登录
        request.setUrl(QUrl("http://119.23.138.209:8080/cloud/Login/EmployeePC.do"));  //登录
        QByteArray postData;
        //emp_password  emp_email   code
        postData.append("emp_phone=");//参数手机
        postData.append(ui->userLineEdit->text());
        postData.append("&emp_password=");//参数密码
        postData.append(ui->passwardLineEdit->text());//

        QNetworkReply* reply = m_accessManager->post(request,postData);//发送http的post请求
    }
}

void TcpClient::on_clearBtn_clicked()
{   //点击清除密码按钮的响应

    ui->passwardLineEdit->setText("");
    ui->passwardLineEdit->setFocus();
}

void TcpClient::on_signBtn_clicked()
{
    //点击注册账号按钮的响应

    registerDialog *regDlg = new registerDialog(this);
    regDlg->exec();
    return;

}

void TcpClient::on_userLineEdit_textChanged()
{
    //用户名输入框文本变化的响应

    ui->label->setStyleSheet(tr("image: url(:/new/login/pictures/login_account_hover.png);"));
    ui->frame_2->setStyleSheet(tr("color: rgb(57, 140, 255);"));
}

void TcpClient::on_userLineEdit_editingFinished()
{
    //用户名输入框停止输入的响应

    ui->label->setStyleSheet(tr("image: url(:/new/login/pictures/login_account.png);"));
    ui->frame_2->setStyleSheet(tr("color: rgb(199, 197, 198);"));
}

void TcpClient::on_passwardLineEdit_editingFinished()
{
    //密码输入框停止输入的响应

    ui->label_3->setStyleSheet(tr("image: url(:/new/login/pictures/login_password.png);"));
    ui->frame->setStyleSheet(tr("color: rgb(199, 197, 198);"));
}

void TcpClient::on_passwardLineEdit_textChanged()
{
    //密码输入框文本变化的响应

    ui->label_3->setStyleSheet(tr("image: url(:/new/login/pictures/login_password_hover.png);"));
    ui->frame->setStyleSheet(tr("color: rgb(57, 140, 255);"));
}


void TcpClient::displayError(QAbstractSocket::SocketError)
{
    qDebug()<<tcpSocket->errorString();   //输出出错信息
}


void TcpClient::readMessages()
{
    //登录、注册通过socket发送给服务器数据后，服务器返回数据调用的函数。
    QString data=tcpSocket->readAll();
    QStringList list=data.split("#");
    if(list[0]=="a" && list[1]=="true")
        QMessageBox::information(this,"信息提示","注册成功!",QMessageBox::Ok);
    else if(list[0]=="a" && list[1]=="false")
        QMessageBox::information(this,"信息提示","注册失败,用户名已经被注册!",QMessageBox::Ok);
    else if(list[0]=="b" && list[1]=="true")
        QMessageBox::information(this,"信息提示","登录成功!",QMessageBox::Ok);
    else if(list[0]=="b" && list[1]=="false")
            QMessageBox::information(this,"信息提示","登录失败,用户名或密码错误!",QMessageBox::Ok);
    else
        return;
}

//http 返回响应
void TcpClient::finishedSlot(QNetworkReply *reply)
{
     if (reply->error() == QNetworkReply::NoError)
     {
         QByteArray bytes = reply->readAll();
         qDebug()<<bytes;
         //QString string = QString::fromUtf8(bytes);

         if(flag==1){   //判断手机是否注册
             QJsonParseError jsonError;//Qt5新类
             QJsonDocument json = QJsonDocument::fromJson(bytes, &jsonError);//Qt5新类
             if (jsonError.error == QJsonParseError::NoError)
             {
                 if (json.isObject())
                 {
                     QJsonObject rootObj = json.object();
                     QString rootpath;
                     int rootusernum;
                     //是否含有key  rootpath
                     if (rootObj.contains("status"))
                     {
                         //取出key为rootpath的值
                         QJsonValue value = rootObj.value("status");
                         //判断是否是string类型
                         if (value.isString())
                         {
                             qDebug()<<rootpath;
                             rootpath = value.toString();
                             if(rootpath!="Success")
                             {

                             }
                         }

                     }

                     if (rootObj.contains("content"))
                     {
                         //取出key为rootpath的值
                         QJsonValue value = rootObj.value("content");
                         QString content;
                         //判断是否是string类型
                         if (value.isString())
                         {
                             qDebug()<<rootpath;
                             content = value.toString();
                             if(content.contains("available",Qt::CaseSensitive))
                             {

                             }
                         }

                     }
                 }
             }
         }else if(flag==2){ //注册
             QJsonParseError jsonError;//Qt5新类
             QJsonDocument json = QJsonDocument::fromJson(bytes, &jsonError);//Qt5新类
             if (jsonError.error == QJsonParseError::NoError)
             {
                 if (json.isObject())
                 {
                     QJsonObject rootObj = json.object();
                     QString rootpath;
                     int rootusernum;
                     //是否含有key  rootpath
                     if (rootObj.contains("status"))
                     {
                         //取出key为rootpath的值
                         QJsonValue value = rootObj.value("status");
                         //判断是否是string类型
                         if (value.isString())
                         {
                             qDebug()<<rootpath;
                             rootpath = value.toString();
                             if(rootpath!="Success")
                             {

                             }
                         }

                     }

                     if (rootObj.contains("content"))
                     {
                         //取出key为rootpath的值
                         QJsonValue value = rootObj.value("content");
                         QString content;
                         //判断是否是string类型
                         if (value.isString())
                         {
                             qDebug()<<rootpath;
                             content = value.toString();
                             if(content.contains("error",Qt::CaseSensitive))
                             {
                                 //验证码错误。
                                  QMessageBox::information(this,"警告","验证码错误",QMessageBox::Ok);


                             }
                             else if(content.contains("invalid",Qt::CaseSensitive))
                             {
                                 //验证码错误。
                                  QMessageBox::information(this,"警告","验证码失败",QMessageBox::Ok);


                             }
                             else{
                                 QMessageBox::information(this,"警告","注册成功",QMessageBox::Ok);
                             }
                         }

                     }
                 }
             }

         }else if(flag==4){   //获取验证码返回
             QJsonParseError jsonError;
             QJsonDocument json = QJsonDocument::fromJson(bytes, &jsonError);
             if (jsonError.error == QJsonParseError::NoError)
             {
                 if (json.isObject())
                 {
                     QJsonObject rootObj = json.object();
                     QString rootpath;
                     int rootusernum;
                     //是否含有key  rootpath
                     if (rootObj.contains("status"))
                     {
                         //取出key为rootpath的值
                         QJsonValue value = rootObj.value("status");
                         //判断是否是string类型
                         if (value.isString())
                         {
                             qDebug()<<rootpath;
                             rootpath = value.toString();
                             if(rootpath!="Success")
                             {

                             }
                         }

                     }

                     if (rootObj.contains("content"))
                     {
                         //取出key为rootpath的值
                         QJsonValue value = rootObj.value("content");
                         QString content;
                         //判断是否是string类型
                         if (value.isString())
                         {
                             qDebug()<<rootpath;
                             content = value.toString();
                             if(content.contains("success",Qt::CaseSensitive))
                             {
                                 //成功，发送注册信息。
                                 //http 判断手机号是否注册
                                qDebug()<<"code ok";

                             }else{
                                 qDebug()<<"code error";

                             }
                         }

                     }
                 }
             }
         }else{//登录
             QJsonParseError jsonError;//Qt5新类
             QJsonDocument json = QJsonDocument::fromJson(bytes, &jsonError);//Qt5新类
             if (jsonError.error == QJsonParseError::NoError)
             {
                 if (json.isObject())
                 {
                     QJsonObject rootObj = json.object();
                     QString rootpath;
                     int rootusernum;
                     //是否含有key  rootpath
                     if (rootObj.contains("status"))
                     {
                         //取出key为rootpath的值
                         QJsonValue value = rootObj.value("status");
                         //判断是否是string类型
                         if (value.isString())
                         {
                             qDebug()<<rootpath;
                             rootpath = value.toString();
                             if(rootpath!="Success")
                             {

                             }
                         }

                     }

                     if (rootObj.contains("content"))
                     {
                         //取出key为rootpath的值
                         QJsonValue value = rootObj.value("content");
                         QString content;
                         //判断是否是string类型
                         if (value.isString())
                         {
                             qDebug()<<rootpath;
                             content = value.toString();

                             if(content.contains("phonenull",Qt::CaseSensitive))
                             {
                                 //验证码错误。
                                  QMessageBox::information(this,"警告","电话号不存在",QMessageBox::Ok);


                             }
                             else if(content.contains("passworderror",Qt::CaseSensitive))
                             {
                                 //验证码错误。
                                  QMessageBox::information(this,"警告","密码错误",QMessageBox::Ok);


                             }
                             else if(content.contains("freezing",Qt::CaseSensitive))
                             {
                                 //验证码错误。
                                  QMessageBox::information(this,"警告","账号已冻结",QMessageBox::Ok);


                             }
                             else{
                                 accept();
                                 int index = content.lastIndexOf('_');
                                 QString midQString = content.mid(0,index);
                                 index = midQString.lastIndexOf('_');
                                 UserPhoneNum = midQString.mid(0,index);
                                 LoginUserID = midQString.mid(index+1);
                                 qDebug()<<LoginUserID;

//                                 QMessageBox::information(this,"警告","登录成功",QMessageBox::Ok);
                             }
                         }

                     }
                 }
             }
         }

         flag = 0;


     }
     else
     {

         qDebug()<<"handle errors here";
         QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
         qDebug( "found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
         qDebug(qPrintable(reply->errorString()));
     }
     reply->deleteLater();
}

void TcpClient::on_minBtn_clicked()
{
    this->showMinimized();   //窗口最小化
}

void TcpClient::on_closeBtn_clicked()
{
    this->close();        //窗口关闭
}

//点击获取验证码
void TcpClient::on_codeBtn_clicked()
{
    //http 获取验证码
    QNetworkRequest request;
    //request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data");
    flag = 4; //步骤：获取验证码
    request.setUrl(QUrl("http://119.23.138.209:8080/cloud/AcquireCode.do"));
    QByteArray postData;
    postData.append("emp_phone=");//参数
    postData.append(ui->userLineEdit->text());//参数
    QNetworkReply* reply = m_accessManager->post(request,postData);//发送http的post请求
}

void TcpClient::mousePressEvent(QMouseEvent *qevent)
{
    if(qevent->button() == Qt::LeftButton)
    {
        mouse_press = true;
        //鼠标相对于窗体的位置（或者使用event->globalPos() - this->pos()）
        move_point = qevent->pos();;
    }
}
void TcpClient::mouseMoveEvent(QMouseEvent *qevent)
{
    //若鼠标左键被按下
    if(mouse_press)
    {
        //鼠标相对于屏幕的位置
        QPoint move_pos = qevent->globalPos();

        //移动主窗体位置
        this->move(move_pos - move_point);
    }
}
void TcpClient::mouseReleaseEvent(QMouseEvent *qevent)
{
    //设置鼠标为未被按下
    mouse_press = false;
}

void TcpClient::paintEvent(QPaintEvent *event)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(5, 5, this->width()-10, this->height()-10);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));

    QColor color(0, 0, 0, 50);
    for(int i=0; i<5; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(5-i, 5-i, this->width()-(5-i)*2, this->height()-(5-i)*2);
        color.setAlpha(150 - qSqrt(i)*50);
        painter.setPen(color);
        painter.drawPath(path);
    }
}

