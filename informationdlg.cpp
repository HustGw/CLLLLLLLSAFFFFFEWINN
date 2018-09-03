#include "informationdlg.h"
int count = 0;

informationDlg::informationDlg(QWidget *parent):QDialog(parent)
{
    setStyleSheet("QWidget{background-color: #FFFFFF;}");
    //设置标题栏隐藏
    topWidget = new QWidget(this);
    topWidget->setGeometry(6,6,540,37);
    topWidget->setStyleSheet("background-color:#EEF0F5");
    this->setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    topWidget->show();
    bottomWidget = new QWidget(this);
    bottomWidget->setGeometry(7,43,540,463);
    bottomWidget->setStyleSheet("background-color: #FFFFFF");
    titleText = new QLabel(topWidget);
    titleText->setGeometry(54,11,200,15);
    titleText->setFont(QFont("Timers",9,QFont::Bold));
    titleText->setStyleSheet("background-color: #EEF0F5;");
    titleText->setText(QStringLiteral("消息"));
    titleText->show();
//    titleLabel = new QLabel(topWidget);
//    titleLabel->setGeometry(0,0,width-100,50);
    titleIcon = new QLabel(topWidget);
    titleIcon->setGeometry(22,10,23,17);
    titleIcon->setStyleSheet("border-image: url(:/new/mainwindow/pictures/system_icon.png);"
                             "background-color: #EEF0F5;");
    titleIcon->show();
 //   titleLabel->setStyleSheet("QLabel{font-family :微软雅黑;font-size : 5em;color : rgb(255,255,255);background-color: #EEF0F5;}");
    closeBtn = new QPushButton(this);
    closeBtn->setGeometry(506, 12, 13, 13);
    closeBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/delete_button.png);background-color: #EEF0F5;}QPushButton:hover{border-image:url(:/new/mainwindow/pictures/delete_button_hover.png);background-color: #EEF0F5;}");
    closeBtn->setCursor(QCursor(Qt::PointingHandCursor));   
    connect(closeBtn,SIGNAL(clicked()),this,SLOT(closeBtn_press()));
    db = ConnectionPool::openConnection();
    setWindowTitle(tr("消息"));
    vbox = new QVBoxLayout();
    ItemWidget = new QWidget();
    setItem();//布局函数
    CleanStatusLabel = new QLabel(this);
    cleanInforBtn = new Mylabel(this);
    cleanInforBtn->setText("清空消息记录");
    cleanInforBtn->setStyleSheet("color:#3D6CFE");
    cleanInforBtn->setGeometry(6,6,100,50);
    cleanInforBtn->setCursor(QCursor(Qt::PointingHandCursor));
    cleanInforBtn->setAlignment(Qt::AlignCenter);
    connect(cleanInforBtn,SIGNAL(LabelClicked()),this,SLOT(CleanAllInfor()));
    CleanStatusLabel->setText("暂无消息！");
    CleanStatusLabel->setGeometry(256,186,100,40);
    if(count==0){
        CleanStatusLabel->show();
        ItemWidget->setLayout(vbox);
        scrollArea = new QScrollArea();
        //scrollArea->setFrameRect(0,50,width-20,this->height()-50);
        //scrollArea->setGeometry(QRect(0,50,width-20,this->height()-50));
        scrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
        scrollLayout = new QVBoxLayout();
       // scrollArea->setWidget(CleanStatusLabel);
       // scrollLayout->addWidget(titleLabel);
        scrollLayout->addWidget(scrollArea);
        scrollLayout->addWidget(cleanInforBtn);
        bottomWidget->setLayout(scrollLayout);
    }
    else{
        CleanStatusLabel->hide();
        ItemWidget->setLayout(vbox);
        scrollArea = new QScrollArea();
        scrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
        scrollLayout = new QVBoxLayout();
        scrollArea->setWidget(ItemWidget);
        scrollLayout->addWidget(scrollArea);
        scrollLayout->addWidget(cleanInforBtn);
        bottomWidget->setLayout(scrollLayout);
    }

}

void informationDlg::recvReq(){
    QPushButton *pt = qobject_cast<QPushButton *>(sender());
    QSqlQuery query(db);
    bool success = query.exec("select * from Decryption where emp_id='"+User_ID+"'");
    if(!success){
        qDebug()<<"查询失败";
        return;
    }
    else{
        while(query.next()){
            QString id = query.record().value("id").toString();
            QPushButton *b1 = this->findChild<QPushButton *>(id+"inforAllowBtn");
            if(!pt)
                return;
            if(pt==b1){
                qDebug()<<id;
                bool updataSuccess = query.exec("update Decryption set status = 3 where id = '"+id+"'");
                if(updataSuccess){
                    informationNum--;//发送数量减少信号
                    emit InforNumDecrease(); //发送信号通知InformationNum更新
                    InformationItem *q1 = this->findChild<InformationItem *>(id+"information");
                     q1->allowBtn->setEnabled(false);
                     q1->allowBtn->setText("已允许");
                     q1->ignoreBtn->hide();
                  //   QMessageBox::warning(this,tr("Success"),tr("成功同意"),QMessageBox::Yes);
                }
                else{
                    MsgBox *msgbox = new MsgBox(2,QStringLiteral("操作失败！"));
                    msgbox->exec();
                }
            }
        }
    }


}
void informationDlg::ignoreReq(){
    QPushButton *pt = qobject_cast<QPushButton *>(sender());
    QSqlQuery query(db);
    bool success = query.exec("select * from Decryption where emp_id='"+User_ID+"'");
    if(!success){
        qDebug()<<"查询失败";
        return;
    }
    else{
        while(query.next()){
            QString id = query.record().value("id").toString();
            QPushButton *b1 = this->findChild<QPushButton *>(id+"inforIgnoreBtn");
            if(!pt)
                return;
            if(pt==b1){
                bool updataSuccess = query.exec("update Decryption set status = 4 where id = '"+id+"'");
                if(updataSuccess){
                    InformationItem *q1 = this->findChild<InformationItem *>(id+"information");                   
                    informationNum--;//发送信号数量减少
                    emit InforNumDecrease(); //发送信号通知InformationNum更新
//                    delete q1;
//                    //删除成功重新布局
//                    delete this->layout();
//                    QWidget *newItemWidget = new QWidget();
//                    newItemWidget->setLayout(this->vbox);
//                    scrollArea->setWidget(newItemWidget);
//                    QVBoxLayout *newVbox = new QVBoxLayout();
//                    newVbox->addWidget(scrollArea);
//                    this->setLayout(newVbox);
//                     QMessageBox::warning(this,tr("Success"),tr("成功忽略"),QMessageBox::Yes);
                    q1->allowBtn->hide();
                    q1->ignoreBtn->setEnabled(false);
                    q1->ignoreBtn->setText("已忽略");
                }
                else{
                    MsgBox *msgbox = new MsgBox(2,QStringLiteral("操作失败！"));
                    msgbox->exec();
                }
            }
        }
    }

}
void informationDlg::newInformation(){
    CleanStatusLabel->hide();
    this->vbox = new QVBoxLayout();
    setItem();
    delete bottomWidget->layout();
    QWidget *newItemWidget = new QWidget();
    newItemWidget->setLayout(this->vbox);
    scrollArea->setWidget(newItemWidget);
    QVBoxLayout *newVbox = new QVBoxLayout();
    newVbox->addWidget(scrollArea);
    newVbox->addWidget(cleanInforBtn);
    bottomWidget->setLayout(newVbox);

}

void informationDlg::setItem(){
    QSqlQuery query(db);
    QString sql = "select * from Decryption where emp_id = '"+User_ID+"' and status = '2'";
    qDebug()<<sql;
    bool success = query.exec("select * from Decryption where emp_id = '"+User_ID+"' and is_solved = '0' and (status = '2' or status ='4' or status = '3' or status = '5') order by apply_time DESC");
    if(success){
        qDebug()<<"查询数据库成功";
        while (query.next()) {
            count++;
            InformationItem *m1 = new InformationItem();
            QString oemp_id= query.record().value("oemp_id").toString();
            qDebug()<<oemp_id;
            //找出用户昵称
            QSqlQuery nameQuery(db);
            QString nickName;
            bool nameSuccess = nameQuery.exec("select * from employee where emp_id = '"+oemp_id+"'");
            if(!nameSuccess){
                qDebug()<<"查询失败";
            }
            else{
                while(nameQuery.next()){
                    nickName = nameQuery.record().value("emp_name").toString();
                }
            }
            QString filename = query.record().value("file_name").toString();
            QString title = nickName+"请求文件"+filename+"密钥文件下载";
            m1->titleLabel->setText(title);
            //判断当前状态 给予不同的按钮状态
            QString time = query.record().value("apply_time").toString();
            m1->timeLabel->setText(time);
            QString nowStatus = query.record().value("status").toString();
            if(nowStatus=="2"){
                m1->setObjectName(query.record().value("id").toString()+"information");
                m1->allowBtn->setObjectName(query.record().value("id").toString()+"inforAllowBtn");
                m1->ignoreBtn->setObjectName(query.record().value("id").toString()+"inforIgnoreBtn");
                connect(m1->allowBtn,SIGNAL(clicked(bool)),this,SLOT(recvReq()));
                connect(m1->ignoreBtn,SIGNAL(clicked(bool)),this,SLOT(ignoreReq()));
                vbox->addWidget(m1);
            }
            if(nowStatus=="3"||nowStatus=="5"){
                m1->setObjectName(query.record().value("id").toString()+"information");
                m1->allowBtn->setObjectName(query.record().value("id").toString()+"inforAllowBtn");
                m1->ignoreBtn->setObjectName(query.record().value("id").toString()+"inforIgnoreBtn");
                m1->allowBtn->setText(QStringLiteral("已允许"));
                m1->ignoreBtn->hide();
                m1->allowBtn->setEnabled(false);
                vbox->addWidget(m1);
            }
            if(nowStatus =="4"){
                m1->setObjectName(query.record().value("id").toString()+"information");
                m1->allowBtn->setObjectName(query.record().value("id").toString()+"inforAllowBtn");
                m1->ignoreBtn->setObjectName(query.record().value("id").toString()+"inforIgnoreBtn");
                m1->allowBtn->hide();
                m1->ignoreBtn->setText(QStringLiteral("已忽略"));
                m1->ignoreBtn->setEnabled(false);
                vbox->addWidget(m1);
            }
        }
    }
    else{
        qDebug()<<"查询数据库失败";
    }
    QSqlQuery friendQuery(db);
    bool friendSuccess = friendQuery.exec("select * from friend where friend_id = '"+User_ID+"' and status = '0' and is_solved = '0' order by create_time DESC");
    if(!friendSuccess){
        qDebug()<<"初始化好友列表失败";
    }
    else{
        while(friendQuery.next()){
            count++;
            //将查询到好友申请添加到InforDlg中
            InformationItem *f1 = new InformationItem();
            f1->InforKindsLabel->setText("好友申请");
            QString userNickName = friendQuery.record().value("user_nickname").toString();
            QString t = userNickName+"添加您为好友";
            f1->titleLabel->setText(t);
            f1->timeLabel->setText(friendQuery.record().value("create_time").toString());
            f1->allowBtn->setText("允许");
            f1->ignoreBtn->setText("忽略");
            f1->setObjectName(friendQuery.record().value("id").toString()+"friendInfor");
            f1->allowBtn->setObjectName(friendQuery.record().value("id").toString()+"friendBtn");
            f1->ignoreBtn->setObjectName(friendQuery.record().value("id").toString()+"friendIgn");
            connect(f1->ignoreBtn,SIGNAL(clicked(bool)),this,SLOT(AddFriendIgnore()));
            connect(f1->allowBtn,SIGNAL(clicked(bool)),this,SLOT(AddFriendRequest()));
            vbox->addWidget(f1);
        }



    }
    //设置消息种类为文件传输
    QSqlQuery transforQuery(db);
    bool transforSuccess = transforQuery.exec("select * from Decryption where oemp_id = '"+User_ID+"' and apply_time<> '0' and is_solved = '0' order by apply_time DESC");
    if(!transforSuccess){
        qDebug()<<"查询消息种类失败";
    }
    else{
        while(transforQuery.next()){
            count++;
            //将查询到的传输消息添加到InforDlg中
            InformationItem *m1 = new InformationItem();
            QString m_id = transforQuery.record().value("emp_id").toString();
            QString name;
            QString fileName = transforQuery.record().value("file_name").toString();
            QString time = transforQuery.record().value("apply_time").toString();
            QSqlQuery nameQuery(db);
            bool nameSuccess = nameQuery.exec("select * from employee where emp_id = '"+m_id+"'");
            if(!nameSuccess){
                qDebug()<<"infordlg:查找用户名失败";
            }
            else{
                while(nameQuery.next()){
                    name = nameQuery.record().value("emp_name").toString();
                }
            }
            QString s = name+"传输文件"+fileName;
            m1->InforKindsLabel->setText("文件传输");
            m1->titleLabel->setText(s);
            m1->timeLabel->setText(time);
            m1->allowBtn->hide();
            m1->ignoreBtn->hide();
            vbox->addWidget(m1);
        }
    }

}

void informationDlg::CleanAllInfor(){
    qDebug()<<"cleanClicked";
    //清除所有
    CleanStatusLabel->show();
    QSqlQuery query(db);
    bool success = query.exec("update Decryption set is_solved = 1 where emp_id = '"+User_ID+"'");
    if(!success){
        qDebug()<<"update failed!";
    }
    bool upFriSuc = query.exec("update friend set is_solved = 1 where friend_nickname ='"+User_ID+"'");
    if(upFriSuc){
        qDebug()<<"update friend error";
    }
    informationNum=0;
    FriendRequestCount = 0;
    emit CleanInforNum();
    this->vbox = new QVBoxLayout();
    delete bottomWidget->layout();
    QWidget *newItemWidget = new QWidget();
    newItemWidget->setLayout(this->vbox);
    scrollArea->setWidget(newItemWidget);
    QVBoxLayout *newVbox = new QVBoxLayout();
    newVbox->addWidget(scrollArea);
    newVbox->addWidget(cleanInforBtn);
    bottomWidget->setLayout(newVbox);

}

void informationDlg::NewRequestRec(QString name, QString fileName,QString time){
     InformationItem *m1 = new InformationItem();
     CleanStatusLabel->hide();
     QString s = name+"传输文件"+fileName;
     m1->InforKindsLabel->setText("文件传输");
     m1->titleLabel->setText(s);
     m1->timeLabel->setText(time);
     m1->allowBtn->hide();
     m1->ignoreBtn->hide();
     vbox->addWidget(m1);
     delete bottomWidget->layout();
     QWidget *newItemWidget = new QWidget();
     newItemWidget->setLayout(this->vbox);
     scrollArea->setWidget(newItemWidget);
     QVBoxLayout *newVbox = new QVBoxLayout();
     newVbox->addWidget(scrollArea);
     newVbox->addWidget(cleanInforBtn);
     bottomWidget->setLayout(newVbox);


}

void informationDlg::NewFriend(){
    qDebug()<<"new friend";
    FriendCount++;//
    CleanStatusLabel->hide();
    QSqlQuery query(db);
    bool success = query.exec("select * from friend where friend_id = '"+User_ID+"' and status = '0'");
    if(!success){
        qDebug()<<"informationdlg:查询失败";
    }
    else{
        while(query.next()){
            int isNew = 1;
            QString sendId = query.record().value("user_id").toString();
            for(int i = 0;i<FriendArrayIndex;i++){
                if(sendId == FriendNickNameArray[i]){
                    isNew = 0;
                    break;
                }
            }
            if(isNew ==1){
                //如果是新的好友请求，则在InforDig中增加一个新的条目
                QString SendName = query.record().value("user_nickname").toString();
                FriendNickNameArray[FriendArrayIndex] = SendName;
                FriendArrayIndex++;
                InformationItem *m1 = new InformationItem();
                m1->InforKindsLabel->setText("好友申请");
                QString title = SendName+"添加您为好友";
                m1->titleLabel->setText(title);
                m1->ignoreBtn->setText("忽略");
                m1->allowBtn->setText("允许");
                m1->setObjectName(query.record().value("id").toString()+"friendInfor");
                m1->allowBtn->setObjectName(query.record().value("id").toString()+"friendBtn");
                m1->ignoreBtn->setObjectName(query.record().value("id").toString()+"friendIgn");
                connect(m1->allowBtn,SIGNAL(clicked(bool)),this,SLOT(AddFriendRequest()));
                connect(m1->ignoreBtn,SIGNAL(clicked(bool)),this,SLOT(AddFriendIgnore()));
                vbox->addWidget(m1);
                delete bottomWidget->layout();
                QWidget *newItemWidget = new QWidget();
                newItemWidget->setLayout(this->vbox);
                scrollArea->setWidget(newItemWidget);
                QVBoxLayout *newVbox = new QVBoxLayout();
                newVbox->addWidget(scrollArea);
                newVbox->addWidget(cleanInforBtn);
                bottomWidget->setLayout(newVbox);
            }
        }
    }


}

void informationDlg::AddFriendRequest(){
    QPushButton *pt = qobject_cast<QPushButton *>(sender());
    QSqlQuery query(db);
    bool success= query.exec("select * from friend where friend_id='"+User_ID+"'");
    if(!success){
        qDebug()<<"查询失败";
        return;
    }
    else{
        while (query.next()) {
            QString ID = query.record().value("id").toString();
            QPushButton *b1 = this->findChild<QPushButton *>(ID+"friendBtn");
            if(!pt)
                return;
            if(pt==b1){
                QString name = query.record().value("user_nickname").toString();
                emit addFriendToMain(name);
                QSqlQuery updateQuery(db);
                bool updatesuccess =updateQuery.exec("update friend set status = '1' where id = '"+ID+"'");
                if(!updatesuccess){
                    qDebug()<<"update friend failed";
                }
                FriendRequestCount--;
                FriendStatusNum--;
                b1->setText("已允许");
                b1->setEnabled(false);
            }
        }
    }
}

void informationDlg::closeBtn_press(){
    close();
}

void informationDlg::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton)
        {
            mMoving = true;
            mLastMousePosition = event->globalPos();
        }

}

void informationDlg::mouseMoveEvent(QMouseEvent *event){

    if( event->buttons().testFlag(Qt::LeftButton) && mMoving)
    {
        this->move(this->pos() + (event->globalPos() - mLastMousePosition));
        mLastMousePosition = event->globalPos();
    }

}

void informationDlg::mouseReleaseEvent(QMouseEvent *event){

    if(event->button() == Qt::LeftButton)
    {
        mMoving = false;
    }
}

void informationDlg::paintEvent(QPaintEvent *event){
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

void informationDlg::AddFriendIgnore(){
    QPushButton *pt = qobject_cast<QPushButton *>(sender());
    QSqlQuery query(db);
    bool success = query.exec("select * from friend where friend_id = '"+User_ID+"' and status = '0'");
    if(!success){
        qDebug()<<"inforDlg:查询失败";
        return;
    }
    else{
        while(query.next()){
            QString ID = query.record().value("id").toString();
            QPushButton *b1 = this->findChild<QPushButton *>(ID+"friendIgn");
            if(!pt)
                return;
            if(pt==b1){
                QSqlQuery updateQuery(db);
                bool updateSuccess = updateQuery.exec("update friend set status = '2' where id = '"+ID+"'");
                if(!updateSuccess){
                    qDebug()<<"update friend failed";
                }
                FriendRequestCount--;//消息数量减一
                FriendStatusNum--;
                b1->setText("已忽略");
                b1->setEnabled(false);
                    }
                }
        }
}
