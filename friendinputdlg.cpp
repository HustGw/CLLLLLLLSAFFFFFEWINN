#include "friendinputdlg.h"
QNetworkAccessManager *f_accessManager;
QString ddd =nullptr;
QString qq_num = nullptr;
extern QStringList m_fontList;
friendInputDlg::friendInputDlg(QWidget *parent):QDialog(parent)
{
    QFont f_1;
    f_1.setFamily(m_fontList.at(0));
    f_1.setPixelSize(14);
    f_1.setWeight(QFont::Normal);
    QFont f_2;
    f_2.setFamily(m_fontList.at(0));
    f_2.setPixelSize(14);
    f_2.setWeight(QFont::Bold);
    this->setFont(f_1);
    setStyleSheet("QWidget{background-color: #FFFFFF;}");
    topWidget = new QWidget(this);
    topWidget->setGeometry(6,6,500,37);
    topWidget->setStyleSheet("background-color:#EEF0F5");
    topWidget->show();
    bottomWidget = new QWidget(this);
    bottomWidget->setGeometry(7,43,500,363);
    bottomWidget->setStyleSheet("background-color: #FFFFFF");
    this->setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    titleText = new QLabel(topWidget);
    titleText->setGeometry(54,14,200,15);
    titleText->setFont(f_2);
    titleText->setStyleSheet("background-color: #EEF0F5;");
    titleText->setText(QStringLiteral("好友添加"));
    titleText->show();
    titleIcon = new QLabel(topWidget);
    titleIcon->setGeometry(22,13,23,17);
    titleIcon->setStyleSheet("border-image: url(:/new/mainwindow/pictures/system_icon.png);"
                             "background-color: #EEF0F5;");
    titleIcon->show();
    closeBtn = new QPushButton(this);
    closeBtn->setGeometry(466, 12, 13, 13);
    closeBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/delete_button.png);background-color: #EEF0F5;}QPushButton:hover{border-image:url(:/new/mainwindow/pictures/delete_button_hover.png);background-color: #EEF0F5;}");
    closeBtn->setCursor(QCursor(Qt::PointingHandCursor));
    closeBtn->setFont(f_1);
    connect(closeBtn,SIGNAL(clicked()),this,SLOT(closeBtn_press()));
    db = ConnectionPool::openConnection();//打开数据库连接
    setWindowTitle(tr("好友添加"));
//    titleLabel = new QLabel(this);
//    titleLabel->setText(tr("请输入对方名称"));
    inputLineEdit = new QLineEdit(this);
//    titleLabel->setGeometry(10,10,150,40);
    inputLineEdit->setGeometry(30,60,300,30);
    inputLineEdit->setPlaceholderText(tr("请输入好友手机号/昵称/用户名"));
    inputLineEdit->setFont(f_1);
    okBtn = new QPushButton(this);
    okBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: rgb(119,146,183);}QPushButton:pressed{background-color: rgb(139,159,185);}");
//    cancelBtn = new QPushButton(this);
    okBtn->setText(QStringLiteral("查找"));
//    cancelBtn->setText(tr("取消"));
    okBtn->setGeometry(350,60,40,30);
    okBtn->setFont(f_1);
//    cancelBtn->setGeometry(200,100,40,30);
    connect(okBtn,SIGNAL(clicked(bool)),this,SLOT(SearchFriend()));
//    connect(cancelBtn,SIGNAL(clicked(bool)),this,SLOT(cancel()));
 //   this->setFixedSize(this->width(),this->height());
    userHead = new QLabel(this);
    userName = new QLabel(this);
//    userPhone = new QLabel(this);
//    userTitle = new QLabel(this);
//    userId = new QLabel(this);
    userHead->hide();
    userName->hide();
//    userPhone->hide();
//    userId->hide();
//    userTitle->hide();
    addFriendBtn = new QPushButton(this);
    addFriendBtn->setFont(f_1);
    addFriendBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: rgb(119,146,183);}QPushButton:pressed{background-color: rgb(139,159,185);}");
    cancelBtn = new QPushButton(this);
    cancelBtn->setFont(f_1);
    cancelBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: rgb(119,146,183);}QPushButton:pressed{background-color: rgb(139,159,185);}");
    addFriendBtn->hide();
    cancelBtn->hide();
    connect(cancelBtn,SIGNAL(clicked(bool)),this,SLOT(cancel()));
    connect(addFriendBtn,SIGNAL(clicked(bool)),this,SLOT(sendName()));

    f_accessManager = new QNetworkAccessManager(this);
    connect(f_accessManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(finishedSlot(QNetworkReply*)));
}
void friendInputDlg::sendName(){
    qDebug()<<"send";
    if(qq_num==User_qqNum){
        qDebug()<<"添加自己为好友";
       MsgBox *msgbox = new MsgBox(2,QStringLiteral("不能添加自己为好友！"),this);
       msgbox->exec();
    }
    else{
        emit sendNameToMain(ddd);
    }
    this->accept();
    AddFriendFlag = 0;
    ConnectionPool::closeConnection(db);
}
void friendInputDlg::cancel(){
//    this->accept();
//    //取消关闭时断开数据库连接
    AddFriendFlag = 0;
//    ConnectionPool::closeConnection(db);
    userHead->hide();
    userName->hide();
    addFriendBtn->hide();
    cancelBtn->hide();


}

void friendInputDlg::paintEvent(QPaintEvent *event){
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(5, 5, this->width()-10, this->height()-10);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));
    QColor color(128, 128, 128, 50);
    for(int i=0; i<5; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(5-i, 5-i, this->width()-(5-i)*2, this->height()-(5-i)*2);
        color.setAlpha(120 - qSqrt(i)*50);
        painter.setPen(color);
        painter.drawPath(path);
    }
    QPainter painterline(this);
    painterline.setPen(QColor(139,139,139));
    painterline.drawLine(0,70,this->width()-1,70);
}

void friendInputDlg::ShowDetail(){

}
void friendInputDlg::SearchFriend(){
    //点击搜索按钮响应

    QSqlQuery query(db);
    QString name = inputLineEdit->text();
    if(!name.isEmpty()){
    QNetworkRequest request;
    request.setUrl(QUrl("http://www.yunjiami1.com/cloud/Employee/FindUser.do"));
    QByteArray postData;
    postData.append("data=");//请求参数名
    postData.append(name);
    QNetworkReply *reply = f_accessManager->post(request,postData);
//    bool success = query.exec("select * from employee where emp_name='"+name+"'");
//    if(!success){
//        qDebug()<<"查询好友失败";
//    }
//    else{
//        if(query.size()==0){
//            MsgBox *msgbox = new MsgBox(2,QStringLiteral("好友不存在"),this);
//            msgbox->exec();
//        }
//        else{
//            while(query.next()){
//                QString nickname = query.record().value("emp_name").toString();
//                userHead->setGeometry(60,110,80,80);
//                userHead->setStyleSheet("min-width:  80px;"
//                                        "max-width:  80px;"
//                                        "min-height: 80px;"
//                                        "max-height: 80px;"

//                                        "border-radius:40px;"
//                                        "border-width: 0 0 0 0;"
//                                        "border-image: url(:/new/src/head2) 0 0 0 0 stretch strectch;"
//                                        );
//                userHead->show();
//                userName->setGeometry(150,110,80,30);
//                userName->setText(nickname);
//                userName->show();
//                userPhone->setGeometry(240,110,80,30);
//                userPhone->setText("111111111111");
//                userPhone->show();
//                userTitle->setGeometry(150,160,50,30);
//                userTitle->setText("用户名");
//                userTitle->show();
//                userId->setGeometry(220,160,80,30);
//                userId->setText("8462023464");
//                userId->show();
//                addFriendBtn->setGeometry(180,220,80,40);
//                addFriendBtn->setText("添加好友");
//                addFriendBtn->show();
//                cancelBtn->setGeometry(280,220,50,40);
//                cancelBtn->setText("取消");
//                cancelBtn->show();

//            }
//        }

//    }

    }else{
        MsgBox *msgbox = new MsgBox(2,QStringLiteral("请输入好友ID！"),this);
        msgbox->exec();
    }

}
void friendInputDlg::closeEvent(QCloseEvent *event){
    qDebug()<<"点击关闭按钮";
    AddFriendFlag = 0;
    ConnectionPool::closeConnection(db);

}

void friendInputDlg::closeBtn_press(){
    close();
    AddFriendFlag = 0;
    ConnectionPool::closeConnection(db);
}

void friendInputDlg::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton)
        {
            mMoving = true;
            mLastMousePosition = event->globalPos();
        }
}

void friendInputDlg::mouseMoveEvent(QMouseEvent *event){
    if( event->buttons().testFlag(Qt::LeftButton) && mMoving)
    {
        this->move(this->pos() + (event->globalPos() - mLastMousePosition));
        mLastMousePosition = event->globalPos();
    }
}

void friendInputDlg::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton)
    {
        mMoving = false;
    }
}

void friendInputDlg::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
    case Qt::Key_Return:SearchFriend();break;
    case Qt::Key_Enter:SearchFriend();break;
    default:
        break;
    }
}
void friendInputDlg::finishedSlot(QNetworkReply *reply){
    if(reply->error()==QNetworkReply::NoError){
        QByteArray bytes = reply->readAll();
        qDebug()<<bytes;
        QJsonParseError jsonError;//Qt5新类
        QJsonDocument json = QJsonDocument::fromJson(bytes, &jsonError);//Qt5新类
        if(jsonError.error==QJsonParseError::NoError){
            if(json.isObject()){
                QJsonObject rootObj = json.object();
                QString rootpath;
                if(rootObj.contains("status")){
                    QJsonValue value = rootObj.value("status");
                    if(value.isString()){
                        rootpath = value.toString();
                        if(rootpath=="success"){
                            QJsonObject contentValue = rootObj.value("content").toObject();
                            QJsonValue CV = rootObj.value("content");
                            QJsonValue a = contentValue.take("emp_name");
                            QString output = a.toString();
                            qDebug()<<output;
                            output = CV.toString();
                            qDebug()<<output;
                            QString ename = "emp_name";
                            QString lastindex = "emp_password";
                            QString key = "qq_num";
                            int index = output.indexOf(ename);
                            int last = output.indexOf(lastindex);
                            int qq = output.indexOf(key);
                            qq +=9;
                            qq_num = output.mid(qq,9);
                            //last-=5;
                            qDebug()<<last;
                            index+=11;
                            qDebug()<<index;
                            int lenth = last-index-3;
                            ddd = output.mid(index,lenth);
                            qDebug()<<ddd;
                            qDebug()<<qq_num;
                            if(output==NULL){
                                //好友不存在
                                MsgBox *msgbox = new MsgBox(2,QStringLiteral("好友不存在"),this);
                                msgbox->exec();
                            }
                            else{
                                //好友存在 对output进行处理
                                userHead->setGeometry(60,110,80,80);
                                userHead->setStyleSheet("min-width:  80px;"
                                                        "max-width:  80px;"
                                                        "min-height: 80px;"
                                                        "max-height: 80px;"

                                                        "border-radius:40px;"
                                                        "border-width: 0 0 0 0;"
                                                        "border-image: url(:/new/src/head2) 0 0 0 0 stretch strectch;"
                                                        );
                                userHead->show();
                                userName->setGeometry(180,140,80,30);
                                userName->setText(ddd);
                                userName->show();
                               // userPhone->setGeometry(240,110,80,30);
                                //userPhone->setText("111111111111");
                                //userPhone->show();
//                                userTitle->setGeometry(150,160,50,30);
//                                userTitle->setText("用户名");
//                                userTitle->show();
//                                userId->setGeometry(220,160,80,30);
//                                userId->setText(qq_num);
//                                userId->show();
                                addFriendBtn->setGeometry(180,220,80,40);
                                addFriendBtn->setText("添加好友");
                                addFriendBtn->show();
                                cancelBtn->setGeometry(280,220,50,40);
                                cancelBtn->setText("取消");
                                cancelBtn->show();

                            }

                        }
                        else{
                            //查询失败
                            qDebug()<<"查询失败";

                        }
                    }
                }
            }
        }


    }
    else{
        qDebug()<<"handle errors here";
        QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        qDebug( "found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
        qDebug(qPrintable(reply->errorString()));

    }
    reply->deleteLater();
}


