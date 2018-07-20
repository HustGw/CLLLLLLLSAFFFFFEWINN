#include "finishdecryptionItem.h"

FinishDecryptionItem::FinishDecryptionItem(QWidget *parent): QWidget(parent){

    fileName = new QLabel(this);
    fileSize = new QLabel(this);
    fileDescription = new QLabel(this);

    openBtn = new QPushButton(this);
    pathOpenBtn = new QPushButton(this);
    deleteBtn = new QPushButton(this);

    fileIcon = new QLabel(this);
    checkBox = new QCheckBox(this);

    //location

    openBtn ->setText("打开\n文件");
    pathOpenBtn ->setText("路径\n打开");
    deleteBtn->setText("删除");

    fileName->setGeometry(200,10,100,30);
    fileSize->setGeometry(200,40,100,30);
    fileDescription->setGeometry(200,80,200,30);


    openBtn->setGeometry(480,20,50,80);
    pathOpenBtn->setGeometry(540,20,50,80);
    deleteBtn->setGeometry(600,20,50,80);

    checkBox->setGeometry(10,70,20,20);

    fileIcon->setGeometry(30,50,100,30);


}

void FinishDecryptionItem::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setPen(QColor(139,139,139));
    painter.drawLine(31,0,this->width()-1,0);
    painter.drawLine(31,0,31,this->height()-1);
    painter.drawLine(this->width()-1,0,this->width()-1,this->height()-1);
    painter.drawLine(31,this->height()-1,this->width()-1,this->height()-1);
}


void FinishDecryptionItem::on_openBtn_clicked(){
    QString openPath;
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString name = button->objectName();
    QSqlQuery query(db);
       bool success = query.exec("select * from Decryption where id=" + name);
       if(!success){
           QMessageBox::information(NULL, "warning", "未找到路径！");
           return;
       }else{
           openPath = QString("file:///C:/Users/Administrator/Desktop/1.txt") + query.record().value("file_address").toString();
       }
    QDesktopServices::openUrl(QUrl(openPath, QUrl::TolerantMode));

}

void FinishDecryptionItem::on_pathOpenBtn_clicked(){
    QString openPath;
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString name = button->objectName();
   QSqlQuery query(db);
       bool success = query.exec("select * from Decryption where id=" + name);
       if(!success){
           QMessageBox::information(NULL, "warning", "未找到路径！");
           return;
       }else{
           openPath = QString("file:///") + query.record().value("file_name").toString();
       }
    QDesktopServices::openUrl(QUrl(openPath, QUrl::TolerantMode));
}

