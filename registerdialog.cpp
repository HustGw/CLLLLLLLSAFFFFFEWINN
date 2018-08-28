#include "registerdialog.h"
#include "ui_registerdialog.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QMouseEvent>
#include <QRegExpValidator>
#include <Qvalidator>

QNetworkAccessManager *m_accessManagerRegister;

registerDialog::registerDialog(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::registerDialog)
{
   ui->setupUi(this);
   setWindowFlags(windowFlags()|Qt::FramelessWindowHint);

   flag = 0;
   ui->passwardLineEdit->setEchoMode(QLineEdit::Password);
   ui->passwardLineEdit_2->setEchoMode(QLineEdit::Password);
   ui->nichengLineEdit->setPlaceholderText(tr("昵称一旦设置成功，无法修改"));
   ui->passwardLineEdit->setPlaceholderText(tr("设置你的登录密码"));
   ui->passwardLineEdit_2->setPlaceholderText(tr("请再次输入你的密码"));
   ui->userLineEdit->setPlaceholderText(tr("请输入手机号"));
   ui->email->setPlaceholderText(tr("请输入邮箱"));
   ui->nichengAlert->setVisible(false);
   ui->passwardAlert->setVisible(false);
   ui->userAlert->setVisible(false);
   ui->codeAlert->setVisible(false);
   ui->emailAlert->setVisible(false);

   ui->signBtn->setStyleSheet(                 //调整登录按钮样式
               "QPushButton{border-radius:4px;background-color: rgb(46, 130, 255);font-size:14pt;font-weight:bold;color:white;}"
               "QPushButton:hover{border-radius:4px;background-color: rgb(85, 170, 255);font-size:14pt;font-weight:bold;color:white;}");
   ui->codeBtn->setStyleSheet(                 //调整登录按钮样式
               "QPushButton{background-color: rgb(247, 247, 247);color:rgb(153,153,171);border:1px solid rgb(214,216,221);border-radius:4px;}"
               "QPushButton:hover{background-color: rgb(247, 247, 247);color:rgb(46,130,255);border:1px solid rgb(46,130,255);border-radius:4px;}");
   ui->closeBtn->setStyleSheet(               //调整关闭按钮样式
               "QPushButton{background-image: url(:/new/mainwindow/pictures/delete_button.png);border:none;}"
               "QPushButton:hover{background-image: url(:/new/mainwindow/pictures/delete_button.png);border:none;background-color:rgb(200,200,200)}");
   ui->nichengLineEdit->setStyleSheet(                 //调整登录按钮样式
               "QLineEdit{border:1px solid rgb(214,216,221);border-radius:4px;}"
               "QLineEdit:hover{border:1px solid rgb(46,130,255);border-radius:4px;}");
   ui->passwardLineEdit->setStyleSheet(                 //调整登录按钮样式
               "QLineEdit{border:1px solid rgb(214,216,221);border-radius:4px;}"
               "QLineEdit:hover{border:1px solid rgb(46,130,255);border-radius:4px;}");
   ui->passwardLineEdit_2->setStyleSheet(                 //调整登录按钮样式
               "QLineEdit{border:1px solid rgb(214,216,221);border-radius:4px;}"
               "QLineEdit:hover{border:1px solid rgb(46,130,255);border-radius:4px;}");
   ui->userLineEdit->setStyleSheet(                 //调整登录按钮样式
               "QLineEdit{border:1px solid rgb(214,216,221);border-radius:4px;}"
               "QLineEdit:hover{border:1px solid rgb(46,130,255);border-radius:4px;}");
   ui->email->setStyleSheet(                 //调整登录按钮样式
               "QLineEdit{border:1px solid rgb(214,216,221);border-radius:4px;}"
               "QLineEdit:hover{border:1px solid rgb(46,130,255);border-radius:4px;}");
   ui->code->setStyleSheet(                 //调整登录按钮样式
               "QLineEdit{border:1px solid rgb(214,216,221);border-radius:4px;}"
               "QLineEdit:hover{border:1px solid rgb(46,130,255);border-radius:4px;}");


   qDebug()<<"hehe flag:"<<flag;
   //http
   m_accessManagerRegister = new QNetworkAccessManager(this);
   QObject::connect(m_accessManagerRegister, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));

}

registerDialog::~registerDialog()
{
   delete ui;
}

void registerDialog::on_signBtn_clicked()
{
    ui->nichengAlert->setVisible(false);
    ui->passwardAlert->setVisible(false);
    ui->userAlert->setVisible(false);
    ui->codeAlert->setVisible(false);
    ui->emailAlert->setVisible(false);
   //点击注册按钮的响应
   QString s;
   QRegExp rx("[1]\\d{10}");
   QRegExp rx2("[a-zA-Z0-9_]+@[a-zA-Z0-9_]+(\\.[a-zA-Z]+)+");
   QRegExpValidator v(rx,0);
   QRegExpValidator v2(rx2,0);
   int pos = 0;
   QString userName=ui->userLineEdit->text();  //获取对话框中用户名
   QString passward=ui->passwardLineEdit->text();//获取对话框中密码
   QString passward_2=ui->passwardLineEdit_2->text();
   QString nicheng=ui->nichengLineEdit->text();//获取昵称
   QString email=ui->email->text();
   if(userName=="" || passward==""|| nicheng==""){//判断用户名、密码是否为空，为空弹出警告
       QMessageBox::information(this,"警告","输入不能为空",QMessageBox::Ok);
   }
   else{
       if(QValidator::Acceptable!=v.validate(userName,pos)){
           ui->userAlert->setVisible(true);
       }else if(QValidator::Acceptable!=v2.validate(email,pos)){
           ui->emailAlert->setVisible(true);
       }else if(passward!=passward_2){
           ui->passwardAlert->setVisible(true);
       }else if(ui->checkBox->isChecked()==true){
           //http 判断手机号是否注册
           QNetworkRequest request;
           //request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data");
           flag = 1; //步骤：判断手机是否注册
           qDebug()<<"hehe flag:"<<flag;
           request.setUrl(QUrl("http://119.23.138.209:8080/cloud/Employee/Judge_Phone.do"));  //判断手机是否注册

           QByteArray postData;
           postData.append("emp_phone=");//参数
           postData.append(ui->userLineEdit->text());//参数
           QNetworkReply* reply = m_accessManagerRegister->post(request,postData);//发送http的post请求

           /*
           //登录测试
           QNetworkRequest request;
           flag = 2; //步骤：注册
           request.setUrl(QUrl("http://119.23.138.209:8080/cloud/Register/Employee.do"));  //登录
           QByteArray postData;
           //emp_password  emp_email   code
           postData.append("emp_phone=");//参数手机
           postData.append(ui->userLineEdit->text());
           postData.append("&emp_password=");//参数密码
           postData.append(ui->passwardLineEdit->text());//
           postData.append("&emp_email=");//参数邮箱
           postData.append(ui->email->text());//
           postData.append("&code=");//参数验证码
           postData.append(ui->code->text());//
           QNetworkReply* reply = m_accessManagerRegister->post(request,postData);//发送http的post请求
           */
       }
   }
}

//void registerDialog::on_userLineEdit_textChanged()
//{
//    //用户名输入框文本变化的响应

//    ui->userLineEdit->setStyleSheet(tr("border:1px solid rgb(46,130,225);border-radius:4px;"));
//}

//void registerDialog::on_userLineEdit_editingFinished()
//{
//    //用户名输入框停止输入的响应

//    ui->userLineEdit->setStyleSheet(tr("border:1px solid rgb(214,216,221);border-radius:4px;"));
//}

//void registerDialog::on_passwardLineEdit_textChanged()
//{
//    //密码输入框文本变化的响应

//    ui->passwardLineEdit->setStyleSheet(tr("border:1px solid rgb(46,130,225);border-radius:4px;"));
//}

//void registerDialog::on_passwardLineEdit_editingFinished()
//{
//    //密码输入框停止输入的响应

//    ui->passwardLineEdit->setStyleSheet(tr("border:1px solid rgb(214,216,221);border-radius:4px;"));
//}

//void registerDialog::on_passwardLineEdit_2_textChanged()
//{
//    //确认密码框文本变化的响应

//    ui->passwardLineEdit_2->setStyleSheet(tr("border:1px solid rgb(46,130,225);border-radius:4px;"));
//}

//void registerDialog::on_passwardLineEdit_2_editingFinished()
//{
//    //确认密码框停止输入的响应

//    ui->passwardLineEdit_2->setStyleSheet(tr("border:1px solid rgb(214,216,221);border-radius:4px;"));
//}

//void registerDialog::on_nichengLineEdit_textChanged()
//{
//    //昵称输入框文本变化的响应

//    ui->nichengLineEdit->setStyleSheet(tr("border:1px solid rgb(46,130,225);border-radius:4px;"));
//}

//void registerDialog::on_nichengLineEdit_editingFinished()
//{
//    //昵称输入框停止输入的响应

//    ui->nichengLineEdit->setStyleSheet(tr("border:1px solid rgb(214,216,221);border-radius:4px;"));
//}

//void registerDialog::on_email_textChanged()
//{
//    //邮箱输入框文本变化的响应

//    ui->email->setStyleSheet(tr("border:1px solid rgb(46,130,225);border-radius:4px;"));
//}

//void registerDialog::on_email_editingFinished()
//{
//    //邮箱输入框停止输入的响应

//    ui->email->setStyleSheet(tr("border:1px solid rgb(214,216,221);border-radius:4px;"));
//}

//void registerDialog::on_code_textChanged()
//{
//    //验证码框文本变化的响应

//    ui->code->setStyleSheet(tr("border:1px solid rgb(46,130,225);border-radius:4px;"));
//}

//void registerDialog::on_code_editingFinished()
//{
//    //验证码框停止输入的响应

//    ui->code->setStyleSheet(tr("border:1px solid rgb(214,216,221);border-radius:4px;"));
//}

void registerDialog::on_codeBtn_clicked(){
    //http 获取验证码
    QNetworkRequest request;
    //request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data");
    flag = 4; //步骤：获取验证码
    qDebug()<<"hehe flag:"<<flag;
    request.setUrl(QUrl("http://119.23.138.209:8080/cloud/AcquireCode.do"));
    QByteArray postData;
    postData.append("emp_phone=");//参数
    postData.append(ui->userLineEdit->text());//参数
    QNetworkReply* reply = m_accessManagerRegister->post(request,postData);//发送http的post请求
}

//http 返回响应
void registerDialog::finishedSlot(QNetworkReply *reply)
{




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
        if(flag==3){
               qDebug()<<"判断昵称是否注册";
               if(content == "available"){
                   //成功，发送注册信息。
                   //http 判断昵称是否注册
                   QNetworkRequest request;
                   //request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data");
                   flag = 2; //步骤：注册
                   qDebug()<<"hehe flag:"<<flag;
                   request.setUrl(QUrl("http://119.23.138.209:8080/cloud/Register/Employee.do"));  //登录
                   QByteArray postData;
                   //emp_password  emp_email   code
                   postData.append("emp_phone=");//参数手机
                   postData.append(ui->userLineEdit->text());
                   postData.append("&emp_password=");//参数密码
                   postData.append(ui->passwardLineEdit->text());//
                   postData.append("&emp_email=");//参数邮箱
                   postData.append(ui->email->text());//
                   postData.append("&code=");//参数验证码
                   postData.append(ui->code->text());//
                   postData.append("&emp_name=");//昵称
                   postData.append(ui->nichengLineEdit->text());//
                   QNetworkReply* reply = m_accessManagerRegister->post(request,postData);//发送http的post请求
               }else{
                    ui->nichengAlert->setVisible(true);
               }
               return;

           }
           else if(flag==2){ //注册
               qDebug()<<"注册";
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
                                    ui->codeAlert->setVisible(true);


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
               qDebug()<<"获取验证码";
               if(content.contains("success",Qt::CaseSensitive))
               {
                   QMessageBox::information(this,"警告","验证码已发送！",QMessageBox::Ok);
               }else{
                    QMessageBox::information(this,"警告","验证码发送失败！",QMessageBox::Ok);
               }
               return;

           }else if(flag!=1){
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
                                   QMessageBox::information(this,"警告","注册成功",QMessageBox::Ok);
                               }
                           }

                       }
                   }
               }
           }
        else if(flag==1){
            qDebug()<<"判断手机号是否注册";

            if(content == "available"){

                QNetworkRequest request_2;
                flag = 3;
                request_2.setUrl(QUrl("http://119.23.138.209:8080/cloud/Employee/Judge_Name.do"));

                QByteArray postData_2;
                postData_2.append("emp_name=");
                postData_2.append(ui->nichengLineEdit->text());
                QNetworkReply* reply_2 = m_accessManagerRegister->post(request_2,postData_2);
            }else{
                QMessageBox::information(this,"警告","手机号已注册！",QMessageBox::Ok);
            }
            return;
           }
           flag = 0;


       }
       else
       {

           qDebug()<<"handle errors here";
           QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
           //statusCodeV是HTTP服务器的相应码
           qDebug( "found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
           qDebug(qPrintable(reply->errorString()));
       }
       reply->deleteLater();
}

void registerDialog::on_closeBtn_clicked()
{
    this->close();        //窗口关闭
}

void registerDialog::mousePressEvent(QMouseEvent *event) //窗口鼠标按下事件
{
    this->windowPos = this->pos();                // 获得部件当前位置
    this->mousePos = event->globalPos();     // 获得鼠标位置
    this->dPos = mousePos - windowPos;
}

void registerDialog::mouseMoveEvent(QMouseEvent *event)  //窗口鼠标移动事件
{
     this->move(event->globalPos() - this->dPos);
}

