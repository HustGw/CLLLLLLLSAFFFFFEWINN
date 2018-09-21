#include "finishencryptionItem.h"

QString file_id;
FinishEncryptionItem::FinishEncryptionItem(QWidget *parent): QWidget(parent){

    label = new QLabel(this);
    label->setGeometry(0,0,34,16);
    label->setStyleSheet("QLabel{ border-image:url(:/new/mainwindow/pictures/finen_label.png); }");
    elseLabel = new QLabel(this);
    elseLabel->setGeometry(49,25,30,16);
    elseLabel->setStyleSheet("QLabel{background-color:#91A7B9 ; color:white;}");
        elseLabel->setAlignment(Qt::AlignCenter);
    fileName = new QLabel(this);
    fileSize = new QLabel(this);
    fileDescription = new QLabel(this);

    fileName->setFont(QFont("Timers",11,QFont::Bold));
    fileSize->setFont(QFont("Timers",8));
    fileDescription->setFont(QFont("Timers",8));
    fileSize->setStyleSheet("color:#9999AB");
    fileDescription->setStyleSheet("color:#9999AB");

    transprotBtn = new QPushButton(this);
    shareBtn = new QPushButton(this);
    pathOpenBtn = new QPushButton(this);
    deleteBtn = new QPushButton(this);

    fileIcon = new QLabel(this);
    checkBox = new QCheckBox(this);
    checkBox->setCursor(QCursor(Qt::PointingHandCursor));
    //location
    transprotBtn->setStyleSheet("QPushButton { border-image:url(:/new/mainwindow/pictures/trans_button.png); }"
                                       "QPushButton:hover { border-image:url(:/new/mainwindow/pictures/trans_button_hover.png); }");
    shareBtn->setStyleSheet("QPushButton { border-image:url(:/new/mainwindow/pictures/share_button.png); }"
                                       "QPushButton:hover { border-image:url(:/new/mainwindow/pictures/share_button_hover.png); }");
    pathOpenBtn->setStyleSheet("QPushButton { border-image:url(:/new/mainwindow/pictures/pathopen_button.png); }"
                                       "QPushButton:hover { border-image:url(:/new/mainwindow/pictures/pathopen_button_hover.png); }");
    deleteBtn->setStyleSheet("QPushButton { border-image:url(:/new/mainwindow/pictures/delete_button.png); }"
                                       "QPushButton:hover { border-image:url(:/new/mainwindow/pictures/delete_button_hover.png); }");

    transprotBtn->setToolTip("传输");
    shareBtn->setToolTip("分享");
    pathOpenBtn->setToolTip("打开路径");
    deleteBtn->setToolTip("删除条目");
    timeLabel = new QLabel(this);
    timeLabel->setStyleSheet("background:transparent");
    timeLabel->setGeometry(294,31,230,10);
    timeLabel->setFont(QFont("Timers",8));
    timeLabel->setStyleSheet("color:#9999AB");
    transprotBtn->setCursor(QCursor(Qt::PointingHandCursor));
    shareBtn->setCursor(QCursor(Qt::PointingHandCursor));
    pathOpenBtn->setCursor(QCursor(Qt::PointingHandCursor));
    deleteBtn->setCursor(QCursor(Qt::PointingHandCursor));

    fileName->setGeometry(97,10,300,16);
    fileSize->setGeometry(97,28,300,11);
    fileDescription->setGeometry(97,41,400,12);

    transprotBtn->setGeometry(503,10,13,15);
    shareBtn->setGeometry(540,10,11,14);
    pathOpenBtn->setGeometry(575,10,15,14);
    deleteBtn->setGeometry(614,10,13,13);

    checkBox->setGeometry(15,25,13,13);
    checkBox->setStyleSheet("QCheckBox::indicator {width: 13px;height: 13px;}");

    fileIcon->setGeometry(45,10,38,46);

}

void FinishEncryptionItem::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setPen(QColor(139,139,139));
    painter.drawLine(0,this->height()-1,this->width()-1,this->height()-1);
}


void FinishEncryptionItem::on_transprotBtn_clicked(){

    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString name = button->objectName();
    file_id = name;
    dlg = new sendDialog();
    dlg->show();

}

void FinishEncryptionItem::on_shareBtn_clicked(){
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString name = button->objectName();
    file_id = name;
    share = new shareDialog();
    share->show();
}
void FinishEncryptionItem::on_pathOpenBtn_clicked(){
    QString openPath;
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString name = button->objectName();
    db = ConnectionPool::openConnection();
    qDebug()<<name;
    QSqlQuery query(db);
       bool success = query.exec("select * from varticle where article_id='" + name + "'");
       if(!success){
           MsgBox *msgbox = new MsgBox(2,QStringLiteral("未找到路径"),this);
           msgbox->exec();
           return;
       }else{
           while(query.next()){
           openPath = QString("file:///C:/CloundSafe/"+User_qqNum+"/encrypt/yZip/");
           }
       }
        QDesktopServices::openUrl(QUrl(openPath, QUrl::TolerantMode));
        ConnectionPool::closeConnection(db);
}

