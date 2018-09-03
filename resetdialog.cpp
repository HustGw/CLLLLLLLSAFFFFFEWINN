#include "resetdialog.h"
#include "ui_resetdialog.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QMouseEvent>
#include <QRegExpValidator>
#include <Qvalidator>
QNetworkAccessManager *m_accessManagerReset;

resetDialog::resetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::resetDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    ui->passwordLineEdit_2->setEchoMode(QLineEdit::Password);

    ui->userLineEdit->setPlaceholderText("请输入手机号");
    ui->email->setPlaceholderText("请输入邮箱");
    ui->passwordLineEdit->setPlaceholderText("设置你的登录密码");
    ui->passwordLineEdit_2->setPlaceholderText("请再次输入你的密码");

    ui->userAlert->setVisible(false);
    ui->emailAlert->setVisible(false);
    ui->passwordAlert->setVisible(false);
    ui->codeAlert->setVisible(false);

    ui->confirmBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->codeBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->closeBtn->setCursor(QCursor(Qt::PointingHandCursor));

    ui->confirmBtn->setStyleSheet(                 //调整登录按钮样式
                "QPushButton{border-radius:4px;background-color: rgb(46, 130, 255);font-size:14pt;font-weight:bold;color:white;}"
                "QPushButton:hover{border-radius:4px;background-color: rgb(85, 170, 255);font-size:14pt;font-weight:bold;color:white;}");
    ui->codeBtn->setStyleSheet(                 //调整验证码按钮样式
                "QPushButton{background-color: rgb(247, 247, 247);color:rgb(153,153,171);border:1px solid rgb(214,216,221);border-radius:4px;}"
                "QPushButton:hover{background-color: rgb(247, 247, 247);color:rgb(46,130,255);border:1px solid rgb(46,130,255);border-radius:4px;}");
    ui->closeBtn->setStyleSheet(               //调整关闭按钮样式
                "QPushButton{border-image: url(:/new/mainwindow/pictures/delete_button.png);border:none;background-color:#EEF0F5;}"
                "QPushButton:hover{border-image: url(:/new/mainwindow/pictures/delete_button_hover.png);border:none;background-color:#EEF0F5;}");
    ui->passwordLineEdit->setStyleSheet(                 //调整密码输入框样式
                "QLineEdit{border:1px solid rgb(214,216,221);border-radius:4px;}"
                "QLineEdit:hover{border:1px solid rgb(46,130,255);border-radius:4px;}");
    ui->passwordLineEdit_2->setStyleSheet(                 //调整确认密码输入框样式
                "QLineEdit{border:1px solid rgb(214,216,221);border-radius:4px;}"
                "QLineEdit:hover{border:1px solid rgb(46,130,255);border-radius:4px;}");
    ui->userLineEdit->setStyleSheet(                 //调整手机号输入框样式
                "QLineEdit{border:1px solid rgb(214,216,221);border-radius:4px;}"
                "QLineEdit:hover{border:1px solid rgb(46,130,255);border-radius:4px;}");
    ui->email->setStyleSheet(                 //调整邮箱输入框样式
                "QLineEdit{border:1px solid rgb(214,216,221);border-radius:4px;}"
                "QLineEdit:hover{border:1px solid rgb(46,130,255);border-radius:4px;}");
    ui->code->setStyleSheet(                 //调整验证码输入框样式
                "QLineEdit{border:1px solid rgb(214,216,221);border-radius:4px;}"
                "QLineEdit:hover{border:1px solid rgb(46,130,255);border-radius:4px;}");

    flag = 0;

    qDebug()<<"hehe flag:"<<flag;
    m_accessManagerReset = new QNetworkAccessManager(this);
    QObject::connect(m_accessManagerReset, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));

}

resetDialog::~resetDialog()
{
    delete ui;
}

void resetDialog::on_confirmBtn_clicked()
{
   //点击注册按钮的响应
    QString userName=ui->userLineEdit->text();  //获取对话框中用户名
    QString email = ui->email->text();
    QString code = ui->code->text();
    QString password=ui->passwordLineEdit->text();//获取对话框中密码
    QString password_2=ui->passwordLineEdit_2->text();
    QRegExp rx("[1]\\d{10}");
    QRegExp rx2("[a-zA-Z0-9_]+@[a-zA-Z0-9_]+(\\.[a-zA-Z]+)+");
    QRegExpValidator v(rx,0);
    QRegExpValidator v2(rx2,0);
    int pos = 0;
    int flag_2 = 1;

   if(userName=="" || password=="" || code==""){//判断用户名、密码、验证码是否为空，为空弹出警告
       if(password != password_2){
           ui->passwordAlert->setVisible(true);  //两次密码不一样，出现提示行
       }
       if(userName != "" && QValidator::Acceptable!=v.validate(userName,pos)){
           ui->userAlert->setText("手机号码格式不正确");
           ui->userAlert->setVisible(true);
       }
       if(email != "" && QValidator::Acceptable!=v2.validate(email,pos)){
           ui->emailAlert->setVisible(true);
       }
       QMessageBox::information(this,"警告","输入不能为空",QMessageBox::Ok);
   }else{
       if(password != password_2){
           ui->passwordAlert->setVisible(true);  //两次密码不一样，出现提示行
           flag_2 = 0;
       }
       if(QValidator::Acceptable!=v.validate(userName,pos)){
           ui->userAlert->setText("手机号码格式不正确");
           ui->userAlert->setVisible(true);
           flag_2 = 0;
       }
       if(email != "" && QValidator::Acceptable!=v2.validate(email,pos)){
           ui->emailAlert->setVisible(true);
           flag_2 = 0;
       }
       if(flag_2 == 1){
       QNetworkRequest request;
       flag = 1;
       qDebug()<<"hehe flag:"<<flag;
       request.setUrl(QUrl("http://119.23.138.209:8080/cloud/Employee/Judge_Phone.do"));  //判断手机是否注册

       QByteArray postData;
       postData.append("emp_phone=");//参数
       postData.append(ui->userLineEdit->text());//参数
//       postData.append("&emp_password=");//参数
//       postData.append(ui->passwordLineEdit->text());//参数
//       postData.append("&emp_email=");//参数
//       postData.append(ui->email->text());//参数
//       postData.append("&code=");//参数
//       postData.append(ui->code->text());//参数
       QNetworkReply* reply = m_accessManagerReset->post(request,postData);//发送http的post请求
       }
   }
}


void resetDialog::on_codeBtn_clicked(){
    //http 获取验证码
    QNetworkRequest request;
    //request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data");
    flag = 3; //步骤：获取验证码
    qDebug()<<"hehe flag:"<<flag;
    request.setUrl(QUrl("http://119.23.138.209:8080/cloud/AcquireCode.do"));
    QByteArray postData;
    postData.append("emp_phone=");//参数
    postData.append(ui->userLineEdit->text());//参数
    QNetworkReply* reply = m_accessManagerReset->post(request,postData);//发送http的post请求
}

void resetDialog::finishedSlot(QNetworkReply *reply){

    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();

        qDebug()<<"json:"<<bytes;
        qDebug()<<"flag:"<<flag;
        //QString string = QString::fromUtf8(bytes);

        QString jsonString = QString(bytes);
        QByteArray njba = jsonString.toUtf8();
         QJsonObject nobj = QJsonObject(QJsonDocument::fromJson(njba).object());


         QString status = nobj.take("status").toString();
         QString content = nobj.take("content").toString();

         qDebug()<<"content:"<<content;
         if(flag==1){
            qDebug()<<"判断手机号是否注册";

            if(content == "unavailable"){
                QNetworkRequest request;
                flag = 2;
                qDebug()<<"hehe flag:"<<flag;

                request.setUrl(QUrl("http://119.23.138.209:8080/cloud/ResetPassword/RestPasswordByCode.do"));
                QByteArray postData;
                postData.append("emp_phone=");//参数
                postData.append(ui->userLineEdit->text());//参数
                postData.append("&emp_password=");//参数
                postData.append(ui->passwordLineEdit->text());//参数
                postData.append("&emp_email=");//参数
                postData.append(ui->email->text());//参数
                postData.append("&code=");//参数
                postData.append(ui->code->text());//参数
                QNetworkReply* reply = m_accessManagerReset->post(request,postData);//发送http的post请求
            }else{
                ui->userAlert->setText("手机号未注册！");
                ui->userAlert->setVisible(true);
            }
            return;
         }
         else if(flag==2){
             qDebug()<<"判断是否重置成功";

             if(content.contains("success",Qt::CaseSensitive)){
                 QMessageBox::information(this,"提示","密码重置成功！",QMessageBox::Ok);
                 this->close();
             }else{              //如果注册账号的时候没填邮箱，那么重置密码的时候也别填密码，否则会弹出这个提示框。
                 QMessageBox::information(this,"警告","邮箱错误！",QMessageBox::Ok); //如果注册账号的时候填了邮箱，那么重置密码的时候必须填上跟注册时一样的邮箱。
             }
         }
         else if(flag==3){
             qDebug()<<"获取验证码";

             if(content.contains("success",Qt::CaseSensitive)){
                 QMessageBox::information(this,"提示","验证码已发送！",QMessageBox::Ok);
             }else{
                  QMessageBox::information(this,"警告","验证码发送失败！",QMessageBox::Ok);
             }
             return;
         }else{
             flag = 0;
         }
   }
   else{

        qDebug()<<"handle errors here";
        QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        //statusCodeV是HTTP服务器的相应码
        qDebug( "found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
        qDebug(qPrintable(reply->errorString()));
   }
   reply->deleteLater();
}



void resetDialog::on_closeBtn_clicked(){
    this->close();       //窗口关闭
}

void resetDialog::on_passwordLineEdit_textChanged(){
    QString password=ui->passwordLineEdit->text();  //获取对话框中密码
    QString password_2=ui->passwordLineEdit_2->text();  //获取确认密码框中密码

    if(password_2 != ""){
        if(password != password_2){
            ui->passwordAlert->setVisible(true);
        }else{
            ui->passwordAlert->setVisible(false);
        }
    }
}

void resetDialog::on_passwordLineEdit_editingFinished(){
    QString password=ui->passwordLineEdit->text();  //获取对话框中密码
    QString password_2=ui->passwordLineEdit_2->text();  //获取确认密码框中密码

    if(password_2 != ""){
        if(password != password_2){
            ui->passwordAlert->setVisible(true);
        }else{
            ui->passwordAlert->setVisible(false);
        }
    }
}

void resetDialog::on_passwordLineEdit_2_textChanged(){
    QString passward=ui->passwordLineEdit->text();  //获取对话框中密码
    QString passward_2=ui->passwordLineEdit_2->text();  //获取确认密码框中密码

    if(passward != passward_2){
        ui->passwordAlert->setVisible(true);  //两次密码不一样，出现提示行
    }else{
        ui->passwordAlert->setVisible(false);
    }
}

void resetDialog::on_passwordLineEdit_2_editingFinished(){

    QString passward=ui->passwordLineEdit->text();  //获取对话框中密码
    QString passward_2=ui->passwordLineEdit_2->text();  //获取确认密码框中密码

    if(passward != passward_2){
        ui->passwordAlert->setVisible(true);  //两次密码不一样，出现提示行
    }else{
        ui->passwordAlert->setVisible(false);
    }
}

void resetDialog::on_userLineEdit_textChanged(){
    ui->userAlert->setVisible(false);
}

void resetDialog::on_userLineEdit_editingFinished(){
    QString userName=ui->userLineEdit->text();  //获取对话框中用户名

    QRegExp rx("[1]\\d{10}");
    QRegExpValidator v(rx,0);
    int pos = 0;

    if(userName != "" && QValidator::Acceptable!=v.validate(userName,pos)){
        ui->userAlert->setText("手机号码格式不正确");
        ui->userAlert->setVisible(true);
    }else{
        ui->userAlert->setVisible(false);
    }
}

void resetDialog::on_email_editingFinished(){
    QString email = ui->email->text();
    QRegExp rx2("[a-zA-Z0-9_]+@[a-zA-Z0-9_]+(\\.[a-zA-Z]+)+");
    QRegExpValidator v2(rx2,0);
    int pos = 0;

    if(email != "" && QValidator::Acceptable!=v2.validate(email,pos)){
        ui->emailAlert->setVisible(true);
    }else{
        ui->emailAlert->setVisible(false);
    }
}

void resetDialog::paintEvent(QPaintEvent *event)
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
void resetDialog::mousePressEvent(QMouseEvent *qevent)
{
    if(qevent->button() == Qt::LeftButton)
    {
        mouse_press = true;
        //鼠标相对于窗体的位置（或者使用event->globalPos() - this->pos()）
        move_point = qevent->pos();;
    }
}
void resetDialog::mouseMoveEvent(QMouseEvent *qevent)
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
void resetDialog::mouseReleaseEvent(QMouseEvent *qevent)
{
    //设置鼠标为未被按下
    mouse_press = false;
}
