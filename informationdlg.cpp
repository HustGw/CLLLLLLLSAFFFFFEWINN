#include "informationdlg.h"
QString infor_User_ID = "";


void informationDlg::setUserID(QString user_id){
    infor_User_ID = user_id;
    qDebug()<<infor_User_ID;
    setItem();//调用布局函数
    ItemWidget->setLayout(vbox);
    scrollArea = new QScrollArea();
    scrollLayout = new QVBoxLayout();
    scrollArea->setWidget(ItemWidget);
    scrollLayout->addWidget(scrollArea);
    this->setLayout(scrollLayout);
}
informationDlg::informationDlg(QWidget *parent):QDialog(parent)
{
    db = ConnectionPool::openConnection();
    setWindowTitle(tr("消息"));
    vbox = new QVBoxLayout();
    ItemWidget = new QWidget();

}

void informationDlg::recvReq(){
    QPushButton *pt = qobject_cast<QPushButton *>(sender());
    QSqlQuery query(db);
    bool success = query.exec("select * from Decryption where emp_id='"+infor_User_ID+"'");
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
                    InformationItem *q1 = this->findChild<InformationItem *>(id+"information");
                    delete q1;
                    //删除成功重新布局
                    delete this->layout();
                    QWidget *newItemWidget = new QWidget();
                    newItemWidget->setLayout(this->vbox);
                    scrollArea->setWidget(newItemWidget);
                    QVBoxLayout *newVbox = new QVBoxLayout();
                    newVbox->addWidget(scrollArea);
                    this->setLayout(newVbox);
                     QMessageBox::warning(this,tr("Success"),tr("成功同意"),QMessageBox::Yes);
                }
                else{
                     QMessageBox::warning(this,tr("error"),tr("操作失败!"),QMessageBox::Yes);
                }
            }
        }
    }


}
void informationDlg::ignoreReq(){
    QPushButton *pt = qobject_cast<QPushButton *>(sender());
    QSqlQuery query(db);
    bool success = query.exec("select * from Decryption where emp_id='"+infor_User_ID+"'");
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
                    delete q1;
                    //删除成功重新布局
                    delete this->layout();
                    QWidget *newItemWidget = new QWidget();
                    newItemWidget->setLayout(this->vbox);
                    scrollArea->setWidget(newItemWidget);
                    QVBoxLayout *newVbox = new QVBoxLayout();
                    newVbox->addWidget(scrollArea);
                    this->setLayout(newVbox);
                     QMessageBox::warning(this,tr("Success"),tr("成功忽略"),QMessageBox::Yes);
                }
                else{
                     QMessageBox::warning(this,tr("error"),tr("操作失败!"),QMessageBox::Yes);
                }
            }
        }
    }

}
void informationDlg::newInformation(){
    qDebug()<<"有新消息";
    this->vbox = new QVBoxLayout();
    setItem();
    delete this->layout();
    QWidget *newItemWidget = new QWidget();
    newItemWidget->setLayout(this->vbox);
    scrollArea->setWidget(newItemWidget);
    QVBoxLayout *newVbox = new QVBoxLayout();
    newVbox->addWidget(scrollArea);
    this->setLayout(newVbox);

}

void informationDlg::setItem(){
    QSqlQuery query(db);
    QString sql = "select * from Decryption where emp_id = '"+infor_User_ID+"' and status = '2'";
    qDebug()<<sql;
    bool success = query.exec("select * from Decryption where emp_id = '"+infor_User_ID+"' and status = '2'");
    if(success){
        qDebug()<<"查询数据库成功";
        while (query.next()) {
            InformationItem *m1 = new InformationItem();
            QString oemp_id= query.record().value("oemp_id").toString();
            qDebug()<<oemp_id;
            //预留 ：找出用户昵称
            QString filename = query.record().value("file_name").toString();
            QString title = oemp_id+"请求文件"+filename+"密钥文件下载";
            qDebug()<<title;
            m1->titleLabel->setText(title);
            m1->setObjectName(query.record().value("id").toString()+"information");
            m1->allowBtn->setObjectName(query.record().value("id").toString()+"inforAllowBtn");
            m1->ignoreBtn->setObjectName(query.record().value("id").toString()+"inforIgnoreBtn");
            connect(m1->allowBtn,SIGNAL(clicked(bool)),this,SLOT(recvReq()));
            connect(m1->ignoreBtn,SIGNAL(clicked(bool)),this,SLOT(ignoreReq()));
            vbox->addWidget(m1);
        }
    }
    else{
        qDebug()<<"查询数据库失败";
    }
}
