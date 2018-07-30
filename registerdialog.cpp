#include "registerdialog.h"
#include "ui_registerdialog.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
QNetworkAccessManager *m_accessManagerRegister;

registerDialog::registerDialog(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::registerDialog)
{
   ui->setupUi(this);

   flag = 0;

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
   //点击注册按钮的响应
   QString userName=ui->userLineEdit->text();  //获取对话框中用户名
   QString passward=ui->passwardLineEdit->text();//获取对话框中密码
   if(userName=="" || passward=="")//判断用户名、密码是否为空，为空弹出警告
       QMessageBox::information(this,"警告","输入不能为空",QMessageBox::Ok);


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

void registerDialog::on_codeBtn_clicked()
{
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
        if(flag==1){   //判断手机是否注册
            qDebug()<<"判断手机是否注册";

            if(content == "available")
            {
                //成功，发送注册信息。
                //http 判断手机号是否注册
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
                 QMessageBox::information(this,"警告","手机号已注册！",QMessageBox::Ok);
            }
            return;

        }else if(flag==2){ //注册
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
            qDebug()<<"获取验证码";
            if(content.contains("success",Qt::CaseSensitive))
            {
                QMessageBox::information(this,"警告","验证码已发送！",QMessageBox::Ok);
            }else{
                 QMessageBox::information(this,"警告","验证码发送失败！",QMessageBox::Ok);
            }
            return;

        }else{
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
