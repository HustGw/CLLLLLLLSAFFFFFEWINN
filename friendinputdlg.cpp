#include "friendinputdlg.h"

friendInputDlg::friendInputDlg(QWidget *parent):QDialog(parent)
{
    db = ConnectionPool::openConnection();//打开数据库连接
    setWindowTitle(tr("好友添加"));
//    titleLabel = new QLabel(this);
//    titleLabel->setText(tr("请输入对方名称"));
    inputLineEdit = new QLineEdit(this);
//    titleLabel->setGeometry(10,10,150,40);
    inputLineEdit->setGeometry(30,30,300,30);
    inputLineEdit->setPlaceholderText(tr("请输入好友手机号/昵称/用户名"));
    okBtn = new QPushButton(this);
//    cancelBtn = new QPushButton(this);
    okBtn->setText(tr("查找"));
//    cancelBtn->setText(tr("取消"));
    okBtn->setGeometry(350,30,40,30);
//    cancelBtn->setGeometry(200,100,40,30);
    connect(okBtn,SIGNAL(clicked(bool)),this,SLOT(SearchFriend()));
//    connect(cancelBtn,SIGNAL(clicked(bool)),this,SLOT(cancel()));
 //   this->setFixedSize(this->width(),this->height());
    userHead = new QLabel(this);
    userName = new QLabel(this);
    userPhone = new QLabel(this);
    userTitle = new QLabel(this);
    userId = new QLabel(this);
    addFriendBtn = new QPushButton(this);
    cancelBtn = new QPushButton(this);
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
    QPainter painter(this);
    painter.setPen(QColor(139,139,139));
    painter.drawLine(0,70,this->width()-1,70);
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
                userHead->setGeometry(60,90,80,80);
                userHead->setStyleSheet("min-width:  80px;"
                                        "max-width:  80px;"
                                        "min-height: 80px;"
                                        "max-height: 80px;"

                                        "border-radius:40px;"
                                        "border-width: 0 0 0 0;"
                                        "border-image: url(:/new/src/head2) 0 0 0 0 stretch strectch;"
                                        );
                userHead->show();
                userName->setGeometry(150,90,50,30);
                userName->setText(nickname);
                userName->show();
                userPhone->setGeometry(200,90,80,30);
                userPhone->setText("111111111111");
                userPhone->show();
                userTitle->setGeometry(150,140,50,30);
                userTitle->setText("用户名");
                userTitle->show();
                userId->setGeometry(220,140,80,30);
                userId->setText("8462023464");
                userId->show();
                addFriendBtn->setGeometry(160,200,80,40);
                addFriendBtn->setText("添加好友");
                addFriendBtn->show();
                cancelBtn->setGeometry(240,200,50,40);
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
