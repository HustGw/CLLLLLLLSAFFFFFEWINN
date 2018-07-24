#include "finishencryptionItem.h"

QString file_id;
FinishEncryptionItem::FinishEncryptionItem(QWidget *parent): QWidget(parent){

    fileName = new QLabel(this);
    fileSize = new QLabel(this);
    fileDescription = new QLabel(this);

    transprotBtn = new QPushButton(this);
    shareBtn = new QPushButton(this);
    pathOpenBtn = new QPushButton(this);
    deleteBtn = new QPushButton(this);

    fileIcon = new QLabel(this);
    checkBox = new QCheckBox(this);

    //location

    transprotBtn ->setText("传输");
    shareBtn ->setText("分享");
    pathOpenBtn ->setText("路径\n打开");
    deleteBtn->setText("删除");

    fileName->setGeometry(200,10,100,30);
    fileSize->setGeometry(200,40,100,30);
    fileDescription->setGeometry(200,80,200,30);

    transprotBtn->setGeometry(420,20,50,80);
    shareBtn->setGeometry(480,20,50,80);
    pathOpenBtn->setGeometry(540,20,50,80);
    deleteBtn->setGeometry(600,20,50,80);

    checkBox->setGeometry(10,70,20,20);

    fileIcon->setGeometry(30,50,100,30);


}

void FinishEncryptionItem::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setPen(QColor(139,139,139));
    painter.drawLine(31,0,this->width()-1,0);
    painter.drawLine(31,0,31,this->height()-1);
    painter.drawLine(this->width()-1,0,this->width()-1,this->height()-1);
    painter.drawLine(31,this->height()-1,this->width()-1,this->height()-1);
}


void FinishEncryptionItem::on_transprotBtn_clicked(){

    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString name = button->objectName();
    file_id = name;
    dlg = new sendDialog();
    dlg->show();

}

void FinishEncryptionItem::on_shareBtn_clicked(){
    QString openPath = QString("file:///E:");
//    QString openPath = QString("file:///") + filePath;
    QDesktopServices::openUrl(QUrl(openPath, QUrl::TolerantMode));
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
           QMessageBox::information(NULL, "warning", "未找到路径！");
           return;
       }else{
           while(query.next()){
           openPath = QString("file:///C:/CloundSafe/encrypt/yZip");
           }
       }
        QDesktopServices::openUrl(QUrl(openPath, QUrl::TolerantMode));
}

