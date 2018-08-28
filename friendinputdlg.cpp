#include "friendinputdlg.h"

friendInputDlg::friendInputDlg(QWidget *parent):QDialog(parent)
{
    setStyleSheet("QWidget{background-color: #FFFFFF;}");
    topWidget = new QWidget(this);
    topWidget->setGeometry(6,6,500,37);
    topWidget->setStyleSheet("background-color:#EEF0F5");
    topWidget->show();
    bottomWidget = new QWidget(this);
    bottomWidget->setGeometry(7,43,500,363);
    bottomWidget->setStyleSheet("background-color: #FFFFFF");
    this->setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    titleText = new QLabel(topWidget);
    titleText->setGeometry(54,14,200,15);
    titleText->setFont(QFont("Timers",9,QFont::Bold));
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
    connect(closeBtn,SIGNAL(clicked()),this,SLOT(closeBtn_press()));
    db = ConnectionPool::openConnection();//打开数据库连接
    setWindowTitle(tr("好友添加"));
//    titleLabel = new QLabel(this);
//    titleLabel->setText(tr("请输入对方名称"));
    inputLineEdit = new QLineEdit(this);
//    titleLabel->setGeometry(10,10,150,40);
    inputLineEdit->setGeometry(30,60,300,30);
    inputLineEdit->setPlaceholderText(tr("请输入好友手机号/昵称/用户名"));
    okBtn = new QPushButton(this);
    okBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: rgb(119,146,183);}QPushButton:pressed{background-color: rgb(139,159,185);}");
//    cancelBtn = new QPushButton(this);
    okBtn->setText(QStringLiteral("查找"));
//    cancelBtn->setText(tr("取消"));
    okBtn->setGeometry(350,60,40,30);
//    cancelBtn->setGeometry(200,100,40,30);
    connect(okBtn,SIGNAL(clicked(bool)),this,SLOT(SearchFriend()));
//    connect(cancelBtn,SIGNAL(clicked(bool)),this,SLOT(cancel()));
 //   this->setFixedSize(this->width(),this->height());
    userHead = new QLabel(this);
    userName = new QLabel(this);
    userPhone = new QLabel(this);
    userTitle = new QLabel(this);
    userId = new QLabel(this);
    userHead->hide();
    userName->hide();
    userPhone->hide();
    userId->hide();
    userTitle->hide();
    addFriendBtn = new QPushButton(this);
    addFriendBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: rgb(119,146,183);}QPushButton:pressed{background-color: rgb(139,159,185);}");
    cancelBtn = new QPushButton(this);
    cancelBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: rgb(119,146,183);}QPushButton:pressed{background-color: rgb(139,159,185);}");
    addFriendBtn->hide();
    cancelBtn->hide();
    connect(cancelBtn,SIGNAL(clicked(bool)),this,SLOT(cancel()));
    connect(addFriendBtn,SIGNAL(clicked(bool)),this,SLOT(sendName()));
}
void friendInputDlg::sendName(){
    QString name = inputLineEdit->text();
    qDebug()<<"send";
    emit sendNameToMain(name);
    this->accept();

}
void friendInputDlg::cancel(){
    this->accept();
    //取消关闭时断开数据库连接
    ConnectionPool::closeConnection(db);

}

void friendInputDlg::paintEvent(QPaintEvent *event){
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
    QPainter painterline(this);
    painterline.setPen(QColor(139,139,139));
    painterline.drawLine(0,70,this->width()-1,70);
}

void friendInputDlg::ShowDetail(){

}
void friendInputDlg::SearchFriend(){
    QSqlQuery query(db);
    QString name = inputLineEdit->text();
    bool success = query.exec("select * from employee where emp_name='"+name+"'");
    if(!success){
        qDebug()<<"查询好友失败";
    }
    else{
        if(query.size()==0){
            MsgBox *msgbox = new MsgBox(2,QStringLiteral("好友不存在"));
            msgbox->exec();
        }
        else{
            while(query.next()){
                QString nickname = query.record().value("emp_name").toString();
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
                userName->setGeometry(150,110,80,30);
                userName->setText(nickname);
                userName->show();
                userPhone->setGeometry(240,110,80,30);
                userPhone->setText("111111111111");
                userPhone->show();
                userTitle->setGeometry(150,160,50,30);
                userTitle->setText("用户名");
                userTitle->show();
                userId->setGeometry(220,160,80,30);
                userId->setText("8462023464");
                userId->show();
                addFriendBtn->setGeometry(180,220,80,40);
                addFriendBtn->setText("添加好友");
                addFriendBtn->show();
                cancelBtn->setGeometry(280,220,50,40);
                cancelBtn->setText("取消");
                cancelBtn->show();

            }
        }

    }



}
void friendInputDlg::closeEvent(QCloseEvent *event){
    qDebug()<<"点击关闭按钮";
    ConnectionPool::closeConnection(db);

}

void friendInputDlg::closeBtn_press(){
    close();
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
