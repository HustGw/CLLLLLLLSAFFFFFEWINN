#include "finishdecryptionItem.h"


FinishDecryptionItem::FinishDecryptionItem(QWidget *parent): QWidget(parent){

    label = new QLabel(this);
    label->setGeometry(0,0,34,16);
    label->setStyleSheet("QLabel{ border-image:url(:/new/mainwindow/pictures/finde_label.png); }");
    elseLabel = new QLabel(this);
    elseLabel->setGeometry(49,25,30,16);
    elseLabel->setStyleSheet("background-color:#91A7B9;color:white;");
    elseLabel->raise();
        elseLabel->setAlignment(Qt::AlignCenter);
    fileName = new QLabel(this);
    fileSize = new QLabel(this);
    fileDescription = new QLabel(this);

    fileName->setFont(QFont("Timers",11,QFont::Bold));
    fileSize->setFont(QFont("Timers",8));
    fileDescription->setFont(QFont("Timers",8));
    fileSize->setStyleSheet("color:#9999AB");
    fileDescription->setStyleSheet("color:#9999AB");

    openBtn = new QPushButton(this);
    pathOpenBtn = new QPushButton(this);
    deleteBtn = new QPushButton(this);

    fileIcon = new QLabel(this);
    checkBox = new QCheckBox(this);
    checkBox->setCursor(QCursor(Qt::PointingHandCursor));

    //location
    openBtn->setStyleSheet("QPushButton { border-image:url(:/new/mainwindow/pictures/open_button.png); }"
                                       "QPushButton:hover { border-image:url(:/new/mainwindow/pictures/open_button_hover.png); }");
    pathOpenBtn->setStyleSheet("QPushButton { border-image:url(:/new/mainwindow/pictures/pathopen_button.png); }"
                                       "QPushButton:hover { border-image:url(:/new/mainwindow/pictures/pathopen_button_hover.png); }");
    deleteBtn->setStyleSheet("QPushButton { border-image:url(:/new/mainwindow/pictures/delete_button.png); }"
                                       "QPushButton:hover { border-image:url(:/new/mainwindow/pictures/delete_button_hover.png); }");

    openBtn->setToolTip("打开文件");
    pathOpenBtn->setToolTip("打开路径");
    deleteBtn->setToolTip("删除条目");

    openBtn->setCursor(QCursor(Qt::PointingHandCursor));
    pathOpenBtn->setCursor(QCursor(Qt::PointingHandCursor));
    deleteBtn->setCursor(QCursor(Qt::PointingHandCursor));

    fileName->setGeometry(97,10,300,16);
    fileSize->setGeometry(97,28,300,11);
    fileDescription->setGeometry(97,41,400,12);

    openBtn->setGeometry(540,10,14,16);
    pathOpenBtn->setGeometry(575,11,15,14);
    deleteBtn->setGeometry(614,12,13,13);

    checkBox->setGeometry(15,25,13,13);
    checkBox->setStyleSheet("QCheckBox::indicator {width: 13px;height: 13px;}");

    fileIcon->setGeometry(45,10,38,46);

}

void FinishDecryptionItem::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setPen(QColor(139,139,139));
    painter.drawLine(0,this->height()-1,this->width()-1,this->height()-1);
}


void FinishDecryptionItem::on_openBtn_clicked(){
    QString openPath;
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString name = button->objectName();
    db = ConnectionPool::openConnection();
    QSqlQuery query(db);
       bool success = query.exec("select * from Decryption where file_id='" + name+"'");

       if(!success){
           MsgBox *msgbox = new MsgBox(2,QStringLiteral("未找到路径"));
           msgbox->exec();
           return;
       }else{
           while(query.next()){
           openPath = QString("file:///D:/CloundSafeWindows/file/") + query.record().value("file_name").toString();
           }
       }
    QDesktopServices::openUrl(QUrl(openPath, QUrl::TolerantMode));
    ConnectionPool::closeConnection(db);

}

void FinishDecryptionItem::on_pathOpenBtn_clicked(){
    QString openPath;
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString name = button->objectName();
    db = ConnectionPool::openConnection();
    QSqlQuery query(db);
       bool success = query.exec("select * from Decryption where file_id='" + name+"'");
       if(!success){
           MsgBox *msgbox = new MsgBox(2,QStringLiteral("未找到路径"));
           msgbox->exec();
           return;
       }else{
           while(query.next()){
           openPath = QString("file:///D:/CloundSafeWindows/file/");
           }
       }
    QDesktopServices::openUrl(QUrl(openPath, QUrl::TolerantMode));
    ConnectionPool::closeConnection(db);
}

