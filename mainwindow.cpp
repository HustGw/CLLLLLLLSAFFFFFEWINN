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
QString User_ID = "123";
QString URL = "119.23.162.138/cloud";
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    encryptionPage = new EncryptionItem();
    decryptionPage = new DecryptionItem();
    encryptionBtnItem = new EncryptionBtnView();
    decryptionBtnItem = new DecryptionBtnView();
    decryptionViewController = new DecryptionViewController();
    encryptionViewController = new EncryptionViewController();
    finishViewController = new FinishViewController();
    finishViewController2 = new FinishViewController2();
    finishEncryptionItem = new FinishEncryptionItem();
    finishDecryptionItem = new FinishDecryptionItem();

    finScrollArea = new QScrollArea();

    ui->MidStaWidget->addWidget(encryptionPage);
    ui->MidStaWidget->addWidget(encryptionViewController);
    ui->MidStaWidget->addWidget(decryptionViewController);
    ui->MidStaWidget->addWidget(finishViewController);
    ui->MidStaWidget->addWidget(finishViewController2);
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
    //使用连接池 管理数据库连接
     db = ConnectionPool::openConnection();
        //查询数据库  查询解密请求
     QSqlQuery query(db);

     bool success = query.exec("select * from Decryption where oemp_id="+User_ID+"");
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
                   //设置fileIcon的图片
                   QPixmap pixmap("icon.png");
                   pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                   v1->fileIcon->setScaledContents(true);
                   v1->fileIcon->setPixmap(pixmap);
                   v1->checkBox->setObjectName((query.record().value("file_id").toString())+"Decheck");//设置checkbox的ID
                   v1->setObjectName(query.record().value("file_id").toString()+"decryption");//设置Item的ID
                   v1->downloadBtn->setObjectName((query.record().value("file_id").toString())+"btn");//设置downloadBtn的ID
                   //连接信号槽
                   connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                   if(query.record().value("status").toString()=="0"){
                       v1->fileDescription->setText("主体文件指定分享需确认下载.");
                       v1->downloadBtn->setText("确认下载");
                       connect(v1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(OssDownLoadFile()));
                       decryptionViewController->vbox->addWidget(v1);//将v1添加到视图中
                   }
                   else if(query.record().value("status").toString()=="1"){
                       v1->fileDescription->setText("文件已加密需下载密钥文件.");
                       v1->downloadBtn->setText("申请解密");
                       connect(v1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(getFileID()));
                       decryptionViewController->vbox->addWidget(v1);//将v1添加到视图中
                   }

               }
               QWidget *newItemWidget = new QWidget();
               newScrollArea = new QScrollArea();
               newItemWidget->setLayout(decryptionViewController->vbox);
               newScrollArea->setWidget(newItemWidget);
               QVBoxLayout *newVbox = new QVBoxLayout();
               newVbox->addWidget(newScrollArea);
               decryptionViewController->setLayout(newVbox);
           //    return;
           }
     RequestRecThread *recThread = new RequestRecThread();
     connect(recThread,SIGNAL(numChanged()),this,SLOT(ReceiveNewReq()));
     recThread->start();

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
    ui->MidStaWidget->setCurrentWidget(finishViewController);
    ui->BtnStaWidget->setCurrentIndex(2);
}

void MainWindow::on_FinDepBtn_clicked()
{
    ui->MidStaWidget->setCurrentWidget(finishViewController2);
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
    QSqlQuery query(db);
    bool success = query.exec("select * from Decryption where oemp_id="+User_ID+"");
    if(!success){
        qDebug() << "查询user失败";
        return;
     }else{
              qDebug()<<"查询成功";
              //将数据库查到的数据添加到视图中
              while(query.next()){
                  QString fileID = query.record().value("file_id").toString();
                  QCheckBox *check = ui->MidStaWidget->findChild<QCheckBox*>(fileID+"Decheck");
                  if(check!=NULL){//判断是否找到相应控件
                      if(check->isChecked()){
                          QString deleteQuest = "delete from Decryption where file_id ="+fileID+"";
                          bool deleteSuccess = query.exec(deleteQuest);
                          if(!deleteSuccess){
                              qDebug()<<"delete failed";
                          }
                          else{
                              DecryptionItem *v1 = ui->MidStaWidget->findChild<DecryptionItem *>(fileID+"decryption");
                              delete v1;
                              qDebug()<<"delete success";
                          }

                      }

                  }
                  else{  //若没找到则继续查找
                      continue;
                  }

              }
              delete decryptionViewController->layout();
              QWidget *newItemWidget = new QWidget();
              newItemWidget->setLayout(decryptionViewController->vbox);
              newScrollArea->setWidget(newItemWidget);
              QVBoxLayout *newVbox = new QVBoxLayout();
              newVbox->addWidget(newScrollArea);
              decryptionViewController->setLayout(newVbox);
    }

}
//点击全选按钮 QCheckbox处于被选择状态
void MainWindow::on_pushButton_clicked()
{
   // QVBoxLayout *SelLayout = decryptionViewController->vbox;
    if(decryptionViewController->vbox){
        //遍历layout中的Item
        for(int i= 0;i<decryptionViewController->vbox->count();i++){


        }
    }
}

void MainWindow::getFileID(){
    QPushButton *pt = qobject_cast<QPushButton *>(sender());
    QSqlQuery query(db);
    bool success = query.exec("select * from Decryption where oemp_id="+User_ID+"");
    if(!success){
        qDebug() << "查询user失败";
        return;
     }else{

              while(query.next()){
                  QString fileID = query.record().value("file_id").toString();
                  QPushButton *b1 = ui->MidStaWidget->findChild<QPushButton*>(fileID+"btn");
                  if(!pt)
                      return;
                  if(pt==b1){
                      qDebug()<<fileID;
                      bool updateRequest = query.exec("update Decryption set status = 2 where file_id = "+fileID+"");
                      if(updateRequest){
                           QMessageBox::warning(this,tr("Success"),tr("申请成功请等待！"),QMessageBox::Yes);
                      }
                      else{
                           QMessageBox::warning(this,tr("error"),tr("申请失败!"),QMessageBox::Yes);
                      }
                  }
              }
        }

}

void MainWindow::OssDownLoadFile(){
    QDir *temp = new QDir;
    bool exist = temp->exists("D://CloundSafeWindows");
    if(exist){
        qDebug()<<"the dir has existed";
    }
    else {
        bool ojbk = temp->mkdir("D://CloundSafeWindows");//创建文件夹
        if(ojbk){
            qDebug()<<"the dir creat successfully!";
        }
    }
    //开始下载文件

    QMessageBox::warning(this,tr("Success"),tr("文件开始下载！"),QMessageBox::Yes);
}
//收到有新请求后 将原有视图清空后重新布局
void MainWindow::ReceiveNewReq(){
    qDebug()<<"MainWindow:recv!";
    QMessageBox::warning(this,tr("ATTTENTION!"),tr("有新请求!"),QMessageBox::Yes);
    QSqlQuery query(db);
    bool success = query.exec("select * from Decryption where oemp_id="+User_ID+"");
    if(!success){
        qDebug() << "查询user失败";
        return;
     }else{
              qDebug()<<"查询成功";
              //将数据库查到的数据添加到视图中
              decryptionViewController->vbox = new QVBoxLayout();//将原有视图清空
              while(query.next()){
                  DecryptionItem *v1 =  new DecryptionItem();
                  v1->fileName->setText(query.record().value("file_name").toString());//设置文件名
                  v1->fileSize->setText(query.record().value("file_size").toString());//设置文件大小
                  //设置fileIcon的图片
                  QPixmap pixmap("icon.png");
                  pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                  v1->fileIcon->setScaledContents(true);
                  v1->fileIcon->setPixmap(pixmap);
                  v1->checkBox->setObjectName((query.record().value("file_id").toString())+"Decheck");//设置checkbox的ID
                  v1->setObjectName(query.record().value("file_id").toString()+"decryption");//设置Item的ID
                  v1->downloadBtn->setObjectName((query.record().value("file_id").toString())+"btn");//设置downloadBtn的ID
                  //连接信号槽
                  connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                  if(query.record().value("status").toString()=="0"){
                      v1->fileDescription->setText("主体文件指定分享需确认下载.");
                      v1->downloadBtn->setText("确认下载");
                      connect(v1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(OssDownLoadFile()));
                      decryptionViewController->vbox->addWidget(v1);//将v1添加到视图中
                  }
                  else if(query.record().value("status").toString()=="1"){
                      v1->fileDescription->setText("文件已加密需下载密钥文件.");
                      v1->downloadBtn->setText("申请解密");
                      connect(v1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(getFileID()));
                      decryptionViewController->vbox->addWidget(v1);//将v1添加到视图中
                  }
              }
              delete decryptionViewController->layout();
              QWidget *newItemWidget = new QWidget();
              newItemWidget->setLayout(decryptionViewController->vbox);
              newScrollArea->setWidget(newItemWidget);
              QVBoxLayout *newVbox = new QVBoxLayout();
              newVbox->addWidget(newScrollArea);
              decryptionViewController->setLayout(newVbox);
    }
}


void MainWindow::on_pushButton_4_clicked()
{
    DecryptionItem *v1 = new DecryptionItem();
    v1->fileName->setText(tr("这是测试"));
    v1->fileSize->setText(tr("222tB"));
    v1->fileDescription->setText(tr("描述啊啊啊啊啊啊"));
    v1->fileIcon->setText(tr("图片片"));
    connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
    decryptionViewController->vbox->addWidget(v1);
    delete decryptionViewController->layout();
    QWidget *newItemWidget = new QWidget();
    QScrollArea *newScrollArea = new QScrollArea();
    newItemWidget->setLayout(decryptionViewController->vbox);
    newScrollArea->setWidget(newItemWidget);
    QVBoxLayout *newVbox = new QVBoxLayout();
    newVbox->addWidget(newScrollArea);
    decryptionViewController->setLayout(newVbox);
}

//已加密文件页面刷新按钮
void MainWindow::on_pushButton_8_clicked()
{
    QString file_id;
    QString file_name;
    QString file_size;
    QString file_discryption;
    QString file_status;

  //  connect(ui->pushButton_8,SIGNAL(clicked()),connectOdbc,SLOT(connectodbc()));

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

    QSqlQuery query;
       bool success = query.exec("select * from varticle where emp_id="+User_ID);
       if(!success){
           qDebug() << "查询密文失败";
           return;
       }else{
           qDebug()<<"查询成功";

           while(query.next())
           {
               file_name = query.record().value("article_name").toString();
               file_size = query.record().value("article_size").toString();
               file_status = query.record().value("article_status").toString();
               file_id = query.record().value("article_id").toString();

               if(file_status == "0"){
                   file_discryption = "文件未上传，请上传。";


               }else if(file_status == "1"){
                   file_discryption = "文件已上传，可传输分享。";
               }

               FinishEncryptionItem *f2 = ui->MidStaWidget->findChild<FinishEncryptionItem*>(file_id);

               delete f2;

               FinishEncryptionItem *f1 = new FinishEncryptionItem();
               f1->setObjectName(file_id);

               f1->checkBox->setObjectName(file_id + "check");
               f1->pathOpenBtn->setObjectName(file_id);
               f1->transprotBtn->setObjectName(file_id + "trans");
               f1->shareBtn->setObjectName(file_id + "share");
               f1->deleteBtn->setObjectName(file_id);

               f1->fileName->setText(file_name);
               f1->fileSize->setText(file_size);
               f1->fileDescription->setText(file_discryption);

               finishViewController->vbox->addWidget(f1);
               delete finishViewController->layout();
               QWidget *newItemWidget = new QWidget();
               //QScrollArea *newScrollArea = new QScrollArea();
               newItemWidget->setLayout(finishViewController->vbox);
               finScrollArea->setWidget(newItemWidget);
               QVBoxLayout *newVbox = new QVBoxLayout();
               newVbox->addWidget(finScrollArea);
               finishViewController->setLayout(newVbox);

               connect(f1->pathOpenBtn,SIGNAL(clicked(bool)),f1,SLOT(on_pathOpenBtn_clicked()));
               connect(f1->deleteBtn,SIGNAL(clicked(bool)),this,SLOT(on_deleteBtn_clicked()));

               qDebug()<<"文件名称:"+query.record().value("article_name").toString();
               qDebug()<<"密文ID:"+file_id;
           }
           return;
       }

}

void MainWindow::on_pushButton_5_clicked()
{
    QString file_name;
    QString file_id;
    QString file_size;
    QString file_discryption;
    QString file_status;

     QSqlQuery query;
     int check_flag = 0;
        bool success = query.exec("select * from varticle where emp_id="+User_ID);
        if(!success){
            qDebug() << "查询密文失败";
            return;
        }else{
            qDebug()<<"查询成功";

            while(query.next())
            {

                file_name = query.record().value("article_name").toString();
                file_id = query.record().value("article_id").toString();
                file_status = query.record().value("article_status").toString();
                file_size = query.record().value("article_size").toString();

                if(file_status == "0"){
                    file_discryption = "文件未上传，请上传。";
                }else if(file_status == "1"){
                    file_discryption = "文件已上传，可传输分享。";
                }

                QCheckBox *checkcheck = ui->MidStaWidget->findChild<QCheckBox*>(file_id+"check");
                if(checkcheck->isChecked()){
                    check_flag = 1;
                }

                FinishEncryptionItem *f2 = ui->MidStaWidget->findChild<FinishEncryptionItem*>(file_id);
                delete f2;

                delete finishViewController->layout();
                QWidget *newItemWidget = new QWidget();
                //QScrollArea *newScrollArea = new QScrollArea();
                newItemWidget->setLayout(finishViewController->vbox);
                finScrollArea->setWidget(newItemWidget);
                QVBoxLayout *newVbox = new QVBoxLayout();
                newVbox->addWidget(finScrollArea);
                finishViewController->setLayout(newVbox);

                if(check_flag == 0){
                    FinishEncryptionItem *f1 = new FinishEncryptionItem();
                    f1->setObjectName(file_id);

                    f1->checkBox->setObjectName(file_id + "check");

                    f1->pathOpenBtn->setObjectName(file_id);
                    f1->transprotBtn->setObjectName(file_id + "trans");
                    f1->shareBtn->setObjectName(file_id + "share");
                    f1->deleteBtn->setObjectName(file_id);

                    f1->fileName->setText(file_name);
                    f1->fileSize->setText(file_size);
                    f1->fileDescription->setText(file_discryption);

                    finishViewController->vbox->addWidget(f1);
                    delete finishViewController->layout();
                    QWidget *newItemWidget = new QWidget();
                    //QScrollArea *newScrollArea = new QScrollArea();
                    newItemWidget->setLayout(finishViewController->vbox);
                    finScrollArea->setWidget(newItemWidget);
                    QVBoxLayout *newVbox = new QVBoxLayout();
                    newVbox->addWidget(finScrollArea);
                    finishViewController->setLayout(newVbox);
                }
                check_flag = 0;
            }
        }
}

void MainWindow::on_deleteBtn_clicked(){
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString name = button->objectName();

    FinishEncryptionItem *f2 = ui->MidStaWidget->findChild<FinishEncryptionItem*>(name);
    delete f2;
    qDebug()<<name;

    delete finishViewController->layout();
    QWidget *newItemWidget = new QWidget();
   // QScrollArea *newScrollArea = new QScrollArea();
    newItemWidget->setLayout(finishViewController->vbox);
    finScrollArea->setWidget(newItemWidget);
    QVBoxLayout *newVbox = new QVBoxLayout();
    newVbox->addWidget(finScrollArea);
    finishViewController->setLayout(newVbox);
}

void MainWindow::on_deleteBtn2_clicked(){
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString name = button->objectName();

    FinishDecryptionItem *f2 = ui->MidStaWidget->findChild<FinishDecryptionItem*>(name);
    delete f2;
    qDebug()<<name;

    delete finishViewController2->layout();
    QWidget *newItemWidget = new QWidget();
   // QScrollArea *newScrollArea = new QScrollArea();
    newItemWidget->setLayout(finishViewController2->vbox);
    finScrollArea->setWidget(newItemWidget);
    QVBoxLayout *newVbox = new QVBoxLayout();
    newVbox->addWidget(finScrollArea);
    finishViewController2->setLayout(newVbox);
}

//已解密文件刷新按钮
void MainWindow::on_pushButton_9_clicked()
{

    QString file_id;
    QString file_name;
    QString file_size;
    QString file_discryption;

  //  connect(ui->pushButton_8,SIGNAL(clicked()),connectOdbc,SLOT(connectodbc()));

    QSqlQuery query;
       bool success = query.exec("select * from Decryption where status = 2 and oemp_id =" + User_ID);
       if(!success){
           qDebug() << "查询密文失败";
           return;
       }else{
           qDebug()<<"查询成功";

           while(query.next())
           {
               file_name = query.record().value("file_name").toString();
               file_size = query.record().value("article_size").toString();

               file_id = query.record().value("id").toString();

               file_discryption = "文件已成功解密。";

               FinishDecryptionItem *f2 = ui->MidStaWidget->findChild<FinishDecryptionItem*>(file_id);

               delete f2;

               FinishDecryptionItem *f1 = new FinishDecryptionItem();
               f1->setObjectName(file_id);

               f1->checkBox->setObjectName(file_id + "check");
               f1->pathOpenBtn->setObjectName(file_id);
               f1->openBtn->setObjectName(file_id);
               f1->deleteBtn->setObjectName(file_id);

               f1->fileName->setText(file_name);
               f1->fileSize->setText(file_size);
               f1->fileDescription->setText(file_discryption);

               finishViewController2->vbox->addWidget(f1);

               delete finishViewController2->layout();
               QWidget *newItemWidget = new QWidget();
              // QScrollArea *newScrollArea = new QScrollArea();
               newItemWidget->setLayout(finishViewController2->vbox);
               finScrollArea->setWidget(newItemWidget);
               QVBoxLayout *newVbox = new QVBoxLayout();
               newVbox->addWidget(finScrollArea);
               finishViewController2->setLayout(newVbox);

               connect(f1->pathOpenBtn,SIGNAL(clicked(bool)),f1,SLOT(on_pathOpenBtn_clicked()));
               connect(f1->openBtn,SIGNAL(clicked(bool)),f1,SLOT(on_openBtn_clicked()));
               connect(f1->deleteBtn,SIGNAL(clicked(bool)),this,SLOT(on_deleteBtn2_clicked()));

                qDebug()<<file_name;
           }
           return;
       }

}
