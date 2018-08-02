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
#include "QProgressBar"

int informationNum = 0;
int RequsetAllowNum = 0;
int isFinishedBtn=0;//用于判断是否已经点击
QString User_ID = NULL;
QString URL = "119.23.162.138/cloud";
bool fileOpenFlag;
bool initLableFlag;
bool initPageFlag;
int decryptionFlag =0;
int threadNum = 0;
int DepThreadNum = 0;
QFont f("ZYSong18030",12,75);
QFileInfo openFileInfo;
QString orfileUuid;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    User_ID = LoginUserID;
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
    inforDlg = new informationDlg();
    CleanButtonClicked();
//    connect(this,SIGNAL(sendUserID(QString)),inforDlg,SLOT(setUserID(QString)));//连接信号槽 将User_ID传到informationDlg窗口
//    emit sendUserID(User_ID);
    //使用连接池 管理数据库连接
    db = ConnectionPool::openConnection();
    // 连接进度条信号槽
    connect(ui->OpenFileBtn, SIGNAL(clicked(bool)), this, SLOT(startProgressBarThread()));
    //开始加密信号槽
    connect(ui->OpenFileBtn,SIGNAL(clicked(bool)),this,SLOT(starEncryptThread()));
    finScrollArea = new QScrollArea();
    ui->MidStaWidget->addWidget(encryptionPage);
    initPageFlag=true;
    friendListLab = new QLabel(ui->RightWidget);
    friendListLab->setText(tr("好友列表"));
    friendListLab->setGeometry(ui->RightWidget->width()/2-40,5,80,30);
    friendListWidget = new QListWidget(ui->RightWidget);
    addFriendBtn = new QPushButton(ui->RightWidget);
    addFriendBtn->setGeometry(ui->RightWidget->width()/2-50,ui->RightWidget->height()-50,100,40);//设置添加好友BUTTON位置
    friendListWidget->setGeometry(0,40,ui->RightWidget->width(),ui->RightWidget->height()-40);
    addFriendBtn->setText(tr("添加好友"));
    connect(addFriendBtn,SIGNAL(clicked(bool)),this,SLOT(showAddfriendWidget()));//信号槽连接
    ui->MidStaWidget->addWidget(encryptionViewController);
    ui->MidStaWidget->addWidget(decryptionViewController);
    ui->MidStaWidget->addWidget(finishViewController);
    ui->MidStaWidget->addWidget(finishViewController2);
    ui->SearchEdit->setPlaceholderText(tr("好友搜索"));
    //查询数据库 获取用户名称
    setEmp_name();
    //使用Mylabel添加头像
    userHead = new Mylabel(ui->TopWidget);
    userHead->setGeometry(40,10,110,70);
    QPixmap pixmap(":/new/src/head1");
    pixmap.scaled(userHead->size(),Qt::KeepAspectRatio);
    userHead->setScaledContents(true);
    userHead->setPixmap(pixmap);
    //连接头像信号槽
    connect(userHead,SIGNAL(LabelClicked()),this,SLOT(HeadClickedSlot()));
    ui->FinDepBtn->hide();
    ui->FinEnpBtn->hide();
    this->setFixedSize(this->width(),this->height());
    QFont font("Microsoft YaHei",10,75);
    this->setFont(font);
        //查询数据库  查询解密请求
     QSqlQuery query(db);
     bool success = query.exec("select * from Decryption where oemp_id='"+User_ID+"'");
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
                   QPixmap pixmap(":/new/src/finEncryption");
                   pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                   v1->fileIcon->setScaledContents(true);
                   v1->fileIcon->setPixmap(pixmap);
                   v1->checkBox->setObjectName((query.record().value("id").toString())+"Decheck");//设置checkbox的ID
                   v1->setObjectName(query.record().value("id").toString()+"decryption");//设置Item的ID
                   v1->downloadBtn->setObjectName((query.record().value("id").toString())+"btn");//设置downloadBtn的ID
                   //连接信号槽
                   connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                   if(query.record().value("status").toString()=="0"){//待下载状态
                       v1->fileDescription->setText("主体文件指定分享需确认下载.");
                       v1->downloadBtn->setText("确认下载");
                       connect(v1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(OssDownLoadFile()));
                       decryptionViewController->vbox->addWidget(v1);//将v1添加到视图中
                       f_progressBar = new QProgressBar(this);
                       f_progressBar->setObjectName(v1->objectName());
                       f_progressBar->setMinimum(0);
                       f_progressBar->setMaximum(100);
                       f_progressBar->setOrientation(Qt::Horizontal);
                       f_progressBar->hide();
                       f_progressBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);  // 对齐方式
                       decryptionViewController->vbox->addWidget(f_progressBar);
                   }
                   else if(query.record().value("status").toString()=="1"){//待申请状态
                       v1->fileDescription->setText("文件已加密需下载密钥文件.");
                       v1->downloadBtn->setText("申请解密");
                       connect(v1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(getFileID()));
                       decryptionViewController->vbox->addWidget(v1);//将v1添加到视图中
                   }
                   else if(query.record().value("status").toString()=="2"){//申请等待状态
                       v1->fileDescription->setText("正在申请解密，请等待！");
                       v1->downloadBtn->setText("申请中");
                       decryptionViewController->vbox->addWidget(v1);
                   }

               }
               QWidget *newItemWidget = new QWidget();
               newScrollArea = new QScrollArea();
               newItemWidget->setLayout(decryptionViewController->vbox);
               newScrollArea->setWidget(newItemWidget);
               QVBoxLayout *newVbox = new QVBoxLayout();
               newVbox->addWidget(newScrollArea);
               decryptionViewController->setLayout(newVbox);          
           }
     //好友列表加载
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
             friendListWidget->insertItem(count,query.record().value("friend_nickname").toString());
             count++;
         }
     }

     RequestRecThread *recThread = new RequestRecThread();
     connect(recThread,SIGNAL(numChanged()),this,SLOT(ReceiveNewReq()));
     connect(recThread,SIGNAL(ReqIsAlliowed()),this,SLOT(FileIsAllowed()));
     recThread->start();
     InformationThread *inforThread = new InformationThread();
     connect(inforThread,SIGNAL(InformationChanged()),this,SLOT(HeadChanged()));
     connect(inforThread,SIGNAL(InformationChanged()),inforDlg,SLOT(newInformation()));
     inforThread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_FinishedBtn_clicked()
{
//    QPalette pal = ui->FinishedBtn->palette();
//    pal.setColor(QColorGroup::ButtonText,QColor(255,0,0));
//    ui->FinishedBtn->setPalette(pal);
    CleanButtonClicked();
    ui->FinishedBtn->setStyleSheet("background-color:rgb(119,119,119);");
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
    CleanButtonClicked();
    ui->DecryptionBtn->setStyleSheet("background-color:rgb(119,119,119);");
    ui->MidStaWidget->setCurrentWidget(decryptionViewController);
    ui->BtnStaWidget->setCurrentIndex(1);
}



void MainWindow::on_EncryptionBtn_clicked()
{   //点击解密按钮后，MidStaWidget跳转到1号子页面
    CleanButtonClicked();
    ui->EncryptionBtn->setStyleSheet("background-color:rgb(119,119,119);");
    ui->MidStaWidget->setCurrentWidget(encryptionViewController);
    if (encryptionViewController->vbox->count()==0||encryptionViewController->layout()->count()-1==0){
        //ui->BtnStaWidget->setCurrentWidget(encryptionPage);
        ui->BtnStaWidget->setCurrentIndex(0);
        ui->MidStaWidget->setCurrentWidget(encryptionPage);
        //initPageFlag=true;
    }else {
        ui->BtnStaWidget->setCurrentIndex(0);
    }
}

void MainWindow::on_FinEnpBtn_clicked()
{
    CleanButtonClicked();
    ui->FinEnpBtn->setStyleSheet("background-color:rgb(119,119,119);");
    ui->MidStaWidget->setCurrentWidget(finishViewController);
    ui->BtnStaWidget->setCurrentIndex(2);
    on_pushButton_8_clicked();
}
void MainWindow::on_FinDepBtn_clicked()
{
    CleanButtonClicked();
    ui->FinDepBtn->setStyleSheet("background-color:rgb(119,119,119);");
    ui->MidStaWidget->setCurrentWidget(finishViewController2);
    ui->BtnStaWidget->setCurrentIndex(3);
    on_pushButton_9_clicked();
}

void MainWindow::on_OpenFileBtn_clicked()
{
    QTextCodec *codec = QTextCodec::codecForName("GB18030");
    QString file_full,fName,fPath,amfSize;
    qint64 fSize;
    double mfSize;
    QFileInfo fInfo;

    file_full = QFileDialog::getOpenFileName(this,"Open File",QDir::currentPath());//打开文件选择
    if (file_full.isEmpty()){
        QMessageBox::information(this,"Erro Message","请选择文件");
        fileOpenFlag = false;

    }else{
        fileOpenFlag = true;
        initLableFlag = false;

        if (initPageFlag){

            delete encryptionPage;
            initPageFlag=false;
        }

        fInfo=QFileInfo(file_full);
        openFileInfo=fInfo;
        fName=fInfo.fileName();
        fPath=fInfo.filePath();
        fSize=fInfo.size();
        mfSize=(double)(fSize/1024.);//字节转换为KB
        amfSize=QString::number(mfSize,10,2);
        //encryption *contest = new encryption();
        //生成源文件UUID
        QUuid orfile_id =QUuid::createUuid();
        QString orFileID = orfile_id.toString();
        orfileUuid = orFileID;
        //contest->fInfo=fInfo;
        EncryptionItem *v1 = new EncryptionItem();
        v1->setObjectName(orFileID);
        v1->fileName->setText(fName);
        v1->fileSize->setText(amfSize+codec->toUnicode(" KB"));

        QPixmap pixmap(":/new/src/fileIcon");
        pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
        v1->fileIcon->setScaledContents(true);
        v1->fileIcon->setPixmap(pixmap);

        v1->encryptStaBtn->setObjectName(orFileID);
        v1->encryptStaBtn->show();
        v1->encryptStaBtn->setEnabled(false);
        v1->encryptStaBtn->setText("正在加密...");
        v1->encryptStaBtn->setFlat(true);

        encryptionViewController->vbox->addWidget(v1);

        f_progressBar = new QProgressBar(this);
        f_progressBar->setObjectName(v1->objectName());
        f_progressBar->setMinimum(0);
        f_progressBar->setMaximum(100);
        //f_progressBar->setValue(20);
        f_progressBar->setOrientation(Qt::Horizontal);
        f_progressBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);  // 对齐方式
        connect(v1->encryptStaBtn,SIGNAL(clicked(bool)),this,SLOT(on_encryptStaBtn_clicked()));
        encryptionViewController->vbox->addWidget(f_progressBar);
        //encryptionViewController->vbox->setGeometry(100);

        delete encryptionViewController->layout();
        QWidget *newItemWidget = new QWidget();
        QScrollArea *newScrollArea = new QScrollArea();
        newItemWidget->setLayout(encryptionViewController->vbox);
        newScrollArea->setWidget(newItemWidget);
        QVBoxLayout *newVbox = new QVBoxLayout();
        newVbox->addWidget(newScrollArea);
        encryptionViewController->setLayout(newVbox);
    }
}

// 更新进度条
void MainWindow::handleResults(int value)
{

    f_progressBar->setValue(value);
    if (value==100){
        EncryptionItem *v1 = ui->MidStaWidget->findChild<EncryptionItem*>(f_progressBar->objectName());
        v1->encryptStaBtn->clicked();

///////////////////////////////删除加密完成的项目
//        EncryptionItem *v1 = ui->MidStaWidget->findChild<EncryptionItem*>(f_progressBar->objectName());
//        //qDebug()<<name;
//        delete v1;
//        delete f_progressBar;
//        delete encryptionViewController->layout();
//        QWidget *newItemWidget = new QWidget();
//        QScrollArea *newScrollArea = new QScrollArea();
//        newItemWidget->setLayout(encryptionViewController->vbox);
//        newScrollArea->setWidget(newItemWidget);
//        QVBoxLayout *newVbox = new QVBoxLayout();
//        newVbox->addWidget(newScrollArea);
//        encryptionViewController->setLayout(newVbox);
///////////////////////////////////////////////////////////////////////
        on_FinEnpBtn_clicked();
    }
}

void MainWindow::on_encryptStaBtn_clicked(){
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString name = button->objectName();
    EncryptionItem *v1 = ui->MidStaWidget->findChild<EncryptionItem*>(name);
    //v1->fileDescription->setText("加密已完成！");
    v1->encryptStaBtn->show();
    v1->encryptStaBtn->setEnabled(false);
    v1->encryptStaBtn->setText("加密已完成！");
}

// 开启进度条
void MainWindow::startProgressBarThread()
{
    if (fileOpenFlag){
        enItemThread *workerThread = new enItemThread(this);
        connect(workerThread, SIGNAL(resultReady(int)), this, SLOT(handleResults(int)));
        // 线程结束后，自动销毁
        //connect(workerThread, SIGNAL(finished()), workerThread, SLOT(deleteLater()));
        workerThread->start();
        //startEncryptThread();
        encryptthread *ecpThread = new encryptthread(this);
        connect(ecpThread,SIGNAL(result(int)),this,SLOT(handleResults(int)));
        // 线程结束后，自动销毁
        connect(ecpThread, SIGNAL(finished()), ecpThread, SLOT(deleteLater()));
        ecpThread->start();
    }

}

//开始加密
void MainWindow::startEncryptThread(){
    encryptthread *ecpThread = new encryptthread(this);
    connect(ecpThread,SIGNAL(result(int)),this,SLOT(handleResults(int)));
    // 线程结束后，自动销毁
    connect(ecpThread, SIGNAL(finished()), ecpThread, SLOT(deleteLater()));
    ecpThread->start();
}

//批量删除按钮
void MainWindow::on_pushButton_3_clicked()
{
    QSqlQuery query(db);
    bool success = query.exec("select * from Decryption where oemp_id='"+User_ID+"'");
    if(!success){
        qDebug() << "查询user失败";
        return;
     }else{
              qDebug()<<"查询成功";
              //将数据库查到的数据添加到视图中
              while(query.next()){
                  QString fileID = query.record().value("id").toString();
                  QCheckBox *check = ui->MidStaWidget->findChild<QCheckBox*>(fileID+"Decheck");
                  if(check!=NULL){//判断是否找到相应控件
                      if(check->isChecked()){
                          QString deleteQuest = "delete from Decryption where id ='"+fileID+"'";
                          QSqlQuery queryDelete(db);
                          bool deleteSuccess = queryDelete.exec(deleteQuest);
                          if(!deleteSuccess){
                              qDebug()<<"delete failed";
                          }
                          else{
                              DecryptionItem *v1 = ui->MidStaWidget->findChild<DecryptionItem *>(fileID+"decryption");
                              delete v1;
                              qDebug()<<"delete success";
                              RequestNum--;
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
    bool success = query.exec("select * from Decryption where oemp_id='"+User_ID+"'");
    if(!success){
        qDebug() << "查询user失败";
        return;
     }else{

              while(query.next()){
                  QString fileID = query.record().value("id").toString();
                  QPushButton *b1 = ui->MidStaWidget->findChild<QPushButton*>(fileID+"btn");
                  if(!pt)
                      return;
                  if(pt==b1){
                      qDebug()<<fileID;
                      bool updateRequest = query.exec("update Decryption set status = 2 where id = '"+fileID+"'");
                      if(updateRequest){
                          DecryptionItem *m1 = ui->MidStaWidget->findChild<DecryptionItem *>(fileID+"decryption");
                          m1->downloadBtn->setText("申请中");
                          m1->fileDescription->setText("正在等待对方应答，请等待！");
                          disconnect(m1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(getFileID()));
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
    //创建CloundSafe 主目录
    QDir dir;
    dir.cd("D://CloundSafeWindows");  //进入某文件夹
    if(!dir.exists("D://CloundSafeWindows"))//判断需要创建的文件夹是否存在
    {
        dir.mkdir("D://CloundSafeWindows"); //创建文件夹
    }
    //创建子目录
    dir.cd("D://CloundSafeWindows//content");  //进入某文件夹
    if(!dir.exists("D://CloundSafeWindows//content"))//判断需要创建的文件夹是否存在
    {
        dir.mkdir("D://CloundSafeWindows//content"); //创建文件夹
    }
    QSqlQuery query(db);
    QPushButton *pt = qobject_cast<QPushButton *>(sender());
    bool success = query.exec("select * from Decryption where oemp_id='"+User_ID+"'");
    if(!success){
        qDebug() << "查询user失败";
        return;
     }else{
        while(query.next()){
            QString onlyId = query.record().value("id").toString();
            QString fileID = query.record().value("file_id").toString();
            QPushButton *b1 = ui->MidStaWidget->findChild<QPushButton*>(onlyId+"btn");
            QProgressBar *f_proccess = ui->MidStaWidget->findChild<QProgressBar*>(onlyId+"decryption");
            if(!pt)
                return;
            if(pt==b1){
                bool updateRequest = query.exec("update Decryption set status = 1 where id = '"+onlyId+"'");
                if(updateRequest){
                    qDebug()<<fileID;
                    QString enkey_id = fileID;
         //         DepDownThread *downThread = new DepDownThread();
                    if(f_proccess==NULL){
                        qDebug()<<"progressBARISnull";
                        return;
                    }
                    f_proccess->show();
                    f_proccess->setValue(20);
                    downThread[threadNum] = new DepDownThread();
                    connect(downThread[threadNum],SIGNAL(finished()),downThread[threadNum],SLOT(deleteLater()));
                    connect(this,SIGNAL(OSSfileDownFileID(QString,QString)),downThread[threadNum],SLOT(DownContent(QString,QString)));
                    connect(downThread[threadNum],SIGNAL(ChangeBtnText(QString)),this,SLOT(ChangeItemBtnText(QString)));
                    downThread[threadNum]->start();
                    emit OSSfileDownFileID(onlyId,enkey_id);
                    threadNum++;
                    //更新按钮内容
                    DecryptionItem *m1 = ui->MidStaWidget->findChild<DecryptionItem*>(onlyId+"decryption");
                    m1->downloadBtn->setText("正在下载");
                    disconnect(m1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(OssDownLoadFile()));//删除原有信号槽
                }
                else{
                    QMessageBox::warning(this,tr("error"),tr("下载失败"),QMessageBox::Yes);
                }
            }
        }
    }
    //重新布局
    ReLayout();
}
//收到有新请求后 将原有视图清空后重新布局
void MainWindow::ReceiveNewReq(){
    qDebug()<<"MainWindow:recv!";
    QMessageBox::warning(this,tr("ATTTENTION!"),tr("有新请求!"),QMessageBox::Yes);
    QSqlQuery query(db);
    bool success = query.exec("select * from Decryption where oemp_id='"+User_ID+"'");
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
                  QPixmap pixmap(":/new/src/finEncryption");
                  pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                  v1->fileIcon->setScaledContents(true);
                  v1->fileIcon->setPixmap(pixmap);
                  v1->checkBox->setObjectName((query.record().value("id").toString())+"Decheck");//设置checkbox的ID
                  v1->setObjectName(query.record().value("id").toString()+"decryption");//设置Item的ID
                  v1->downloadBtn->setObjectName((query.record().value("id").toString())+"btn");//设置downloadBtn的ID
                  //连接信号槽
                  connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                  if(query.record().value("status").toString()=="0"){
                      v1->fileDescription->setText("主体文件指定分享需确认下载.");
                      v1->downloadBtn->setText("确认下载");
                      connect(v1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(OssDownLoadFile()));
                      decryptionViewController->vbox->addWidget(v1);//将v1添加到视图中
                      f_progressBar = new QProgressBar(this);
                      f_progressBar->setObjectName(v1->objectName());
                      f_progressBar->setMinimum(0);
                      f_progressBar->setMaximum(100);
                      f_progressBar->setOrientation(Qt::Horizontal);
                      f_progressBar->hide();
                      f_progressBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);  // 对齐方式
                      decryptionViewController->vbox->addWidget(f_progressBar);
                  }
                  else if(query.record().value("status").toString()=="1"){
                      v1->fileDescription->setText("文件已加密需下载密钥文件.");
                      v1->downloadBtn->setText("申请解密");
                      connect(v1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(getFileID()));
                      decryptionViewController->vbox->addWidget(v1);//将v1添加到视图中
                  }
                  else if(query.record().value("status").toString()=="2"){//申请等待状态
                      v1->fileDescription->setText("正在申请解密，请等待！");
                      v1->downloadBtn->setText("申请中");
                      decryptionViewController->vbox->addWidget(v1);
                      }
              }
              ReLayout();
    }
}
//进行数据库   操作
void MainWindow::addFriendToDatabase(QString name){
    //判断是否是好友关系
    QSqlQuery query(db);
    bool judgeSuccess = query.exec("select * from friend where user_id='"+User_ID+"'");
    if(!judgeSuccess){
        qDebug()<<"查询失败";
    }
    else{
        while (query.next()) {
            if(query.record().value("friend_nickname").toString()==name){
                QMessageBox::warning(this,tr("Warning"),tr("已经是好友！"),QMessageBox::Yes);
                return;
            }
            else{
                continue;
            }
        }
    }
    //将好友关系插入到数据库表中
    QString userid;
    bool success4 = query.exec("select * from employee where emp_name='"+name+"'");
    qDebug()<<name;
    if(success4){
        if(query.size()==0){
            QMessageBox::warning(this,tr("Failed!"),tr("好友不存在"),QMessageBox::Yes);
            return;
        }
        else{
            while (query.next()) {
                if(query.record().value("emp_id").toString().isEmpty()){
                    QMessageBox::warning(this,tr("Failed!"),tr("好友不存在"),QMessageBox::Yes);
                    return;
                }
                else{
                userid = query.record().value("emp_id").toString();
                qDebug()<<userid;
                }
            }

        }

    }
    else {
        qDebug()<<"查询失败";
        QMessageBox::warning(this,tr("Failed!"),tr("好友不存在"),QMessageBox::Yes);
        return;
    }
    bool success1 = query.exec("select *from employee where emp_id = '"+User_ID+"'");
    QString friendname;
    if(success1){
        qDebug()<<"查询用户名成功";
        while(query.next()){
            friendname = query.record().value("emp_name").toString();
            qDebug()<<friendname;
        }

    }
    else{
        qDebug()<<"查询失败";
    }
    //创建记录唯一标识
    QUuid id = QUuid::createUuid();
    QString strID = id.toString();
    bool insertSuccess = query.exec("insert into friend values('"+strID+"','"+User_ID+"','"+friendname+"','"+userid+"','"+name+"',0)");
    if(insertSuccess){
        //将好友插入到视图当中
        int i = friendListWidget->count();
        i++;
        friendListWidget->insertItem(i,name);
         QMessageBox::warning(this,tr("Success"),tr("添加好友成功！"),QMessageBox::Yes);
    }
    else{
        QMessageBox::warning(this,tr("Failed!"),tr("添加好友失败！"),QMessageBox::Yes);
    }
}


//头像点击槽函数
void MainWindow::HeadClickedSlot(){
    QPixmap pixmap(":/new/src/head1");
    pixmap.scaled(userHead->size(),Qt::KeepAspectRatio);
    userHead->setScaledContents(true);
    userHead->setPixmap(pixmap);
    inforDlg->show();
}


//显示添加好友窗口槽函数
void MainWindow::showAddfriendWidget(){
    friendInputDlg *friendAdd = new friendInputDlg();
    connect(friendAdd,SIGNAL(sendNameToMain(QString)),this,SLOT(addFriendToDatabase(QString)));
    friendAdd->show();

}


void MainWindow::on_pushButton_4_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    QString originalText = clipboard->text().section("&&",1,1);
    if(originalText.startsWith("{",Qt::CaseSensitive)){
        LinkInsert(originalText);
        QMessageBox::information(this,tr("Success"),tr("已读取链接信息"),QMessageBox::Yes);
    }else{
    linkDialog = new DelinkDialog();
    connect(linkDialog,SIGNAL(sendLinkToMain(QString)),this,SLOT(LinkInsert(QString)));
    linkDialog->show();
    }
}

//已加密文件页面刷新按钮
void MainWindow::on_pushButton_8_clicked()
{
    QString file_id;
    QString file_name;
    QString file_size;
    QString file_discryption;
    QString file_status;
    finishViewController->vbox = new QVBoxLayout();
   QSqlQuery query(db);
       bool success = query.exec("select * from varticle where emp_id='"+User_ID+"'");

       if(!success){
           qDebug() << "查询密文失败";
           return;
       }else{
           qDebug()<<"查询成功";

           while(query.next())
           {
               file_name = query.record().value("article_name").toString();
               file_size = query.record().value("article_size").toString()+"kb";
               file_status = query.record().value("article_status").toString();
               file_id = query.record().value("article_id").toString();

               if(file_status == "1"){
                   file_discryption = "文件未上传，请上传。";
               }else if(file_status == "0"){
                   file_discryption = "文件已上传，可传输分享。";
               }

               FinishEncryptionItem *f2 = ui->MidStaWidget->findChild<FinishEncryptionItem*>(file_id);

               delete f2;

               FinishEncryptionItem *f1 = new FinishEncryptionItem();
               f1->setObjectName(file_id);

               QPixmap pixmap(":/new/src/finEncryption");
               pixmap.scaled(f1->fileIcon->size(),Qt::KeepAspectRatio);
               f1->fileIcon->setScaledContents(true);
               f1->fileIcon->setPixmap(pixmap);

               f1->checkBox->setObjectName(file_id + "check");
               f1->pathOpenBtn->setObjectName(file_id);
               f1->transprotBtn->setObjectName(file_id);
               f1->shareBtn->setObjectName(file_id);
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
               connect(f1->transprotBtn,SIGNAL(clicked(bool)),f1,SLOT(on_transprotBtn_clicked()));
               connect(f1->shareBtn,SIGNAL(clicked(bool)),f1,SLOT(on_shareBtn_clicked()));

           }
           return;
       }

}
//已加密文件批量删除按钮
void MainWindow::on_pushButton_5_clicked()
{
    QString file_id_d;
        QSqlQuery query3(db);
        bool success = query3.exec("select * from varticle where emp_id='"+User_ID+"'");
        if(!success){
            qDebug() << "查询密文失败";
            return;
        }else{
            qDebug()<<"查询成功";
            while(query3.next())
            {
                file_id_d = query3.record().value("article_id").toString();
                QCheckBox *checkcheck = ui->MidStaWidget->findChild<QCheckBox*>(file_id_d+"check");

                if(checkcheck->isChecked()){
                    QSqlQuery query9(db);
                    bool qq = query9.exec("delete from varticle where article_id = '"+file_id_d+"'");
                    if(qq){
                        qDebug()<<"delete 1";
                    }
                }
                FinishEncryptionItem *f2 = ui->MidStaWidget->findChild<FinishEncryptionItem*>(file_id_d);
                delete f2;
            }
         }
        on_pushButton_8_clicked();
        QMessageBox::information(NULL, "success", "成功批量删除条目！");
}

//已加密文件单独删除按钮
void MainWindow::on_deleteBtn_clicked(){
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString name = button->objectName();


    FinishEncryptionItem *f2 = ui->MidStaWidget->findChild<FinishEncryptionItem*>(name);
    delete f2;
    qDebug()<<name;

    QSqlQuery query(db);
    bool success = query.exec("delete from varticle where article_id = '"+name+"'");
    if(success){
        QMessageBox::information(NULL, "success", "成功删除已加密文件条目！");
    }

    delete finishViewController->layout();
    QWidget *newItemWidget = new QWidget();
    newItemWidget->setLayout(finishViewController->vbox);
    finScrollArea->setWidget(newItemWidget);
    QVBoxLayout *newVbox = new QVBoxLayout();
    newVbox->addWidget(finScrollArea);
    finishViewController->setLayout(newVbox);
}
//已解密文件单独删除按钮
void MainWindow::on_deleteBtn2_clicked(){
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString name = button->objectName();

    FinishDecryptionItem *f2 = ui->MidStaWidget->findChild<FinishDecryptionItem*>(name);
    delete f2;
    qDebug()<<name;

    QSqlQuery query(db);
    bool success = query.exec("delete from Decryption where file_id = '"+name+"'");
    if(success){
        QMessageBox::information(NULL, "success", "成功删除已解密文件条目！");
        RequestNum--;
    }

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
    finishViewController2->vbox = new QVBoxLayout();
   QSqlQuery query(db);
       bool success = query.exec("select * from Decryption where status = 5 and oemp_id ='" + User_ID+"'");
       if(!success){
           qDebug() << "查询密文失败";
           return;
       }else{
           qDebug()<<"查询成功";

           while(query.next())
           {
               file_name = query.record().value("file_name").toString();
               file_size = query.record().value("file_size").toString()+"kb";


               file_id = query.record().value("file_id").toString();

               file_discryption = "文件已成功解密。";

               FinishDecryptionItem *f2 = ui->MidStaWidget->findChild<FinishDecryptionItem*>(file_id);

               delete f2;

               FinishDecryptionItem *f1 = new FinishDecryptionItem();
               f1->setObjectName(file_id);

               QPixmap pixmap(":/new/src/finEncryption");
               pixmap.scaled(f1->fileIcon->size(),Qt::KeepAspectRatio);
               f1->fileIcon->setScaledContents(true);
               f1->fileIcon->setPixmap(pixmap);

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

void MainWindow::HeadChanged(){
    qDebug()<<"head change!";
    QPixmap pixmap(":/new/src/head2");
    pixmap.scaled(userHead->size(),Qt::KeepAspectRatio);
    userHead->setScaledContents(true);
    userHead->setPixmap(pixmap);
}

void MainWindow::FileIsAllowed(){
    //创建密钥文件夹
    QDir dir;
    dir.cd("D://CloundSafeWindows");  //进入某文件夹
    if(!dir.exists("D://CloundSafeWindows"))//判断需要创建的文件夹是否存在
    {
        dir.mkdir("D://CloundSafeWindows"); //创建文件夹
    }
    //创建子目录
    dir.cd("D://CloundSafeWindows//ykey");  //进入某文件夹
    if(!dir.exists("D://CloundSafeWindows//ykey"))//判断需要创建的文件夹是否存在
    {
        dir.mkdir("D://CloundSafeWindows//ykey"); //创建文件夹
    }
    //创建密文文件夹
    QDir cdir;
    cdir.cd("D://CloundSafeWindows");  //进入某文件夹
    if(!cdir.exists("D://CloundSafeWindows"))//判断需要创建的文件夹是否存在
    {
        cdir.mkdir("D://CloundSafeWindows"); //创建文件夹
    }
    //创建子目录
    cdir.cd("D://CloundSafeWindows//file");  //进入某文件夹
    if(!cdir.exists("D://CloundSafeWindows//file"))//判断需要创建的文件夹是否存在
    {
        cdir.mkdir("D://CloundSafeWindows//file"); //创建文件夹
    }


     QSqlQuery query(db);
     bool success = query.exec("select * from Decryption where oemp_id = '"+User_ID+"' and status =3");
     if(success){
         while(query.next()){
             QString onlyID = query.record().value("id").toString();
             QString file_id = query.record().value("file_id").toString();
             DecryptionItem *f1 = ui->MidStaWidget->findChild<DecryptionItem *>(onlyID+"decryption");
             if(f1==NULL){
                 continue;
             }
             else{
                 //下载密文
                 QString id = query.record().value("id").toString();
                 QString fileName = query.record().value("file_name").toString();
                 //查找秘钥ID
                 QSqlQuery keyQuery(db);
                 QString enkey_id;
                 bool keySuccess = keyQuery.exec("select * from varticle where article_id = '"+file_id+"'");
                 if(keySuccess){
                     while(keyQuery.next()){
                         enkey_id = keyQuery.record().value("key_id").toString();
                         qDebug()<<file_id;
                         qDebug()<<enkey_id;
                     }
                 }
                 //开始下载文件
                 depThread[DepThreadNum] = new DecryptionThread();
                 connect(depThread[DepThreadNum],SIGNAL(finished()),depThread[DepThreadNum],SLOT(deleteLater()));
                 connect(this,SIGNAL(sendFileID(QString,QString,QString)),depThread[DepThreadNum],SLOT(DecryptionThread_RecvID(QString,QString,QString)));
                 connect(depThread[DepThreadNum],SIGNAL(decryptionFailed()),this,SLOT(RecDecryptionFailed()));
                 depThread[DepThreadNum]->start();
                 emit sendFileID(enkey_id,file_id,fileName);
                 DepThreadNum++;
                 if(decryptionFlag == 0){
                     //解密完成后将数据库该条数据状态status改成5
                     QSqlQuery updateQuery(db);
                     bool updataSuccess = updateQuery.exec("update Decryption set status = 5 where id ='"+id+"'");
                     if(updataSuccess){
                         RequsetAllowNum--;
                         qDebug()<<"status=5 update success!";
                     }
                     else{
                         qDebug()<<"status=5 update failed";
                     }
                     //解密完成后删除本地密钥和密文

                     //解密完成后删除控件
                     delete f1;
                     //重新布局
                     delete decryptionViewController->layout();
                     QWidget *newItemWidget = new QWidget();
                     newItemWidget->setLayout(decryptionViewController->vbox);
                     newScrollArea->setWidget(newItemWidget);
                     QVBoxLayout *newVbox = new QVBoxLayout();
                     newVbox->addWidget(newScrollArea);
                     decryptionViewController->setLayout(newVbox);
                     QMessageBox::warning(this,tr("Success"),tr("文件解密同意并成功解密"),QMessageBox::Yes);
                 }
                 else{
                     return;
                 }
             }
         }
     }
     else{
         qDebug()<<"查询失败";
     }
}
void MainWindow::ChangeItemBtnText(QString fileID){
    DecryptionItem *m1 = ui->MidStaWidget->findChild<DecryptionItem*>(fileID+"decryption");
    m1->downloadBtn->setText("申请解密");
    QProgressBar *n1 = ui->MidStaWidget->findChild<QProgressBar *>(fileID+"decryption");
    int value = 21;
    while(value<100){
        n1->setValue(value);
        value++;
        Sleep(5);
    }
    delete n1;
    ReLayout();
    //f_progressBar->setValue(100);
    //解除原有信号槽
    disconnect(m1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(DeProgressBarStart()));
    //连接新的信号槽
    connect(m1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(getFileID()));
}
//设置主界面用户名
void MainWindow::setEmp_name(){
    qDebug()<<"函数执行";
    QSqlQuery query(db);
    bool success = query.exec("select * from employee where emp_id = '"+User_ID+"'");
    if(success){
        while (query.next()) {
            QString nickName = query.record().value("emp_name").toString();
            qDebug()<<nickName;
            ui->nameLabel->setText(nickName);
        }
    }
    else{
        qDebug()<<"查询用户名失败";

    }
}

void MainWindow::NumDown(){
    qDebug()<<"down";
}


//已解密文件批量删除按钮
void MainWindow::on_pushButton_7_clicked()
{
    QString file_id;
        QSqlQuery query3(db);
        bool success = query3.exec("select * from Decryption where status = 5 and oemp_id ='" + User_ID+"'");
        if(!success){
            qDebug() << "查询密文失败";
            return;
        }else{
            qDebug()<<"查询成功";
            while(query3.next())
            {
                file_id = query3.record().value("file_id").toString();

                QCheckBox *checkcheck = ui->MidStaWidget->findChild<QCheckBox*>(file_id+"check");

                if(checkcheck->isChecked()){
                    QSqlQuery query9(db);
                    query9.exec("delete from Decryption where file_id = '"+file_id+"'");
                    RequestNum--;
                }
                FinishDecryptionItem *f2 = ui->MidStaWidget->findChild<FinishDecryptionItem*>(file_id);
                delete f2;

            }
         }

        on_pushButton_9_clicked();
        QMessageBox::information(NULL, "success", "成功批量删除条目！");
}
//已解密页面全选按钮
void MainWindow::on_pushButton_10_clicked()
{
    QSqlQuery query(db);
    bool success = query.exec("select * from Decryption where status = 5 and oemp_id ='" + User_ID+"'");
    if(!success){
        qDebug() << "查询密文失败";
        return;
    }else{
        qDebug()<<"查询成功";
        while(query.next())
        {
            QString file_id = query.record().value("file_id").toString();
            QCheckBox *checkcheck = ui->MidStaWidget->findChild<QCheckBox*>(file_id+"check");
            checkcheck->setChecked(true);
        }
    }

}
//已加密页面全选按钮
void MainWindow::on_pushButton_11_clicked()
{
    QSqlQuery query(db);
    bool success = query.exec("select * from varticle where emp_id='"+User_ID+"'");
    if(!success){
        qDebug() << "查询密文失败";
        return;
    }else{
        qDebug()<<"查询成功";
        while(query.next())
        {
            QString file_id = query.record().value("article_id").toString();
            QCheckBox *checkcheck = ui->MidStaWidget->findChild<QCheckBox*>(file_id+"check");
            checkcheck->setChecked(true);
        }


    }
}

void MainWindow::CleanButtonClicked(){
    ui->FinDepBtn->setFont(f);
    ui->DecryptionBtn->setFont(f);
    ui->EncryptionBtn->setFont(f);
    ui->FinishedBtn->setFont(f);
    ui->FinEnpBtn->setFont(f);
    ui->FinDepBtn->setStyleSheet("background-color:rgb(255,255,255);");
    ui->DecryptionBtn->setStyleSheet("background-color:rgb(255,255,255);");
    ui->EncryptionBtn->setStyleSheet("background-color:rgb(255,255,255);");
    ui->FinEnpBtn->setStyleSheet("background-color:rgb(255,255,255);");
    ui->FinishedBtn->setStyleSheet("background-color:rgb(255,255,255);");
}

void MainWindow::LinkInsert(QString link){
    QString GetLink = link;
    QSqlQuery query(db);
    QString Link_empid;
    QString Link_filename;
    QString Link_filesize;
    QDateTime time = QDateTime::currentDateTime();
    QString time_str = time.toString("yyyy-MM-dd hh:mm:ss");
    bool success = query.exec("select * from varticle where article_id = '"+GetLink+"'");
    if(!success){
        qDebug()<<"linkSelectFailed";
    }
    else{
        while(query.next()){

            Link_empid = query.record().value("emp_id").toString();
            Link_filename = query.record().value("article_name").toString();
            Link_filesize = query.record().value("article_size").toString();
        }
    }
    if(Link_empid == NULL){
        QMessageBox::warning(this,tr("ERROR"),tr("无效链接！"),QMessageBox::Yes);
        return;
    }
    QUuid strid = QUuid::createUuid();
    QString id = strid.toString();
    QSqlQuery insertQuery(db);
    bool insertSuccess = insertQuery.exec("insert into Decryption values('"+id+"','"+GetLink+"','"+Link_filename+"','"+Link_empid+"','','"+User_ID+"','',0,'"+time_str+"','"+Link_filesize+"')");
    if(!insertSuccess){
        qDebug()<<"LinkInsertFailed";
    }
    else{
        DecryptionItem *a1 = new DecryptionItem();
        a1->fileName->setText(Link_filename);
        a1->fileSize->setText(Link_filesize);
        QPixmap pixmap(":/new/src/finEncryption");
        pixmap.scaled(a1->fileIcon->size(),Qt::KeepAspectRatio);
        a1->fileIcon->setScaledContents(true);
        a1->fileIcon->setPixmap(pixmap);
        a1->checkBox->setObjectName(id+"Decheck");
        a1->setObjectName(id+"decryption");
        a1->downloadBtn->setObjectName(id+"btn");
        connect(ui->pushButton,SIGNAL(clicked(bool)),a1,SLOT(changeCheckBox()));
        a1->fileDescription->setText("主体文件指定分享需确认下载.");
        a1->downloadBtn->setText("确认下载");
        connect(a1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(OssDownLoadFile()));
        decryptionViewController->vbox->addWidget(a1);
        f_progressBar = new QProgressBar(this);
        f_progressBar->setObjectName(a1->objectName());
        f_progressBar->setMinimum(0);
        f_progressBar->setMaximum(100);
        f_progressBar->setOrientation(Qt::Horizontal);
        f_progressBar->hide();
        f_progressBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);  // 对齐方式
        decryptionViewController->vbox->addWidget(f_progressBar);
        ReLayout();
    }

}
//信号槽：接收到解密线程发送的解密失败信号
void MainWindow::RecDecryptionFailed(){
    QMessageBox::warning(this,tr("error"),tr("文件解密失败"),QMessageBox::Yes);
    decryptionFlag==1;
}

void MainWindow::ReLayout(){
    delete decryptionViewController->layout();
    QWidget *newItemWidget = new QWidget();
    newItemWidget->setLayout(decryptionViewController->vbox);
    newScrollArea->setWidget(newItemWidget);
    QVBoxLayout *newVbox = new QVBoxLayout();
    newVbox->addWidget(newScrollArea);
    decryptionViewController->setLayout(newVbox);
}

void MainWindow::DeProgressBarStart(){
    //d_progressBar->setValue(proccessValue);
}



