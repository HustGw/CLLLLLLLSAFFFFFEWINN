#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QFont>
#include <connectsql.h>
#include <encryption.h>
#include <QLabel>
#include <QTextCodec>
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QtSql/QSqlError>
extern int isFinishedBtn=0;//用于判断是否已经点击
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    encryptionBtnItem = new EncryptionBtnView();
    decryptionBtnItem = new DecryptionBtnView();
    decryptionViewController = new DecryptionViewController();
    encryptionViewController = new EncryptionViewController();
    ui->MidStaWidget->addWidget(encryptionViewController);
    ui->MidStaWidget->addWidget(decryptionViewController);
    ui->nameLabel->setText(tr("新垣结衣"));
    QPixmap pixmap("head.jpg");
    pixmap.scaled(ui->headLabel->size(),Qt::KeepAspectRatio);
    ui->headLabel->setScaledContents(true);
    ui->headLabel->setPixmap(pixmap);
    ui->FinDepBtn->hide();
    ui->FinEnpBtn->hide();
    this->setFixedSize(this->width(),this->height());
    QFont font("Microsoft YaHei",10,75);
    this->setFont(font);
    //连接数据库ODBC
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
           db.setHostName("119.23.138.181");
           db.setPort(3306);
           db.setDatabaseName("Cloud_Encryption");
           db.setUserName("root");
           db.setPassword("F103-backup");
           bool ok = db.open();
        if(ok)
        {
            qDebug() << "connect MySql success!";
        }
        else // 打开数据库失败
        {
            qDebug() <<"error_MySql:\n" << db.lastError().text();
        }
        //查询数据库  查询解密请求
     QSqlQuery query;
     bool success = query.exec("select * from Decryption where oemp_id='123'");
     if(!success){
         qDebug() << "查询user失败";
         return;
      }else{
               qDebug()<<"查询成功";
               //将数据库查到的数据添加到视图中
               while(query.next())
               {
                   DecryptionItem *v1 =  new DecryptionItem();
                   v1->fileName->setText(query.record().value("file_name").toString());//设置文件名
                   v1->fileSize->setText(query.record().value("file_size").toString());//设置文件大小
                   if(query.record().value("status").toString()=="0"){
                       v1->fileDescription->setText("主体文件指定分享需确认下载.");
                       v1->downloadBtn->setText("确认下载");
                   }
                   else {
                       v1->fileDescription->setText("文件已加密需下载密钥文件.");
                       v1->downloadBtn->setText("申请解密");
                   }
                   connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                   decryptionViewController->vbox->addWidget(v1);

               }
          //     delete decryptionViewController->layout();
               QWidget *newItemWidget = new QWidget();
               QScrollArea *newScrollArea = new QScrollArea();
               newItemWidget->setLayout(decryptionViewController->vbox);
               newScrollArea->setWidget(newItemWidget);
               QVBoxLayout *newVbox = new QVBoxLayout();
               newVbox->addWidget(newScrollArea);
               decryptionViewController->setLayout(newVbox);
               return;
           }


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_FinishedBtn_clicked()
{
    //点击已加密判断按钮是否需要隐藏下方按钮
    if(isFinishedBtn==0){
        ui->FinDepBtn->show();
        ui->FinEnpBtn->show();
        isFinishedBtn=1;
    }
    else{
        ui->FinDepBtn->hide();
        ui->FinEnpBtn->hide();
        isFinishedBtn = 0;
    }

}

void MainWindow::on_DecryptionBtn_clicked()
{   //点击加密按钮后，QMidStaWidget跳转到0号子页面
    ui->MidStaWidget->setCurrentWidget(decryptionViewController);
    ui->BtnStaWidget->setCurrentIndex(1);
}



void MainWindow::on_EncryptionBtn_clicked()
{   //点击解密按钮后，MidStaWidget跳转到1号子页面
    ui->MidStaWidget->setCurrentWidget(encryptionViewController);
    ui->BtnStaWidget->setCurrentIndex(0);

}

void MainWindow::on_FinEnpBtn_clicked()
{
    ui->BtnStaWidget->setCurrentIndex(2);
}

void MainWindow::on_FinDepBtn_clicked()
{
    ui->BtnStaWidget->setCurrentIndex(3);
}

void MainWindow::on_OpenFileBtn_clicked()
{
    QTextCodec *codec = QTextCodec::codecForName("GB18030");
    QString file_full,fName,fPath;
    qint64 fSize;
    double mfSize;
    QFileInfo fInfo;
    file_full = QFileDialog::getOpenFileName(this,"Open File",QDir::currentPath());//打开文件选择
    fInfo=QFileInfo(file_full);
    fName=fInfo.fileName();
    fPath=fInfo.filePath();
    fSize=fInfo.size();
    EncryptionItem *v1 = new EncryptionItem();
    v1->fileName->setText(fName);
    mfSize=(double)(fSize/1024.);//字节转换为KB
    v1->fileSize->setText(QString::number( mfSize)+codec->toUnicode(" KB"));
    v1->fileIcon->setText(fName);
    v1->fileDescription->setText("111");
    encryptionViewController->vbox->addWidget(v1);
    delete encryptionViewController->layout();
    QWidget *newItemWidget = new QWidget();
    QScrollArea *newScrollArea = new QScrollArea();
    newItemWidget->setLayout(encryptionViewController->vbox);
    newScrollArea->setWidget(newItemWidget);
    QVBoxLayout *newVbox = new QVBoxLayout();
    newVbox->addWidget(newScrollArea);
    encryptionViewController->setLayout(newVbox);

    encryption *contest = new encryption();
     //连接数据库
    //contest->connect();
    contest->originalFileName =fName;
    contest->originalFilePath =fPath;
    contest->originalFileSize =mfSize;
    qDebug()<<contest->originalFilePath ;
    contest->encrypt();

}
//批量删除按钮
void MainWindow::on_pushButton_3_clicked()
{


}
//点击全选按钮 QCheckbox处于被选择状态
void MainWindow::on_pushButton_clicked()
{
}



void MainWindow::on_pushButton_4_clicked()
{

}
