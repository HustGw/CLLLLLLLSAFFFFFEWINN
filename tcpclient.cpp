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
#include <QSettings>
QString Mac_address;
QString LoginUserID = nullptr;
QString UserPhoneNum = nullptr;
QNetworkAccessManager *m_accessManager;

TcpClient::TcpClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    //登录初始化时设置已记住的账号密码
    QString RemeberPasswd;
    QSettings cfg("user.ini",QSettings::IniFormat);
    usersname= cfg.value("usersname").toString();
    passwd= cfg.value("passwd").toString();
    RemeberPasswd= cfg.value("remeberPasswd").toString();
    if(RemeberPasswd == "true"){
        ui->userLineEdit->setText(usersname);
        ui->passwardLineEdit->setText(passwd);
        ui->Rem_Passwd->setChecked(true);
    }


    ui->passwardLineEdit->setEchoMode(QLineEdit::Password);  //密码方式显示文本
    ui->userLineEdit->setPlaceholderText(tr("请输入手机号"));//设置用户名提示信息
    ui->passwardLineEdit->setPlaceholderText(tr("请输入密码"));//设置密码提示信息

    ui->signBtn->setStyleSheet(                 //调整注册账号按钮样式
                "QPushButton{border:0px;color:rgb(102,102,102);font:16px;}"
                "QPushButton:hover{border:0px;color:rgb(57,140,255);}");
    ui->forgetBtn->setStyleSheet(               //调整忘记密码按钮样式
                "QPushButton{border:0px;color:rgb(57,140,255);}"
                "QPushButton:hover{border:0px;color:rgb(85,170,255);}");
    ui->sendBtn->setStyleSheet(                 //调整登录按钮样式
                "QPushButton{background-color: rgb(57, 140, 255);font: 22px 黑体;border-radius:4px;color:white;}"
                "QPushButton:hover{background-color: rgb(85, 170, 255);font: 22px 黑体;border-radius:4px;color:white;}");
    ui->clearBtn->setStyleSheet(                //调整清除密码账号按钮样式
                "QPushButton{ background-image: url(:/new/login/pictures/login_clear_password.png);border:0px;}"
                "QPushButton:hover{background-image: url(:/new/mainwindow/pictures/delete_button_hover.png);border:0px;}");
//    ui->centralWidget->setStyleSheet("border:1px solid #999999;border-radius:6px;");

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
        Mac_address = getHostMacAddress();
        QString Ip_address = getHostIpAddress();
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
        postData.append("&mac_address=");//参数
        postData.append(Mac_address);//参数
        postData.append("&ip_address=");//参数
        postData.append(Ip_address);//参数
        m_accessManager->post(request,postData);//发送http的post请求
    }
}

QString TcpClient::getHostIpAddress()
{
    QString strIpAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // 获取第一个本主机的IPv4地址
    int nListSize = ipAddressesList.size();
    for (int i = 0; i < nListSize; ++i)
    {
           if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
               ipAddressesList.at(i).toIPv4Address()) {
               strIpAddress = ipAddressesList.at(i).toString();
               break;
           }
     }
     // 如果没有找到，则以本地IP地址为IP
     if (strIpAddress.isEmpty())
        strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();
     return strIpAddress;
}

QString TcpClient::getHostMacAddress()
{
    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// 获取所有网络接口列表
    int nCnt = nets.count();
    QString strMacAddr = "";
    for(int i = 0; i < nCnt; i ++)
    {
        // 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
        if(nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            strMacAddr = nets[i].hardwareAddress();
            break;
        }
    }
    return strMacAddr;
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
         }
         else{//登录
             QJsonParseError jsonError;//Qt5新类
             QJsonDocument json = QJsonDocument::fromJson(bytes, &jsonError);//Qt5新类
             if (jsonError.error == QJsonParseError::NoError)
             {
                 if (json.isObject())
                 {
                     QJsonObject rootObj = json.object();
                     QString rootpath;
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
                                  QMessageBox::warning(this,"警告","电话号不存在！",QMessageBox::Ok);


                             }
                             else if(content.contains("passworderror",Qt::CaseSensitive))
                             {
                                 //验证码错误。
                                  QMessageBox::warning(this,"警告","密码错误！",QMessageBox::Ok);


                             }
                             else if(content.contains("freezing",Qt::CaseSensitive))
                             {
                                 //验证码错误。
                                  QMessageBox::warning(this,"警告","账号已冻结！",QMessageBox::Ok);


                             }else if(content.contains("repeatlogin",Qt::CaseSensitive))
                             {
                                 QMessageBox::warning(this,"警告","禁止重复登录！",QMessageBox::Ok);
                             }
                             else{
                                 accept();
                                 int index = content.lastIndexOf('_');
                                 QString midQString = content.mid(0,index);
                                 index = midQString.lastIndexOf('_');
                                 UserPhoneNum = midQString.mid(0,index);
                                 LoginUserID = midQString.mid(index+1);
                                 qDebug()<<LoginUserID;

                                 savecfg();
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
         qDebug("found error ....code: %d %d\n", statusCodeV.toInt(), reply->error());
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

    QColor color(128, 128, 128, 50);
    for(int i=0; i<5; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(5-i, 5-i, this->width()-(5-i)*2, this->height()-(5-i)*2);
        color.setAlpha(150 - qSqrt(i)*50);
        painter.setPen(color);
        painter.drawPath(path);
    }
    QPainter p(this);
    p.setCompositionMode( QPainter::CompositionMode_Clear );
    p.fillRect(6,6,571,440, Qt::SolidPattern);

}


//勾选记住密码,定义一个标志位
void TcpClient::on_Rem_Passwd_clicked(){
    if(ui->Rem_Passwd->isChecked()){
        remeberPasswd = true;
    }else{
        remeberPasswd = false;
    }
}

//登录成功后保存登录信息
void TcpClient::savecfg(){
    QSettings cfg("user.ini",QSettings::IniFormat);
    usersname = ui->userLineEdit->text();
    passwd = ui->passwardLineEdit->text();
    cfg.setValue("usersname",usersname);
    cfg.setValue("passwd",passwd);
    cfg.setValue("remeberPasswd",remeberPasswd);
    cfg.sync();
}
