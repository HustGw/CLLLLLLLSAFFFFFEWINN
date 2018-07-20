#include "senddialog.h"
#include "ui_senddialog.h"
#include "mainwindow.h"
#include "finishencryptionitem.h"
extern QString file_id;
extern QString User_ID;

sendDialog::sendDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sendDialog)
{
    MainWindow *main_window = new MainWindow();
    connect(main_window,SIGNAL(sendUserId(QString)),this,SLOT(reciveUserId(QString)));
    db1 = ConnectionPool::openConnection();
    ui->setupUi(this);
    QSqlQuery query(db1);
    //QListWidget *friendList = new QListWidget(ui->listWidget);
    //friendList->setGeometry(0,0,251,351);
    //ui->listWidget->setGeometry(0,0,251,351);
    bool friendSelSuc = query.exec("select * from friend where user_id ='"+User_ID+"'");
    if(!friendSelSuc){
        qDebug()<<"查询好友失败";
        return;
    }
    else{
        qDebug()<<"查询好友成功";
        //将查询的好友插入视图中
        int count = 0;
        while(query.next()){
            QString nickName = query.record().value("friend_nickname").toString();
            QString friend_id  =  query.record().value("friend_id").toString();
            QCheckBox * b1 = new QCheckBox(nickName);
            QListWidgetItem* a1 = new QListWidgetItem();

            ui->listWidget->addItem(a1);
            ui->listWidget->setItemWidget(a1,b1);
//            friendList->addItem(a1);
//            friendList->setItemWidget(a1,b1);
            count++;
        }
    }

}

void sendDialog::getCheckedItems(){
    int count = ui->listWidget->count();
    qDebug()<<count;
    bool isChecked = false;
int count2 = ui->listWidget_2->count();
    for (int j = 0;j<count2;j++){
        QListWidgetItem * item = ui->listWidget_2->takeItem(0);
        delete item;
    }

    for (int i = 0;i<count;i++){

        QListWidgetItem * item = ui->listWidget->item(i);
        QWidget * widget = ui->listWidget->itemWidget(item);
        QCheckBox * box =(QCheckBox*) widget;
        QString nick_name = box->text();
        isChecked = box ->isChecked();
        qDebug()<<nick_name+"            "+isChecked;

        if(isChecked){
            ui->listWidget_2->insertItem(i,nick_name);
        }
    }

}

sendDialog::~sendDialog()
{
    delete ui;
}


void sendDialog::closeEvent(QCloseEvent *){
    ConnectionPool::closeConnection(db1);
}

void sendDialog::on_pushButton_clicked()
{

    getCheckedItems();

}

void sendDialog::on_pushButton_2_clicked()
{
    QString fileName;
    int count = ui->listWidget_2->count();
    qDebug()<<file_id;
    QSqlQuery query(db1);
    bool select_file_name = query.exec("select * from varticle where article_id ="+file_id);
    if(!select_file_name){
        qDebug()<<"bug!~!!!!!!";
    }else{
        while(query.next()){
            fileName = query.record().value("article_name").toString();
            for(int i = 0;i<count;i++){
                QListWidgetItem *item = ui->listWidget_2->item(i);
                QString nick_name = item->text();
                QString friend_id;
                QString friend_phone;
                query.exec("select * from friend where friend_nickname ='"+nick_name+"'");
                while(query.next()){
                     friend_id= query.record().value("friend_id").toString();
                }
                query.exec("select * from employee where emp_id  = '"+friend_id+"'");
                while(query.next()){
                     friend_phone = query.record().value("emp_phone").toString();
                }
                query.exec("select * from employee where emp_id  = '"+User_ID+"'");
                while(query.next()){
                     friend_phone = query.record().value("emp_phone").toString();
                }
                qDebug()<<nick_name+"---"+friend_id+"---"+friend_phone;
                query.prepare("insert into Decryption (id,file_id,file_name,emp_id,emp_phone,oemp_id,oemp_phone,status) values (?,?,?,?,9999999,?,?,0)");
                query.bindValue(0,QUuid::createUuid().toString());
                query.bindValue(1,file_id);
                query.bindValue(2,fileName);
                query.bindValue(3,User_ID);
               // query.bindValue(3,user_phone);
                query.bindValue(4,friend_id);
                query.bindValue(5,friend_phone);
                bool success = query.exec();
                if(success){
                    qDebug()<<"插入待解密表成功";
                }else{
                    qDebug()<<"插入失败";
                }
            }
        }
    }


}
void sendDialog::reciveData(QString data){
    file_id = data;
}
void sendDialog::reciveUserId(QString data){
    User_ID = data;
}

