#include "resetdialog.h"
#include "ui_resetdialog.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
QNetworkAccessManager *m_accessManagerReset;

resetDialog::resetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::resetDialog)
{
    ui->setupUi(this);

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
   QString userName=ui->userLineEdit_3->text();  //获取对话框中用户名
   QString email = ui->email_3->text();
   QString code = ui->code_3->text();
   QString password=ui->passwordLineEdit->text();//获取对话框中密码
   QString password_2=ui->passwordLineEdit_2->text();
   if(userName=="" || password=="" || email=="" || code=="")//判断用户名、密码、邮箱、验证码是否为空，为空弹出警告
       QMessageBox::information(this,"警告","输入不能为空",QMessageBox::Ok);
   else if(password != password_2) //判断两次输入的密码是否不同，不同的话弹出警告
       QMessageBox::information(this,"警告","两次输入的密码不同",QMessageBox::Ok);
   else{
       QNetworkRequest request;
       flag = 1;
       qDebug()<<"hehe flag:"<<flag;
       request.setUrl(QUrl("http://119.23.138.209:8080/cloud/Employee/Judge_Phone.do"));  //判断手机是否注册

       QByteArray postData;
       postData.append("emp_phone=");//参数
       postData.append(ui->userLineEdit_3->text());//参数
//       postData.append("&emp_password=");//参数
//       postData.append(ui->passwordLineEdit->text());//参数
//       postData.append("&emp_email=");//参数
//       postData.append(ui->email_3->text());//参数
//       postData.append("&code=");//参数
//       postData.append(ui->code_3->text());//参数
       QNetworkReply* reply = m_accessManagerReset->post(request,postData);//发送http的post请求
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
    postData.append(ui->userLineEdit_3->text());//参数
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
                postData.append(ui->userLineEdit_3->text());//参数
                postData.append("&emp_password=");//参数
                postData.append(ui->passwordLineEdit->text());//参数
                postData.append("&emp_email=");//参数
                postData.append(ui->email_3->text());//参数
                postData.append("&code=");//参数
                postData.append(ui->code_3->text());//参数
                QNetworkReply* reply = m_accessManagerReset->post(request,postData);//发送http的post请求
            }else{
                QMessageBox::information(this,"警告","手机号未注册！",QMessageBox::Ok);
            }
            return;
         }
         else if(flag==2){
             qDebug()<<"判断是否重置成功";

             if(content.contains("success",Qt::CaseSensitive)){
                 QMessageBox::information(this,"警告","密码重置成功！",QMessageBox::Ok);
             }else{
                 QMessageBox::information(this,"警告","邮箱错误！",QMessageBox::Ok);
             }
         }
         else if(flag==3){
             qDebug()<<"获取验证码";

             if(content.contains("success",Qt::CaseSensitive)){
                 QMessageBox::information(this,"警告","验证码已发送！",QMessageBox::Ok);
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
