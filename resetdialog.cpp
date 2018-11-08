#include "resetdialog.h"
#include "ui_resetdialog.h"
#include "msgbox2.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QMouseEvent>
#include <QRegExpValidator>
#include <Qvalidator>
#include <QTimer>

#include <QGraphicsDropShadowEffect>

QStringList m_fontList2;
QFont f2("冬青黑体简体",9,75);
QFont m2;
QFont f_h2("冬青黑体简体",10,60);
QFont q2;
QNetworkAccessManager *m_accessManagerReset;

resetDialog::resetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::resetDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    QString dir = QCoreApplication::applicationDirPath();
    m_fontList2.clear();

    int lcdFontId = QFontDatabase::addApplicationFont(":/pictures/W3.ttf"); // 从source资源文件
    // int lcdFontId = QFontDatabase::addApplicationFont(dir + "/fonts/DS-DIGI.ttf"); //从外部资源文件
    if (lcdFontId != -1) // -1为加载失败
    {
        m_fontList2 << QFontDatabase::applicationFontFamilies(lcdFontId);
    }
    m2.setFamily(m_fontList2.at(0));
    f2.setFamily(m_fontList2.at(0));
    f_h2.setFamily(m_fontList2.at(0));
    q2.setFamily(m_fontList2.at(0));
    m2.setPixelSize(14);
    f2.setPixelSize(14);
    f_h2.setPixelSize(30);
    q2.setPixelSize(12);
    m2.setWeight(QFont::Normal);
    f2.setWeight(QFont::DemiBold);
    f_h2.setWeight(QFont::Bold);
    q2.setWeight(QFont::Normal);

    QGraphicsDropShadowEffect *effect= new QGraphicsDropShadowEffect;
    effect->setOffset(0,0);
    effect->setColor(Qt::black);
    effect->setBlurRadius(15);
    this->setGraphicsEffect(effect);

    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    ui->passwordLineEdit_2->setEchoMode(QLineEdit::Password);
    ui->userLineEdit->setPlaceholderText("请输入手机号");
    ui->passwordLineEdit->setPlaceholderText("设置你的登录密码");
    ui->passwordLineEdit_2->setPlaceholderText("请再次输入你的密码");

    ui->passwordLabel->setFont(m2);
    ui->passwordLabel_2->setFont(m2);
    ui->userLable->setFont(m2);
    ui->codeLabel->setFont(m2);
    ui->passwordAlert->setFont(m2);
    ui->userAlert->setFont(m2);
    ui->codeAlert->setFont(m2);
    ui->passwordLineEdit->setFont(q2);
    ui->passwordLineEdit_2->setFont(q2);
    ui->userLineEdit->setFont(q2);
    ui->code->setFont(q2);
    ui->codeBtn->setFont(m2);
    ui->icon_words->setFont(f2);
    ui->confirmBtn->setFont(f2);

    ui->userAlert->setVisible(false);
    ui->passwordAlert->setVisible(false);
    ui->codeAlert->setVisible(false);

    ui->confirmBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->codeBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->closeBtn->setCursor(QCursor(Qt::PointingHandCursor));

    ui->confirmBtn->setStyleSheet(                 //调整确定按钮样式
                "QPushButton{border-radius:4px;background-color: rgb(46, 130, 255);color:white;}"
                "QPushButton:hover,QPushButton:focus{border-radius:4px;background-color: rgb(85, 170, 255);color:white;}");
    ui->codeBtn->setStyleSheet(                 //调整验证码按钮样式
                "QPushButton{background-color: rgb(247, 247, 247);color:rgb(153,153,171);border:1px solid rgb(214,216,221);border-radius:4px;}"
                "QPushButton:hover,QPushButton:focus{background-color: rgb(247, 247, 247);color:rgb(46,130,255);border:1px solid rgb(46,130,255);border-radius:4px;}");
    ui->closeBtn->setStyleSheet(               //调整关闭按钮样式
                "QPushButton{border-image: url(:/new/mainwindow/pictures/delete_button.png);border:none;background-color:#EEF0F5;}"
                "QPushButton:hover,QPushButton:focus{border-image: url(:/new/mainwindow/pictures/delete_button_hover.png);border:none;background-color:#EEF0F5;}");
    ui->passwordLineEdit->setStyleSheet(                 //调整密码输入框样式
                "QLineEdit{border:1px solid rgb(214,216,221);border-radius:4px;}"
                "QLineEdit:hover,QLineEdit:focus{border:1px solid rgb(46,130,255);border-radius:4px;}");
    ui->passwordLineEdit_2->setStyleSheet(                 //调整确认密码输入框样式
                "QLineEdit{border:1px solid rgb(214,216,221);border-radius:4px;}"
                "QLineEdit:hover,QLineEdit:focus{border:1px solid rgb(46,130,255);border-radius:4px;}");
    ui->userLineEdit->setStyleSheet(                 //调整手机号输入框样式
                "QLineEdit{border:1px solid rgb(214,216,221);border-radius:4px;}"
                "QLineEdit:hover,QLineEdit:focus{border:1px solid rgb(46,130,255);border-radius:4px;}");
    ui->code->setStyleSheet(                 //调整验证码输入框样式
                "QLineEdit{border:1px solid rgb(214,216,221);border-radius:4px;}"
                "QLineEdit:hover,QLineEdit:focus{border:1px solid rgb(46,130,255);border-radius:4px;}");
    ui->widget->setObjectName("myWidget");
    ui->widget->setStyleSheet(
                              "QWidget#myWidget{border-bottom-left-radius:6px;border-bottom-right-radius:6px;background-color:white;}");
    ui->passwordLineEdit->setFocus();
    setTabOrder(ui->passwordLineEdit,ui->passwordLineEdit_2);
    setTabOrder(ui->passwordLineEdit_2,ui->userLineEdit);
    setTabOrder(ui->userLineEdit,ui->code);
    setTabOrder(ui->code,ui->codeBtn);
    setTabOrder(ui->codeBtn,ui->confirmBtn);

    flag = 0;
    c = 61;

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
    ui->passwordLineEdit->clearFocus();
    ui->confirmBtn->setEnabled(false);

    QString userName=ui->userLineEdit->text();  //获取对话框中用户名
    QString code = ui->code->text();
    QString password=ui->passwordLineEdit->text();//获取对话框中密码
    QString password_2=ui->passwordLineEdit_2->text();
    QRegExp rx("[1]\\d{10}");
    QRegExpValidator v(rx,0);
    int pos = 0;
    int flag_2 = 1;

   if(userName=="" || password=="" || code==""){//判断用户名、密码、验证码是否为空，为空弹出警告
       if(password != password_2){
           ui->passwordAlert->setVisible(true);  //两次密码不一样，出现提示行
           ui->confirmBtn->clearFocus();
       }
       if(userName != "" && QValidator::Acceptable!=v.validate(userName,pos)){
           ui->userAlert->setText("手机号码格式不正确");
           ui->userAlert->setVisible(true);
           ui->confirmBtn->clearFocus();
       }
       ui->confirmBtn->setEnabled(true);
       ui->passwordLineEdit->clearFocus();
       MsgBox2 *msgbox = new MsgBox2(3,QStringLiteral("输入不能为空！"),this);
       msgbox->exec();
   }else{
       if(password != password_2){
           ui->passwordLineEdit->clearFocus();
           ui->passwordAlert->setVisible(true);  //两次密码不一样，出现提示行
           ui->confirmBtn->clearFocus();
           ui->confirmBtn->setEnabled(true);
           flag_2 = 0;
       }
       if(QValidator::Acceptable!=v.validate(userName,pos)){
           ui->passwordLineEdit->clearFocus();
           ui->userAlert->setText("手机号码格式不正确");
           ui->userAlert->setVisible(true);
           ui->confirmBtn->clearFocus();
           ui->confirmBtn->setEnabled(true);
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
//       postData.append("&code=");//参数
//       postData.append(ui->code->text());//参数
       QNetworkReply* reply = m_accessManagerReset->post(request,postData);//发送http的post请求
       }
   }
}


void resetDialog::on_codeBtn_clicked(){
    ui->codeBtn->clearFocus();
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
                postData.append("");//参数
                postData.append("&code=");//参数
                postData.append(ui->code->text());//参数
                QNetworkReply* reply = m_accessManagerReset->post(request,postData);//发送http的post请求
            }else{
                ui->passwordLineEdit->clearFocus();
                ui->userAlert->setText("手机号未注册！");
                ui->userAlert->setVisible(true);
                ui->confirmBtn->setEnabled(true);
            }
            return;
         }
         else if(flag==2){ //重置密码
             qDebug()<<"重置密码";
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
                                  ui->passwordLineEdit->clearFocus();
                                  ui->codeAlert->setVisible(true);
                                  ui->confirmBtn->setEnabled(true);
                                  return;

                             }
                             else if(content.contains("invalid",Qt::CaseSensitive))
                             {
                                 //验证码错误。
                                  ui->passwordLineEdit->clearFocus();
                                  ui->codeAlert->setVisible(true);
                                  ui->confirmBtn->setEnabled(true);
                                  return;

                             }
                             else if(content.contains("success",Qt::CaseSensitive)){
                                 ui->codeAlert->setVisible(false);
                                 ui->passwordLineEdit->clearFocus();
                                 MsgBox2 *msgbox = new MsgBox2(4,QStringLiteral("密码重置成功！"),this);
                                 msgbox->exec();
                                 ui->confirmBtn->setEnabled(true);
                                 this->close();
                                 return;
                             }
                         }

                     }
                 }
             }
             return;
         }
//         else if(flag==2){
//             qDebug()<<"判断是否重置成功";

//             if(content.contains("success",Qt::CaseSensitive)){
//                 MsgBox2 *msgbox = new MsgBox2(4,QStringLiteral("密码重置成功！"),this);
//                 msgbox->exec();
//                 this->close();
//             }
//         }
         else if(flag==3){
             qDebug()<<"获取验证码";

             if(content.contains("success",Qt::CaseSensitive)){
                 c = 61;
                 timer2 = new QTimer(this);
                 connect(timer2,SIGNAL(timeout()),this,SLOT(showTimelimit()));
                 timer2->start(1000);

                 ui->passwordLineEdit->clearFocus();
                 MsgBox2 *msgbox = new MsgBox2(3,QStringLiteral("验证码已发送！"),this);
                 msgbox->exec();
             }else{
                 ui->passwordLineEdit->clearFocus();
                 MsgBox2 *msgbox = new MsgBox2(4,QStringLiteral("验证码发送失败！"),this);
                 msgbox->exec();
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
    }else{
        ui->passwordAlert->setVisible(false);
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
    }else if(password == ""){
        ui->passwordAlert->setVisible(false);
    }else if(!ui->passwordLineEdit_2->hasFocus()){
        ui->passwordAlert->setVisible(true);
    }
}

void resetDialog::on_passwordLineEdit_2_textChanged(){
    QString passward=ui->passwordLineEdit->text();  //获取对话框中密码
    QString passward_2=ui->passwordLineEdit_2->text();  //获取确认密码框中密码

    if(passward != ""){
        if(passward_2 == ""){
            ui->passwordAlert->setVisible(false);
        }else if(passward != passward_2){
            ui->passwordAlert->setVisible(true);
        }else{
            ui->passwordAlert->setVisible(false);
        }
    }else if(passward_2 == ""){
        ui->passwordAlert->setVisible(false);
    }else{
        ui->passwordAlert->setVisible(true);
    }
}

void resetDialog::on_passwordLineEdit_2_editingFinished(){

    QString passward=ui->passwordLineEdit->text();  //获取对话框中密码
    QString passward_2=ui->passwordLineEdit_2->text();  //获取确认密码框中密码

    if(passward == ""){
        if(passward_2 == ""){
            ui->passwordAlert->setVisible(false);
        }else{
            ui->passwordAlert->setVisible(true);
        }
    }else if(passward != passward_2){
        ui->passwordAlert->setVisible(true);
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

void resetDialog::showTimelimit(){
    if(c != 1){
        ui->codeBtn->setEnabled(false);
        c = c-1;
        QString num = QString::number(c);
        ui->codeBtn->setText(num + "秒后再次发送");
    }else{
        ui->codeBtn->setEnabled(true);
        ui->codeBtn->setText("重新发送验证码");
        timer2->stop();
    }
}
