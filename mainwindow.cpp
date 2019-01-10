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
#include <QPropertyAnimation>
#include <qt_windows.h>
#include <QFile>
#include <QTextStream>

extern int const EXIT_CODE_REBOOT;
extern QString tempFilePath;
extern QString UserIdentify;
int dir_Flag = 0;
QStringList m_fontList;
QStringList m_fontList_W4;
bool groupSendDialogFlag;
bool groupShareDialogFlag;
QNetworkAccessManager *d_accessManager;
int extraction_rate;
int AddFriendFlag = 0;
int LinkInsertFlag = 0;
int IsLinkInfor = 0;//用于判断是否链接插入的消息
int findecrypt_flag = 0;//已解密全选判断变量
int finencrypt_flag = 0;//已加密全选判断变量
int DeSelect_flag = 0;//解密全选判断变量
int isFriendListHide = 0;
int FriendRequestCount = 0;
int informationNum = 0;
int RequsetAllowNum = 0;
int Infor_requestNum = 0;
int DecryBarNum = 0;
int isFinishedBtn=0;//用于判断是否已经点击
QString RequestIDArray[50] = {};//存放Request的数组
int RequsetIndex = 0;//数组的INDEX
int FriendCount =0;
QString FriendNickNameArray[50] = {};
int FriendArrayIndex = 0;
QStringList newDownloadFileIdList;
QString User_ID = nullptr;//用户唯一标识ID
QString User_qqNum = nullptr;//用户qq_num
QString User_qqPath = nullptr;//用户本地文件存放路径
QString URL = "119.23.162.138/cloud";
bool fileOpenFlag;
bool initLableFlag = false;
bool initPageFlag=true;
int decryptionFlag =0;
int threadNum = 0;
int DepThreadNum = 0;
int encptThreadNum = 0;
int enitemNum = 0;
QFont f("冬青黑体简体",9,75);
QFont m("冬青黑体简体",10,60);
QFont m_w4;
QFont f_h("冬青黑体简体",10,60);
QFileInfo openFileInfo;
QString orfileUuid;
QString yzipfileUuid;
QString file_id_list; //批量分享时用的文件表
QString DecProFileID = nullptr;
QString file_item_name;
QFileInfo file_item_fileInfo;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //初始化用户User_ID
    User_ID = LoginUserID;
    ui->setupUi(this);

    InitUi();


    //encryptionPage = new EncryptionItem();
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
    QSqlQuery qqQuery(db);
    bool qq_success = qqQuery.exec("select * from employee where emp_id = '"+User_ID+"'");
    if(!qq_success){
        qDebug()<<"初始化QQ_NUM失败";
    }
    else{
        while(qqQuery.next()){
            User_qqNum = qqQuery.record().value("qq_num").toString();
        }
    }
    QString qq_Path = "C://CloundSafe//"+User_qqNum;
    User_qqPath = qq_Path;//给User_qqPath赋值
    tempFilePath = "C://CloundSafe//"+User_qqNum+"//temp//";
    // 连接进度条信号槽
    //connect(ui->OpenFileBtn, SIGNAL(clicked(bool)), encptThreadArr[encptThreadNum], SLOT(startProgressBarThread()));
    //connect(this, SIGNAL(starEncptItem(QString)), this, SLOT(startProgressBarThread(QString)));
    //开始加密信号槽
    //connect(ui->OpenFileBtn,SIGNAL(clicked(bool)),this,SLOT(starEncryptThread()));

    if (dir_Flag != 0 ){
        QString qq_Path = "C://CloundSafe//"+User_qqNum;
        QDir dir;
        dir.cd(qq_Path);  //进入某文件夹
        if(!dir.exists(qq_Path))//判断需要创建的文件夹是否存在
        {
            dir.mkpath(qq_Path); //创建文件夹
        }
        qq_Path = qq_Path+"//encrypt";
        //创建encrypt子目录
        dir.cd(qq_Path);  //进入某文件夹
        if(!dir.exists(qq_Path))//判断需要创建的文件夹是否存在
        {
            dir.mkpath(qq_Path); //创建文件夹
        }
        qq_Path = qq_Path+"//yKey";
        //创建yKey子目录
        dir.cd(qq_Path);  //进入某文件夹
        if(!dir.exists(qq_Path))//判断需要创建的文件夹是否存在
        {
            dir.mkpath(qq_Path); //创建文件夹
        }
        qq_Path = "C://CloundSafe//"+User_qqNum+"//encrypt//yZip";
        //创建yZip子目录
        dir.cd(qq_Path);  //进入某文件夹
        if(!dir.exists(qq_Path))//判断需要创建的文件夹是否存在
        {
            dir.mkpath(qq_Path); //创建文件夹
        }
        //encrypt();
        QString temp_Path = "C://CloundSafe//"+User_qqNum+"//temp";
        //创建yZip子目录
        dir.cd(temp_Path);  //进入某文件夹
        if(!dir.exists(temp_Path))//判断需要创建的文件夹是否存在
        {
            dir.mkpath(temp_Path); //创建文件夹
        }

    }
    dir_Flag ++;

    connect(this,SIGNAL(showDownDialog(QString)),this,SLOT(ShowNewDownDialog(QString)));
    finScrollArea = new QScrollArea();
    //ui->MidStaWidget->addWidget(encryptionPage);
    ui->EncryptionBtn->clicked(true);
    //initLableFlag = true;
    initPageFlag=true;
    friendListLab = new Mylabel(ui->RightWidget);
    friendListLab->setText(tr("好友列表"));
    friendListLab->setFont(m);
    friendListLab->setGeometry(ui->RightWidget->width()/2-60,1,80,30);
    friendIcon = new QLabel(ui->RightWidget);
    friendIcon->setGeometry(ui->RightWidget->width()/2-85,11,15,8);
    friendIcon->setStyleSheet("border-image: url(:/new/mainwindow/pictures/newfriend_show.png);");
    connect(friendListLab,SIGNAL(LabelClicked()),this,SLOT(FriendListWidgetHide()));
    friendListWidget = new QListWidget(ui->RightWidget);
    addFriendBtn = new QPushButton(ui->RightWidget);
    addFriendBtn->setGeometry(22,505,155,26);//设置添加好友BUTTON位置
    friendListWidget->setGeometry(20,30,ui->RightWidget->width()-39,ui->RightWidget->height()-44);
    friendListWidget->setIconSize(QSize(50,20));//设置Item图标大小
    friendListWidget->setStyleSheet("QListWidget{border:0;padding:0;spacing:0;}"
                                    "QListWidget::Item{border:none}"
                                    "QListWidget::item:selected{background-color:#f6f7fa;border:1px solid #dae4ff;color:#3D3939}"
                                    );
    addFriendBtn->setCursor(QCursor(Qt::PointingHandCursor));
    addFriendBtn->setStyleSheet("border-image: url(://pictures/AddFriend_icon.png);");
    connect(addFriendBtn,SIGNAL(clicked(bool)),this,SLOT(showAddfriendWidget()));//信号槽连接

    //ui->MidStaWidget->addWidget(encryptionViewController);
    ui->MidStaWidget->addWidget(decryptionViewController);
    ui->MidStaWidget->addWidget(finishViewController);
    ui->MidStaWidget->addWidget(finishViewController2);
    ui->SearchEdit->setPlaceholderText(tr("好友昵称/手机号"));
    QSize editSize(190,20);
    QSize btsize(30,20);
    ui->SearchEdit->setFixedSize(editSize);
//    QPushButton *m_bt = new QPushButton();
//    Mylabel *bt_icon = new Mylabel(this);
//    bt_icon->setGeometry(1047,100,10,11);
//    bt_icon->setStyleSheet("border-image: url(:/new/mainwindow/pictures/search.png);background-color:#398CFF;");
//    bt_icon->setCursor(QCursor(Qt::PointingHandCursor));
//    connect(bt_icon,SIGNAL(LabelClicked()),m_bt,SLOT(click()));
//    m_bt->setFixedSize(btsize);
//    m_bt->setStyleSheet("background-color:#398CFF;");
//    QHBoxLayout *m_layout=new QHBoxLayout();
//    m_layout->setContentsMargins(0,0,0,0);
//    m_layout->setSpacing(0);
//    m_layout->addStretch();
//    m_layout->addWidget(m_bt);
//    ui->SearchEdit->setLayout(m_layout);
//    ui->SearchEdit->setTextMargins(0,0,m_bt->width()-1,0);
//    m_bt->setCursor(QCursor(Qt::PointingHandCursor));
//    ui->pushButton_13->hide();
//    connect(m_bt,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_13_clicked()));
    ui->pushButton_13->setStyleSheet("QPushButton { border-image:url(:/new/mainwindow/pictures/newSearchIcon.png); }"
                                     "QPushButton:hover { border-image:url(:/new/mainwindow/pictures/newSearchIcon_hover.png); }");
    ui->SearchEdit->setStyleSheet("border-radius:10px;"
                                  "border:1px groove #D6D8DD;"
                                  "padding-left:10px;"
                                  "padding-right:32px;"
                                  );
    //查询数据库 获取用户名称
    setEmp_name();
    //使用Mylabel添加头像
    userHead = new Mylabel(ui->TopWidget);
    userHead->setGeometry(30,14,45,45);
    userHead->setStyleSheet("min-width:  45px;"
                            "max-width:  45px;"
                            "min-height: 45px;"
                            "max-height: 45px;"

                            "border-radius:22px;"
                            "border-width: 0 0 0 0;"
                            "border-image: url(:/new/mainwindow/pictures/newUserhead.png) 0 0 0 0 stretch strectch;"
                            );
//    QPixmap pixmap(":/new/src/head1");
//    pixmap.scaled(userHead->size(),Qt::KeepAspectRatio);
//    userHead->setScaledContents(true);
//    userHead->setPixmap(pixmap);
    //连接头像信号槽
    //connect(userHead,SIGNAL(LabelClicked()),this,SLOT(HeadClickedSlot()));

    this->setFixedSize(this->width(),this->height());

        //查询数据库  查询解密请求
     QSqlQuery query(db);
     bool success = query.exec("select * from Decryption where oemp_id='"+User_ID+"' order by createtime DESC");
     if(!success){
         qDebug() << "查询user失败";
         return;
      }else{
               qDebug()<<"查询成功";
               //将数据库查到的数据添加到视图中
               while(query.next())
               {
                   DecryptionItem *v1 =  new DecryptionItem();
                   QString fName = query.record().value("file_name").toString();
                   QString rate = query.record().value("file_encrypt_num").toString();
                   QString file_level;
                   if(rate == "20"){
                       file_level = "加密等级：强";
                   }else if(rate == "30"){
                       file_level = "加密等级：中";
                   }else {
                       file_level = "加密等级：弱";
                   }
                   v1->fileLevel->setText(file_level);
                   int fontSize = fontMetrics().width( fName );//获取之前设置的字符串的像素大小
                   int pos = 0;int d_count = 0;
                   pos = fName.indexOf(".");
                   while (pos>-1) {
                       d_count++;
                       pos = fName.indexOf(".",pos+1);
                   }
                   QString filetype_extra = fName.section(".",d_count-1,d_count-1).mid(fName.section(".",d_count-1,d_count-1).length()-2)+"."+fName.section(".",d_count,d_count).trimmed().toStdString().c_str() ;
                   if( fontSize >= v1->fileName->width()-100 ) //与label自身相比较
                   {
                       QString str = fontMetrics().elidedText( fName, Qt::ElideRight, v1->fileName->width()-150 )+filetype_extra;//返回一个带有省略号的字符串
                       v1->fileName->setText( str );       //重新设置label上的字符串
                   }else{
                       v1->fileName->setText(fName);
                   }

                   QString m_filesize = query.record().value("file_size").toString();
                   double filesize_double = m_filesize.toDouble();
                   int filesize_count = 0;
                   while(filesize_double>1024){
                       filesize_double = filesize_double/1024;
                       filesize_count++;
                   }
                   if(filesize_count == 0){
                       v1->fileSize->setText(QString::number(filesize_double, 10, 2)+"KB");
                   }else if(filesize_count == 1){
                       v1->fileSize->setText(QString::number(filesize_double, 10, 2)+"MB");
                   }else if(filesize_count == 2){
                       v1->fileSize->setText(QString::number(filesize_double, 10, 2)+"GB");
                   }else if(filesize_count == 3){
                       v1->fileSize->setText(QString::number(filesize_double, 10, 2)+"TB");
                   }
                   //设置文件大小
                   v1->timeLabel->setText(query.record().value("createtime").toString());
                   //设置fileIcon的图片
                   QString filetype = query.record().value("file_name").toString().section(".",1,1).trimmed().toStdString().c_str();
                   if((filetype=="jpg")||(filetype=="png")||(filetype=="jpeg")||(filetype=="bmp")||(filetype=="gif")||(filetype=="webp")||(filetype=="psd")||(filetype=="svg")||(filetype=="tiff")){
                       QPixmap pixmap(":/new/mainwindow/pictures/pic_icon.png");
                       pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                       v1->fileIcon->setScaledContents(true);
                       v1->fileIcon->setPixmap(pixmap);
                   }else if((filetype=="avi")||(filetype=="rmvb")||(filetype=="rm")||(filetype=="asf")||(filetype=="divx")||(filetype=="wmv")||(filetype=="mp4")||(filetype=="mkv")||(filetype=="vob")||(filetype=="mpeg")){
                       QPixmap pixmap(":/new/mainwindow/pictures/video_icon.png");
                       pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                       v1->fileIcon->setScaledContents(true);
                       v1->fileIcon->setPixmap(pixmap);
                   }else if((filetype=="doc")||(filetype=="docx")||(filetype=="xls")||(filetype=="xlsx")||(filetype=="ppt")||(filetype=="pptx")||(filetype=="txt")||(filetype=="docm")){
                       QPixmap pixmap(":/new/mainwindow/pictures/doc_icon.png");
                       pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                       v1->fileIcon->setScaledContents(true);
                       v1->fileIcon->setPixmap(pixmap);
                   }else if((filetype=="rar")||(filetype=="zip")||(filetype=="arj")||(filetype=="z")){
                       QPixmap pixmap(":/new/mainwindow/pictures/zip_icon.png");
                       pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                       v1->fileIcon->setScaledContents(true);
                       v1->fileIcon->setPixmap(pixmap);
                   }else if((filetype=="mp3")||(filetype=="wma")||(filetype=="wav")||(filetype=="ape")||(filetype=="flac")||(filetype=="ogg")||(filetype=="aac")){
                       QPixmap pixmap(":/new/mainwindow/pictures/music_icon.png");
                       pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                       v1->fileIcon->setScaledContents(true);
                       v1->fileIcon->setPixmap(pixmap);
                   }else{
                       QPixmap pixmap(":/new/mainwindow/pictures/else_icon.png");
                       pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                       v1->fileIcon->setScaledContents(true);
                       v1->fileIcon->setPixmap(pixmap);
                       v1->elseLabel->setText(filetype.left(3));
                       v1->elseLabel->raise();
                   }
                   QString initID = query.record().value("id").toString();
                   v1->checkBox->setObjectName(initID+"Decheck");//设置checkbox的ID
                   v1->setObjectName(initID+"decryption");//设置Item的ID
                   v1->downloadBtn->setObjectName(initID+"btn");//设置downloadBtn的ID
                   v1->downloadBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: #3A8CFF;color:white;}QPushButton:pressed{background-color: rgb(139,159,185);}");
                   //连接信号槽
//                 connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                   if(query.record().value("status").toString()=="0"){//待下载状态
                       RequestIDArray[RequsetIndex]=initID;
                       RequsetIndex++;
                       v1->fileDescription->setText("主体文件指定分享需确认下载.");
                       v1->downloadBtn->setText("确认下载");
                       connect(v1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(OssDownLoadFile()));
                       decryptionViewController->vbox->addWidget(v1);//将v1添加到视图中
 //                      connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                       f_progressBar = new QProgressBar();
                       f_progressBar = v1->progressBar;
                       f_progressBar->setObjectName(v1->objectName());
//                       v1->fileIcon->stackUnder(f_progressBar);
//                       v1->timeLabel->stackUnder(f_progressBar);
                       f_progressBar->raise();
                       f_progressBar->hide();
                       f_progressBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);  // 对齐方式
                       //decryptionViewController->vbox->addWidget(f_progressBar);
                   }
                   else if(query.record().value("status").toString()=="1"){//待申请状态
                       v1->fileDescription->setText("文件已加密需下载密钥文件.");
                       v1->downloadBtn->setText("申请解密");
                       v1->label->show();
//                       connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                       connect(v1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(getFileID()));
                       decryptionViewController->vbox->addWidget(v1);//将v1添加到视图中
                   }
                   else if(query.record().value("status").toString()=="2"){//申请等待状态
                       v1->fileDescription->setText("正在申请解密，请等待！");
                       v1->downloadBtn->setText("申请中");
                       v1->label->show();
//                       connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                       decryptionViewController->vbox->addWidget(v1);
                   }
                   else if(query.record().value("status").toString()=="3"){//申请请求已同意
                       v1->fileDescription->setText("正在申请解密，请等待！");
                       v1->downloadBtn->setText("申请中");
                       v1->label->show();
                       decryptionViewController->vbox->addWidget(v1);

                   }else if(query.record().value("status").toString()=="11"){//安卓端本地解密
                       v1->fileDescription->setText("文件已加密需下载密钥文件.");
                       v1->downloadBtn->setText("申请解密");
                       v1->fileSize->setText("文件来自安卓系统");
                       v1->downloadBtn->setObjectName(query.record().value("file_id").toString());
                       v1->label->show();
//                       connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                       connect(v1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(androidDecrypt()));
                       decryptionViewController->vbox->addWidget(v1);//将v1添加到视图中
                       qDebug()<<"android!!!!!!!!!";
                   }else if(query.record().value("status").toString()=="21"){//ios端本地解密
                       v1->fileDescription->setText("文件已加密需下载密钥文件.");
                       v1->downloadBtn->setText("申请解密");
                       v1->fileSize->setText("文件来自iOS系统");
                       v1->downloadBtn->setObjectName(query.record().value("file_id").toString());
                       v1->label->show();
//                       connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                       connect(v1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(getFileID()));
                       decryptionViewController->vbox->addWidget(v1);//将v1添加到视图中
                   }else if(query.record().value("status").toString()=="12"){//安卓端本地解密第二步
                       v1->fileDescription->setText("已申请解密！");
                       v1->downloadBtn->setText("解密");
                       v1->fileSize->setText("文件来自安卓系统");
                       v1->downloadBtn->setObjectName(query.record().value("file_id").toString());
                       v1->label->show();
                       connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                       decryptionViewController->vbox->addWidget(v1);
                   }else if(query.record().value("status").toString()=="22"){//ios端本地解密第二步
                       v1->fileDescription->setText("已申请解密！");
                       v1->downloadBtn->setText("解密");
                       v1->fileSize->setText("文件来自iOS系统");
                       v1->downloadBtn->setObjectName(query.record().value("file_id").toString());
                       v1->label->show();
                       connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                       decryptionViewController->vbox->addWidget(v1);
                   }
               }
               decryptionViewController->vbox->setMargin(0);
               decryptionViewController->vbox->setSpacing(0);
               QWidget *newItemWidget = new QWidget();
               newScrollArea = new QScrollArea();
//               newScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
               newScrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
                                                                 "{"
                                                                 "width:8px;"
                                                                 "background:rgba(0,0,0,0%);"
                                                                 "margin:0px,0px,0px,0px;"
                                                                 "padding-top:9px;"
                                                                 "padding-bottom:9px;"
                                                                 "}"
                                                                 "QScrollBar::handle:vertical"
                                                                 "{"
                                                                 "width:8px;"
                                                                 "background:rgba(0,0,0,25%);"
                                                                 " border-radius:4px;"
                                                                 "min-height:20;"
                                                                 "}"
                                                                 "QScrollBar::handle:vertical:hover"
                                                                 "{"
                                                                 "width:8px;"
                                                                 "background:rgba(0,0,0,50%);"
                                                                 " border-radius:4px;"
                                                                 "min-height:20;"
                                                                 "}"
                                                                 "QScrollBar::add-line:vertical"
                                                                 "{"
                                                                 "height:5px;width:10px;"
                                                                 "border-image:url(:/new/mainwindow/pictures/Scrollbar_bottom.png);"
                                                                 "subcontrol-position:bottom;"
                                                                 "}"
                                                                 "QScrollBar::sub-line:vertical"
                                                                 "{"
                                                                 "height:5px;width:10px;"
                                                                 "border-image:url(:/new/mainwindow/pictures/Scrollbar_top.png);"
                                                                 "subcontrol-position:top;"
                                                                 "}"
                                                                 "QScrollBar::add-line:vertical:hover"
                                                                 "{"
                                                                 "height:9px;width:8px;"
                                                                 "border-image:url(:/images/a/4.png);"
                                                                 "subcontrol-position:bottom;"
                                                                 "}"
                                                                 "QScrollBar::sub-line:vertical:hover"
                                                                 "{"
                                                                 "height:9px;width:8px;"
                                                                 "border-image:url(:/images/a/2.png);"
                                                                 "subcontrol-position:top;"
                                                                 "}"
                                                                 "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
                                                                 "{"
                                                                 "background:rgba(0,0,0,5%);"
                                                                 "border-radius:4px;"
                                                                 "}");
               newItemWidget->setLayout(decryptionViewController->vbox);
               newScrollArea->setWidget(newItemWidget);
               newScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
               QVBoxLayout *newVbox = new QVBoxLayout();
               newVbox->setMargin(0);
               newVbox->setSpacing(0);
               newVbox->addWidget(newScrollArea);
               decryptionViewController->setLayout(newVbox);
           }
     //好友列表加载
     bool friendSelSuc = query.exec("select * from friend where user_id ='"+User_ID+"' and status = '1'");
     if(!friendSelSuc){
         qDebug()<<"查询好友失败";
         return;
     }
     else{
         qDebug()<<"查询好友成功";
         //将查询的好友插入视图中
         int count = 0;
         while(query.next()){
             QString Friend_nickname = query.record().value("friend_nickname").toString();
             QListWidgetItem *add_item = new QListWidgetItem(friendListWidget);
             add_item->setIcon(QIcon(":/new/mainwindow/pictures/FriendListIcon_new.png"));
             add_item->setText(Friend_nickname);
             add_item->setFont(m);
             add_item->setTextAlignment(Qt::AlignLeft|Qt::AlignLeft);
             add_item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
             add_item->setSizeHint(QSize(ui->RightWidget->width()-45,34));
             add_item->setTextAlignment(Qt::AlignVCenter);
             friendListWidget->insertItem(count,add_item);
             count++;
         }
     }
     QSqlQuery friendQuery(db);
     bool FriendArraySuccess = friendQuery.exec("select * from friend where friend_id ='"+User_ID+"'");
     if(!FriendArraySuccess){
         qDebug()<<"初始化好友数组失败";
     }
     else{
         while (friendQuery.next()) {
             FriendNickNameArray[FriendArrayIndex]=friendQuery.record().value("user_id").toString();
             FriendArrayIndex++;
             FriendCount++;

         }
     }
     newdownloadDlg = new newDownloadDialog(this);
     connect(newdownloadDlg,SIGNAL(Infor_numChange()),this,SLOT(newDownDialogInforInit()));
     RequestRecThread *recThread = new RequestRecThread();
     connect(recThread,SIGNAL(numChanged()),this,SLOT(ReceiveNewReq()));
     connect(recThread,SIGNAL(ReqIsAlliowed()),this,SLOT(FileIsAllowed()));
     connect(recThread,SIGNAL(thread_Disconnected()),this,SLOT(internet_Disconnected()));
     connect(recThread,SIGNAL(ReqIsIgnored()),this,SLOT(FileIsIgnored()));
     connect(recThread,SIGNAL(forceShutDown()),this,SLOT(forceShut()));
     connect(this,SIGNAL(newDownload()),newdownloadDlg,SLOT(change_list_view()));
     recThread->start();
     InformationThread *inforThread = new InformationThread();
     //connect(inforThread,SIGNAL(InformationChanged()),this,SLOT(HeadChanged()));
     connect(inforThread,SIGNAL(InformationChanged()),this,SLOT(InforNum_Changed()));
     connect(inforThread,SIGNAL(InformationChanged()),inforDlg,SLOT(newInformation()));
     connect(inforThread,SIGNAL(NewFriendRequest()),inforDlg,SLOT(NewFriend()));
     connect(inforThread,SIGNAL(NewFriendRequest()),this,SLOT(RecvNewFriendReq()));
     connect(inforThread,SIGNAL(NewAgreeFriend()),this,SLOT(NewFriendAgree()));
     connect(inforDlg,SIGNAL(InforNumDecrease()),this,SLOT(InforNum_Changed()));
     connect(inforDlg,SIGNAL(addFriendToMain(QString)),this,SLOT(inforDlgaddFriend(QString)));
     connect(this,SIGNAL(SendInforToInforDlg(QString,QString,QString)),inforDlg,SLOT(NewRequestRec(QString,QString,QString)));
     connect(inforDlg,SIGNAL(CleanInforNum()),this,SLOT(InforNum_Changed()));
     inforThread->start();
     Init_InforIcon();//初始化消息按钮
//     FileIsAllowed();

//     QString qss;
//     QFile qssFile(":/mainWindow.qss");
//     qssFile.open(QFile::ReadOnly);
//     if(qssFile.isOpen()){
//         qssFile.seek(0);
//         qss = qssFile.readAll().toStdString().c_str();
//         qApp->setStyleSheet(qss);
//         qssFile.close();
//     }
     qApp->setStyleSheet("QPushButton#pushButton_4 { border-image: url(:/new/mainwindow/pictures/link_download.png); }"
                         "QPushButton#pushButton_14 { border-image: url(:/new/mainwindow/pictures/min_button.png); }"
                         "QPushButton#pushButton_max { border-image: url(:/new/mainwindow/pictures/max_button.png); }"
                         "QPushButton#pushButton_15 { border-image: url(:/new/mainwindow/pictures/exit_button.png); }"
                         "QPushButton#OpenFileBtn { border-image: url(:/new/mainwindow/pictures/openfile.png); }"
                         "QPushButton#OpenFileBtn:hover { border-image: url(:/new/mainwindow/pictures/openfile_hover.png); }"
                         "QPushButton#OpenFileBtn:pressed { border-image: url(:/new/mainwindow/pictures/openfile_pressed.png); }"
                         "QPushButton#pushButton_3 { border-image: url(:/new/mainwindow/pictures/groupdelete.png); }"
                         "QPushButton#pushButton_3:hover { border-image: url(:/new/mainwindow/pictures/groupdelete_hover.png); }"
                         "QPushButton#pushButton_3:pressed { border-image: url(:/new/mainwindow/pictures/groupdelete_pressed.png); }"
                         "QPushButton#pushButton_5 { border-image: url(:/new/mainwindow/pictures/groupdelete.png); }"
                         "QPushButton#pushButton_5:hover { border-image: url(:/new/mainwindow/pictures/groupdelete_hover.png); }"
                         "QPushButton#pushButton_5:pressed { border-image: url(:/new/mainwindow/pictures/groupdelete_pressed.png); }"
                         "QPushButton#pushButton_6 { border-image: url(:/new/mainwindow/pictures/grouptrans.png); }"
                         "QPushButton#pushButton_6:hover { border-image: url(:/new/mainwindow/pictures/grouptrans_hover.png); }"
                         "QPushButton#pushButton_6:pressed { border-image: url(:/new/mainwindow/pictures/grouptrans_pressed.png); }"
                         "QPushButton#pushButton { border-image: url(:/new/mainwindow/pictures/allselect.png); }"
                         "QPushButton#pushButton_groupshare { border-image: url(:/new/mainwindow/pictures/groupshare.png);}"
                         "QPushButton#pushButton_groupshare:hover { border-image: url(:/new/mainwindow/pictures/groupshare_hover.png);}"
                         "QPushButton#pushButton_groupshare:pressed { border-image: url(:/new/mainwindow/pictures/groupshare_pressed.png);}"
                         "QPushButton#pushButton_10 { border-image: url(:/new/mainwindow/pictures/allselect.png); }"
                         "QPushButton#pushButton_11 { border-image: url(:/new/mainwindow/pictures/allselect.png); }"
                         "QPushButton#pushButton_7 { border-image: url(:/new/mainwindow/pictures/groupdelete.png); }"
                         "QPushButton#pushButton_7:hover { border-image: url(:/new/mainwindow/pictures/groupdelete_hover.png); }"
                         "QPushButton#pushButton_7:pressed { border-image: url(:/new/mainwindow/pictures/groupdelete_pressed.png); }"
                         "QPushButton#pushButton:hover { border-image: url(:/new/mainwindow/pictures/allselect_hover.png); }"
                         "QPushButton#pushButton_10:hover { border-image: url(:/new/mainwindow/pictures/allselect_hover.png); }"
                         "QPushButton#pushButton_11:hover { border-image: url(:/new/mainwindow/pictures/allselect_hover.png); }"
                       //  "#line,#line_1,#line_2,#line_3,#line_4,#line_5,#line_6,#line_7,#line_8,#line_9,#line_10,#line_11 {background-color: rgb(243,243,243);}"
                         "*{outline:0px;}"
                         );


        thread_11 = new QThread(this);
        workThread = new heartThread(); //工作线程，具体的业务实现在此中完成，继承自Object
        workThread->moveToThread( thread_11 );       //加入到子线程
        thread_11->start();                        //启动线程

        threadTimer = new QTimer();
        QObject::connect( threadTimer, SIGNAL(timeout()), workThread, SLOT( work() ),Qt::AutoConnection );
        threadTimer->start(10000);   // 计时器，每隔几秒钟唤起工作线程；
        d_accessManager = new QNetworkAccessManager(this);
        connect(d_accessManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(finishedSlot(QNetworkReply*)));

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::InitUi(){
    httpFlag = 0;
    setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    QString dir = QCoreApplication::applicationDirPath();
    m_fontList.clear();
    m_fontList_W4.clear();
    int lcdFontId = QFontDatabase::addApplicationFont(":/pictures/W3.ttf"); // 从source资源文件
    // int lcdFontId = QFontDatabase::addApplicationFont(dir + "/fonts/DS-DIGI.ttf"); //从外部资源文件
    if (lcdFontId != -1) // -1为加载失败
    {
        m_fontList << QFontDatabase::applicationFontFamilies(lcdFontId);
    }
    int lcdFontId_2 = QFontDatabase::addApplicationFont(":/pictures/W4.ttf"); // 从source资源文件
    // int lcdFontId = QFontDatabase::addApplicationFont(dir + "/fonts/DS-DIGI.ttf"); //从外部资源文件
    if (lcdFontId_2 != -1) // -1为加载失败
    {
        m_fontList_W4 << QFontDatabase::applicationFontFamilies(lcdFontId_2);
    }
    m_w4.setFamily("微软雅黑");
    m.setFamily("微软雅黑");
    f.setFamily("微软雅黑");
    f_h.setFamily("微软雅黑");
//    m_w4.setFamily(m_fontList_W4.at(0));
//    m.setFamily(m_fontList.at(0));
//    f.setFamily(m_fontList.at(0));
//    f_h.setFamily(m_fontList.at(0));
    m.setPixelSize(14);
    m_w4.setPixelSize(14);
    f.setPixelSize(14);
    f_h.setPixelSize(20);
    m.setWeight(QFont::Normal);
    m_w4.setWeight(QFont::Normal);
    f.setWeight(QFont::Bold);
    f_h.setWeight(QFont::Bold);
    ui->BtnStaWidget->setCurrentIndex(0);//BtnStaWidget跳转到加密界面
    ui->pushButton_3->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_4->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_5->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_6->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_7->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_8->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_9->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_10->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_10->hide();
    ui->pushButton_11->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_11->hide();
    ui->pushButton->hide();
    ui->pushButton_12->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_13->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_14->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_15->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_max->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->OpenFileBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->EncryptionBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->DecryptionBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->FinDepBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->FinEnpBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->FinishedBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_groupshare->setCursor(QCursor(Qt::PointingHandCursor));
    ui->finen_checkBox->setStyleSheet("QCheckBox {color: black;}"
                                      "QCheckBox::indicator {width: 52px;height: 14px;}"
                                      "QCheckBox::indicator:enabled:unchecked {image: url(:/new/mainwindow/pictures/mainwindow_checkbox_all.png);}"
                                      "QCheckBox::indicator:enabled:unchecked:hover {image: url(:/new/mainwindow/pictures/mainwindow_checkbox_hover.png);}"
                                      "QCheckBox::indicator:enabled:unchecked:pressed {image: url(:/new/mainwindow/pictures/mainwindow_checkbox_hover.png);}"
                                      "QCheckBox::indicator:enabled:checked {image: url(:/new/mainwindow/pictures/mainwindow_checkbox_checked.png);}"
                                      "QCheckBox::indicator:enabled:checked:hover {image: url(:/new/mainwindow/pictures/mainwindow_checkbox_checked_hover.png);}"
                                      "QCheckBox::indicator:enabled:checked:hover {image: url(:/new/mainwindow/pictures/mainwindow_checkbox_checked_hover.png);}");
    ui->finen_checkBox->setCursor(QCursor(Qt::PointingHandCursor));
    ui->finen_checkBox->setFont(m);
    ui->finen_checkBox_2->setStyleSheet("QCheckBox {color: black;}"
                                        "QCheckBox::indicator {width: 52px;height: 14px;}"
                                        "QCheckBox::indicator:enabled:unchecked {image: url(:/new/mainwindow/pictures/mainwindow_checkbox_all.png);}"
                                        "QCheckBox::indicator:enabled:unchecked:hover {image: url(:/new/mainwindow/pictures/mainwindow_checkbox_hover.png);}"
                                        "QCheckBox::indicator:enabled:unchecked:pressed {image: url(:/new/mainwindow/pictures/mainwindow_checkbox_hover.png);}"
                                        "QCheckBox::indicator:enabled:checked {image: url(:/new/mainwindow/pictures/mainwindow_checkbox_checked.png);}"
                                        "QCheckBox::indicator:enabled:checked:hover {image: url(:/new/mainwindow/pictures/mainwindow_checkbox_checked_hover.png);}"
                                        "QCheckBox::indicator:enabled:checked:hover {image: url(:/new/mainwindow/pictures/mainwindow_checkbox_checked_hover.png);}");
    ui->finen_checkBox_2->setCursor(QCursor(Qt::PointingHandCursor));
    ui->finen_checkBox_2->setFont(m);
    ui->de_checkBox->setStyleSheet("QCheckBox {color: black;}"
                                   "QCheckBox::indicator {width: 52px;height: 14px;}"
                                   "QCheckBox::indicator:enabled:unchecked {image: url(:/new/mainwindow/pictures/mainwindow_checkbox_all.png);}"
                                   "QCheckBox::indicator:enabled:unchecked:hover {image: url(:/new/mainwindow/pictures/mainwindow_checkbox_hover.png);}"
                                   "QCheckBox::indicator:enabled:unchecked:pressed {image: url(:/new/mainwindow/pictures/mainwindow_checkbox_hover.png);}"
                                   "QCheckBox::indicator:enabled:checked {image: url(:/new/mainwindow/pictures/mainwindow_checkbox_checked.png);}"
                                   "QCheckBox::indicator:enabled:checked:hover {image: url(:/new/mainwindow/pictures/mainwindow_checkbox_checked_hover.png);}"
                                   "QCheckBox::indicator:enabled:checked:hover {image: url(:/new/mainwindow/pictures/mainwindow_checkbox_checked_hover.png);}");
    ui->de_checkBox->setCursor(QCursor(Qt::PointingHandCursor));
    ui->de_checkBox->setFont(m);

    ui->FinDepBtn->hide();
    ui->FinEnpBtn->hide();
    ui->line_10->hide();
    ui->line_11->hide();
    ui->pushButton_12->hide();
    ui->pushButton_8->hide();
    ui->pushButton_9->hide();
    connect(this,SIGNAL(decryptSignal(QString,QString)),this,SLOT(decryptSlot(QString,QString)));
    connect(this,SIGNAL(FinishedDecryption(QString)),this,SLOT(finishDecryption(QString)));
}
void MainWindow::on_FinishedBtn_clicked()
{
//    QPalette pal = ui->FinishedBtn->palette();
//    pal.setColor(QColorGroup::ButtonText,QColor(255,0,0));
//    ui->FinishedBtn->setPalette(pal);
    CleanButtonClicked();
    clearCheckBox();
    ui->FinishedBtn->setStyleSheet("border-image: url(:/new/mainwindow/pictures/mainwindow_button_bg_selected.png);color:#3A8CFF;");
    ui->FinishedBtn->setIcon(QIcon(":/new/mainwindow/pictures/fin_icon_selected.png"));
    ui->FinishedBtn->setFont(f);
    ui->blue_encrypt_3->setStyleSheet("background-color: #3A8CFF");
    //点击已加密判断按钮是否需要隐藏下方按钮
    if(isFinishedBtn==0){
        ui->FinDepBtn->show();
        ui->FinEnpBtn->show();
        ui->line_10->show();
        ui->line_11->show();
        isFinishedBtn=1;
    }
    else{
        ui->FinDepBtn->hide();
        ui->FinEnpBtn->hide();
        ui->line_10->hide();
        ui->line_11->hide();
        isFinishedBtn = 0;
    }

}

void MainWindow::on_DecryptionBtn_clicked()
{   //点击解密按钮后，MidStaWidget跳转到1号子页面
    findecrypt_flag = 0;//已解密全选判断变量
    ui->pushButton_11->setStyleSheet(" QPushButton {border-image: url(:/new/mainwindow/pictures/allselect.png);} "
                                     " QPushButton:hover {border-image: url(:/new/mainwindow/pictures/allselect_hover.png);}");
    finencrypt_flag = 0;//已加密全选判断变量
    ui->pushButton_10->setStyleSheet(" QPushButton {border-image: url(:/new/mainwindow/pictures/allselect.png);} "
                                     " QPushButton:hover {border-image: url(:/new/mainwindow/pictures/allselect_hover.png);}");
    CleanButtonClicked();
    clearCheckBox();
    ui->blue_encrypt_2->setStyleSheet("background-color: #3A8CFF");
    ui->DecryptionBtn->setStyleSheet("border-image: url(:/new/mainwindow/pictures/mainwindow_button_bg_selected.png);color:#3A8CFF;");
    ui->DecryptionBtn->setFont(f);
    ui->DecryptionBtn->setIcon(QIcon(":/new/mainwindow/pictures/decryption_icon_selected.png"));
    ui->MidStaWidget->setCurrentWidget(decryptionViewController);
    ui->BtnStaWidget->setCurrentIndex(1);
}



void MainWindow::on_EncryptionBtn_clicked()
{    //点击加密按钮后，QMidStaWidget跳转到0号子页面
    findecrypt_flag = 0;//已解密全选判断变量
    ui->pushButton_11->setStyleSheet(" QPushButton {border-image: url(:/new/mainwindow/pictures/allselect.png);} "
                                     " QPushButton:hover {border-image: url(:/new/mainwindow/pictures/allselect_hover.png);}");
    finencrypt_flag = 0;//已加密全选判断变量
    ui->pushButton_10->setStyleSheet(" QPushButton {border-image: url(:/new/mainwindow/pictures/allselect.png);} "
                                     " QPushButton:hover {border-image: url(:/new/mainwindow/pictures/allselect_hover.png);}");
    CleanButtonClicked();
    clearCheckBox();
    ui->blue_encrypt->setStyleSheet("background-color: #3A8CFF");
    ui->EncryptionBtn->setStyleSheet("border-image: url(:/new/mainwindow/pictures/mainwindow_button_bg_selected.png);color:#3A8CFF;");
    ui->EncryptionBtn->setIcon(QIcon(":/new/mainwindow/pictures/encryption_icon_selected.png"));
    ui->EncryptionBtn->setFont(f);

    if (encryptionViewController->vbox->count()==0){
        //ui->BtnStaWidget->setCurrentWidget(encryptionPage);
        ui->MidStaWidget->addWidget(encryptionViewController);
        //ui->MidStaWidget->setCurrentWidget(encryptionViewController);
        encryptionPage = new EncryptionItem();
        ui->MidStaWidget->addWidget(encryptionPage);
        ui->BtnStaWidget->setCurrentIndex(0);//顶部
        initLableFlag = true;
        //initPageFlag=true;

        ui->MidStaWidget->setCurrentWidget(encryptionPage);



    }else {
        ui->MidStaWidget->setCurrentWidget(encryptionViewController);
        ui->BtnStaWidget->setCurrentIndex(0);
    }
}

void MainWindow::on_FinEnpBtn_clicked()
{
    findecrypt_flag = 0;//已解密全选判断变量
    ui->pushButton_11->setStyleSheet(" QPushButton {border-image: url(:/new/mainwindow/pictures/allselect.png);} "
                                     " QPushButton:hover {border-image: url(:/new/mainwindow/pictures/allselect_hover.png);}");
    finencrypt_flag = 0;//已加密全选判断变量
    ui->pushButton_10->setStyleSheet(" QPushButton {border-image: url(:/new/mainwindow/pictures/allselect.png);} "
                                     " QPushButton:hover {border-image: url(:/new/mainwindow/pictures/allselect_hover.png);}");
    CleanButtonClicked();
    clearCheckBox();
    ui->blue_encrypt_4->setStyleSheet("background-color: #3A8CFF");
    ui->FinEnpBtn->setStyleSheet("border-image: url(:/new/mainwindow/pictures/mainwindow_button_bg_selected.png);color:#3A8CFF;");
    ui->FinEnpBtn->setFont(f);
    ui->MidStaWidget->setCurrentWidget(finishViewController);
    ui->BtnStaWidget->setCurrentIndex(2);
    on_pushButton_8_clicked();

}
void MainWindow::on_FinDepBtn_clicked()
{
    findecrypt_flag = 0;//已解密全选判断变量
    ui->pushButton_11->setStyleSheet(" QPushButton {border-image: url(:/new/mainwindow/pictures/allselect.png);} "
                                     " QPushButton:hover {border-image: url(:/new/mainwindow/pictures/allselect_hover.png);}");
    finencrypt_flag = 0;//已加密全选判断变量
    ui->pushButton_10->setStyleSheet(" QPushButton {border-image: url(:/new/mainwindow/pictures/allselect.png);} "
                                     " QPushButton:hover {border-image: url(:/new/mainwindow/pictures/allselect_hover.png);}");
    CleanButtonClicked();
    clearCheckBox();
    ui->blue_encrypt_5->setStyleSheet("background-color: #3A8CFF");
    ui->FinDepBtn->setStyleSheet("border-image: url(:/new/mainwindow/pictures/mainwindow_button_bg_selected.png);color:#3A8CFF;");
    ui->FinDepBtn->setFont(f);
    ui->MidStaWidget->setCurrentWidget(finishViewController2);
    ui->BtnStaWidget->setCurrentIndex(3);
    on_pushButton_9_clicked();
}

void MainWindow::on_OpenFileBtn_clicked()
{
    encryptionLevel *levelbox = new encryptionLevel(this);

    QTextCodec *codec = QTextCodec::codecForName("utf8");//18030
    //QString file_full,fName,fPath,amfSize;
    QString file_full,fPath,amfSize;
    qint64 fSize;
    double mfSize;
    QFileInfo fInfo;
    QFileDialog *selectFiles =new QFileDialog ();
    selectFiles ->setFileMode(QFileDialog::ExistingFiles);
    //selectFiles->getOpenFileNames(this,"Open File",QDir::currentPath());
    QStringList file_full_list;
    file_full_list = selectFiles->getOpenFileNames(this,"Open File",QDir::currentPath());
    //file_full = QFileDialog::getOpenFileName(this,"Open File",QDir::currentPath());//打开文件选择
    qDebug()<<file_full_list;
    if (file_full_list.isEmpty()){
//            MsgBox *msgbox = new MsgBox(3,QStringLiteral("请选择文件"),this);
//            msgbox->exec();

    //        QMessageBox message(QMessageBox::NoIcon,"Erro Message","请选择文件");
    //        message.setIconPixmap(QPixmap(":/new/mainwindow/pictures/system_waring.png"));
    //        message.exec();
        fileOpenFlag = false;

    }else{
        fileOpenFlag = true;
        int nRes = levelbox->exec();
        if (nRes != QDialog::Accepted){

        }else{

        if (initLableFlag){

            delete encryptionPage;

            delete encryptionViewController->layout();
            QWidget *newItemWidget = new QWidget();
            newItemWidget->setContentsMargins(0,0,0,0);
            ui->MidStaWidget->setCurrentWidget(encryptionViewController);
            QScrollArea *newScrollArea = new QScrollArea();
            //newScrollArea->setWidgetResizable(true);//铺满显示
//            newScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            newScrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
                                                              "{"
                                                              "width:8px;"
                                                              "background:rgba(0,0,0,0%);"
                                                              "margin:0px,0px,0px,0px;"
                                                              "padding-top:9px;"
                                                              "padding-bottom:9px;"
                                                              "}"
                                                              "QScrollBar::handle:vertical"
                                                              "{"
                                                              "width:8px;"
                                                              "background:rgba(0,0,0,25%);"
                                                              " border-radius:4px;"
                                                              "min-height:20;"
                                                              "}"
                                                              "QScrollBar::handle:vertical:hover"
                                                              "{"
                                                              "width:8px;"
                                                              "background:rgba(0,0,0,50%);"
                                                              " border-radius:4px;"
                                                              "min-height:20;"
                                                              "}"
                                                              "QScrollBar::add-line:vertical"
                                                              "{"
                                                              "height:5px;width:10px;"
                                                              "border-image:url(:/new/mainwindow/pictures/Scrollbar_bottom.png);"
                                                              "subcontrol-position:bottom;"
                                                              "}"
                                                              "QScrollBar::sub-line:vertical"
                                                              "{"
                                                              "height:5px;width:10px;"
                                                              "border-image:url(:/new/mainwindow/pictures/Scrollbar_top.png);"
                                                              "subcontrol-position:top;"
                                                              "}"
                                                              "QScrollBar::add-line:vertical:hover"
                                                              "{"
                                                              "height:9px;width:8px;"
                                                              "border-image:url(:/images/a/4.png);"
                                                              "subcontrol-position:bottom;"
                                                              "}"
                                                              "QScrollBar::sub-line:vertical:hover"
                                                              "{"
                                                              "height:9px;width:8px;"
                                                              "border-image:url(:/images/a/2.png);"
                                                              "subcontrol-position:top;"
                                                              "}"
                                                              "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
                                                              "{"
                                                              "background:rgba(0,0,0,5%);"
                                                              "border-radius:4px;"
                                                              "}");
            newItemWidget->setLayout(encryptionViewController->vbox);
            newScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
            //newItemWidget->setSizePolicy(QSizePolicy::Fixed);
            newScrollArea->setWidget(newItemWidget);
            QVBoxLayout *newVbox = new QVBoxLayout();
            newVbox->setMargin(0);
            newVbox->setSpacing(0);
            newVbox->addWidget(newScrollArea);
            encryptionViewController->setLayout(newVbox);
            initLableFlag = false;
            initPageFlag=false;
        }

      for (int i=0;i<file_full_list.size();++i){
          //if ()
        fInfo=QFileInfo(file_full_list.at(i));

        openFileInfo=fInfo;
        //fName=fInfo.fileName();
        std::string name = codec->fromUnicode(fInfo.fileName()).data();
        //name.
        QString fName = codec->fromUnicode(fInfo.fileName()).data();
        fPath=fInfo.filePath();
        fSize=fInfo.size();
        mfSize=(double)(fSize/1024.);//字节转换为KB
        amfSize=QString::number(mfSize,10,2);
        //encryption *contest = new encryption();
        //生成源文件UUID
        QUuid orfile_id =QUuid::createUuid();
        QString orFileID = orfile_id.toString();
        orfileUuid = orFileID;
        //生成密文UUID
        QUuid yzipfile_id =QUuid::createUuid();
        QString yzipFileID = yzipfile_id.toString();
        yzipfileUuid = yzipFileID;
        //contest->fInfo=fInfo;
        //emit starEcptItem(fName);

        EncryptionItem *v1 = new EncryptionItem();

        v1->setObjectName(yzipFileID);
        int fontSize = fontMetrics().width( fName );//获取之前设置的字符串的像素大小

        int pos = 0;int d_count = 0;
        pos = fName.indexOf(".");
        while (pos>-1) {
            d_count++;
            pos = fName.indexOf(".",pos+1);
        }

        QString filetype_extra = fName.section(".",d_count-1,d_count-1).mid(fName.section(".",d_count-1,d_count-1).length()-2)+"."+fName.section(".",d_count,d_count).trimmed().toStdString().c_str() ;
        if( fontSize >= v1->fileName->width() - 100) //与label自身相比较
        {
            QString str = fontMetrics().elidedText( fName, Qt::ElideRight, v1->fileName->width()-150 )+filetype_extra;//返回一个带有省略号的字符串
            v1->fileName->setText( str );       //重新设置label上的字符串
        }else{
            v1->fileName->setText(fName);
        }

        //v1->fileSize->setText(amfSize+codec->toUnicode(" KB"));
        double filesize_double = mfSize;
        int filesize_count = 0;
        while(filesize_double>1024){
            filesize_double = filesize_double/1024;
            filesize_count++;
        }
        if(filesize_count == 0){
            v1->fileSize->setText(QString::number(filesize_double, 10, 2)+"KB");
        }else if(filesize_count == 1){
            v1->fileSize->setText(QString::number(filesize_double, 10, 2)+"MB");
        }else if(filesize_count == 2){
            v1->fileSize->setText(QString::number(filesize_double, 10, 2)+"GB");
        }else if(filesize_count == 3){
            v1->fileSize->setText(QString::number(filesize_double, 10, 2)+"TB");
        }
        QString filetype = fName.section(".",1,1).trimmed().toStdString().c_str();
        if((filetype=="jpg")||(filetype=="png")||(filetype=="jpeg")||(filetype=="bmp")||(filetype=="gif")||(filetype=="webp")||(filetype=="psd")||(filetype=="svg")||(filetype=="tiff")){
            QPixmap pixmap(":/new/mainwindow/pictures/pic_icon.png");
            pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
            v1->fileIcon->setScaledContents(true);
            v1->fileIcon->setPixmap(pixmap);
        }else if((filetype=="avi")||(filetype=="rmvb")||(filetype=="rm")||(filetype=="asf")||(filetype=="divx")||(filetype=="wmv")||(filetype=="mp4")||(filetype=="mkv")||(filetype=="vob")||(filetype=="mpeg")){
            QPixmap pixmap(":/new/mainwindow/pictures/video_icon.png");
            pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
            v1->fileIcon->setScaledContents(true);
            v1->fileIcon->setPixmap(pixmap);
        }else if((filetype=="doc")||(filetype=="docx")||(filetype=="xls")||(filetype=="xlsx")||(filetype=="ppt")||(filetype=="pptx")||(filetype=="txt")||(filetype=="docm")){
            QPixmap pixmap(":/new/mainwindow/pictures/doc_icon.png");
            pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
            v1->fileIcon->setScaledContents(true);
            v1->fileIcon->setPixmap(pixmap);
        }else if((filetype=="rar")||(filetype=="zip")||(filetype=="arj")||(filetype=="z")){
            QPixmap pixmap(":/new/mainwindow/pictures/zip_icon.png");
            pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
            v1->fileIcon->setScaledContents(true);
            v1->fileIcon->setPixmap(pixmap);
        }else if((filetype=="mp3")||(filetype=="wma")||(filetype=="wav")||(filetype=="ape")||(filetype=="flac")||(filetype=="ogg")||(filetype=="aac")){
            QPixmap pixmap(":/new/mainwindow/pictures/music_icon.png");
            pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
            v1->fileIcon->setScaledContents(true);
            v1->fileIcon->setPixmap(pixmap);
        }else{
            QPixmap pixmap(":/new/mainwindow/pictures/else_icon.png");
            pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
            v1->fileIcon->setScaledContents(true);
            v1->fileIcon->setPixmap(pixmap);
        }

        v1->encryptStaBtn->setObjectName(yzipFileID);
        v1->encryptStaBtn->show();
        v1->encryptStaBtn->setEnabled(false);
        v1->encryptStaBtn->setText("加密中...");
        v1->encryptStaBtn->setFlat(true);
        v1->encryptStaBtn->setStyleSheet("background:transparent;text-align: left;");
        //v1->encryptStaBtn->t
        v1->starEncptBtn->setFlat(true);
        //v1->checkBox->setObjectName(fName);

        //qDebug()<<v1->checkBox->objectName();

        v1->progressBar->setObjectName(yzipFileID);
        encryptionViewController->vbox->addWidget(v1);

        //ecptProgressbarItem *v2 = new ecptProgressbarItem();

        //encryptionViewController2->vbox->addWidget(v2);

        f_progressBar = new QProgressBar();
        f_progressBar = v1->progressBar;
        //f_progressBar->raise();
        v1->fileIcon->stackUnder(f_progressBar);

        f_progressBar->setGeometry(0,0,700,75);
        f_progressBar->setTextVisible(false);
        QString strQSS = "QProgressBar { \
                text-align: center; \
                border: 1px ; \
                background: transparent; \
            } \
            QProgressBar::chunk { \
                background-color: rgba(230, 277, 255,180); \
            }";
//        f_progressBar->setStyleSheet(strQSS);
        f_progressBar->setObjectName(yzipFileID);
        //emit starEcptItem(fName);
        //connect(v1,SIGNAL(starEncptItem(QString)),on_OpenFileBtn_clicked(),SLOT(startProgressBarThread(QString)));
//        f_progressBar->setMinimum(0);
//        f_progressBar->setMaximum(100);
//        //f_progressBar->setValue(20);
//        f_progressBar->setOrientation(Qt::Horizontal);
//        f_progressBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);  // 对齐方式
        connect(v1->encryptStaBtn,SIGNAL(clicked(bool)),this,SLOT(on_encryptStaBtn_clicked()));


        connect(v1->starEncptBtn,SIGNAL(clicked(bool)),this,SLOT(on_starEncptBtn_clicked()));

        v1->starEncptBtn->clicked(true);
        //starEcpt(fName);
        //startEncryptThread(fName);
        //startProgressBarThread(fName);
        //encryptionViewController->vbox->addWidget(f_progressBar);
        //encryptionViewController->vbox->setGeometry(100);

        //开始加密
        //encptThreadArr[encptThreadNum] = new encryptthread(this);

        //connect(ui->OpenFileBtn, SIGNAL(clicked(bool)), encptThreadArr[encptThreadNum], SLOT(startProgressBarThread()));
        //encptThreadArr[encptThreadNum]->start();
        //encptThreadNum++;

        delete encryptionViewController->layout();
        QWidget *newItemWidget = new QWidget();
        newItemWidget->setContentsMargins(0,0,0,0);

        QScrollArea *newScrollArea = new QScrollArea();
        //newScrollArea->setWidgetResizable(true);//铺满显示
//        newScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        newItemWidget->setLayout(encryptionViewController->vbox);
        newScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
        //newItemWidget->setSizePolicy(QSizePolicy::Fixed);
        newScrollArea->setWidget(newItemWidget);
        QVBoxLayout *newVbox = new QVBoxLayout();
        newVbox->setMargin(0);
        newVbox->setSpacing(0);
        newVbox->addWidget(newScrollArea);
        encryptionViewController->setLayout(newVbox);

        }
        }
    }
}


// 更新进度条
void MainWindow::handleResults(int value,QString itemName,double debugTime,double uploadTime)
{
    EncryptionItem *v1 = ui->MidStaWidget->findChild<EncryptionItem*>(itemName);
    //qDebug()<<"进度条name"<<itemName;
    //QProgressBar *f_progressBar = f_progressBar->findChild<QProgressBar *>(itemName);
    //f_progressBar->setValue(value);
    //进度条效果
    v1->m_proAnimation->setStartValue(v1->m_proAnimation->endValue());
    v1->m_proAnimation->setEndValue(value);
    v1->m_proAnimation->setDuration(300);
    v1->m_proAnimation->start();
    //v1->progressBar->setValue(value);
    //v1->progressBar->setMaximum(value);
    //v1->progressBar->setMinimum(value);
    //qDebug()<<"VVV"<<value;
    //v1->encryptStaBtn->setText("上传中...");
    if (value>26){
        v1->encryptStaBtn->setText("上传中...");
        v1->encryptStaBtn->setStyleSheet("background:transparent;text-align: left;");
    }

    if (value>=100){
        v1->m_proAnimation->setEndValue(100);
        qDebug()<<"删除"<<itemName;
        v1->encryptStaBtn->clicked();
        //QMessageBox::information(NULL,tr("成功"),tr("加密完成！"),QMessageBox::Yes,NULL);
        itemName = itemName.toUtf8();

        QString str  ;
        str.append("加密已完成！");
        //str.toStdString();

        QString temstr ;
        temstr.sprintf("%s%s%s","加密已完成！","\n","加密文件耗时：");

        int ss = 1;
        int mi = ss * 60;
        int hh = mi * 60;
        long hour = debugTime / hh;
        long minute = (debugTime - hour * hh) / mi;
        long second = (debugTime - hour * hh - minute * mi) / ss;
        long milliSecond = debugTime - hour * hh - minute * mi - second * ss;

        QString hou = QString::number(hour,10);
        QString min = QString::number(minute,10);
        QString sec = QString::number(second,10);
        QString msec = QString::number(milliSecond,10);

        if (debugTime < 60){
            temstr.append(QString::number(debugTime,10,2));
            temstr.append("秒");
        }else{
            //显示时间


            //int dd = hh * 24;

            //long day = ms / dd;


            if (hour >0){
                temstr.append(hou);
                temstr.append("时");
            }
            if (minute >0){
                temstr.append(min);
                temstr.append("分");
            }
            if (second >0){
                temstr.append(sec);
                temstr.append("秒");
            }
        }

        temstr.append("\n");
        temstr.append("上传文件耗时：");
        if (uploadTime < 60){

            temstr.append(QString::number(uploadTime,10,2));
            temstr.append("秒");
        }else{
            hour = uploadTime / hh;
            minute = (uploadTime - hour * hh) / mi;
            second = (uploadTime - hour * hh - minute * mi) / ss;
            hou = QString::number(hour,10);
            min = QString::number(minute,10);
            sec = QString::number(second,10);

            if (hour >0){
                temstr.append(hou);
                temstr.append("时");
            }
            if (minute >0){
                temstr.append(min);
                temstr.append("分");
            }
            if (second >0){
                temstr.append(sec);
                temstr.append("秒");
            }
        }

        //v1->progressBar->setValue(100);
        //QString *temstr = "加密已完成！"+"加密时间："+ QString::number( debugTime,10)+"s   上传时间："+ QString::number(uploadTime,10)+"s";
        MsgBox *msgbox = new MsgBox(4,temstr,this);
        msgbox->exec();
///////////////////////////////删除加密完成的项目
        //EncryptionItem *v1 = ui->MidStaWidget->findChild<EncryptionItem*>(f_progressBar->objectName());
        //qDebug()<<name;
        delete v1;
        //ReLayout();
        delete encryptionViewController->layout();
        QWidget *newItemWidget = new QWidget();
        newItemWidget->setContentsMargins(0,0,0,0);
        QScrollArea *newScrollArea = new QScrollArea();

        newItemWidget->setLayout(encryptionViewController->vbox);
        newScrollArea->setWidget(newItemWidget);
        newScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
        QVBoxLayout *newVbox = new QVBoxLayout();
        newVbox->setMargin(0);
        newVbox->setSpacing(0);
        newVbox->addWidget(newScrollArea);
        encryptionViewController->setLayout(newVbox);


///////////////////////////////////////////////////////////////////////
        //initPageFlag=true;
        if (encryptionViewController->vbox->count()==0){
//            ui->FinishedBtn->close();
            if (isFinishedBtn == 0)
                ui->FinishedBtn->clicked(true);
           ui->FinEnpBtn->clicked(true);
           initPageFlag = true;
         }

    }
    if (value==0){
        //QMessageBox::information(NULL,tr("失败！"),tr("网络连接错误！"),QMessageBox::Yes,NULL);
        MsgBox *msgbox = new MsgBox(2,QStringLiteral("网络连接错误！"),this);
        msgbox->exec();
        EncryptionItem *v1 = ui->MidStaWidget->findChild<EncryptionItem*>(f_progressBar->objectName());
        delete v1;
        //delete f_progressBar;
        delete encryptionViewController->layout();
        QWidget *newItemWidget = new QWidget();
        newItemWidget->setContentsMargins(0,0,0,0);
        QScrollArea *newScrollArea = new QScrollArea();
        newItemWidget->setLayout(encryptionViewController->vbox);
        newScrollArea->setWidget(newItemWidget);
        newScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
        QVBoxLayout *newVbox = new QVBoxLayout();
        newVbox->setMargin(0);
        newVbox->setSpacing(0);
        newVbox->addWidget(newScrollArea);
        encryptionViewController->setLayout(newVbox);
    }
}

void MainWindow::starEcpt(QString fName){
    //connect()
    //connect(this, SIGNAL(starEncptItem(QString)), this, SLOT(startProgressBarThread(QString)));
    emit starEcptItem(fName);

}

void MainWindow::on_encryptStaBtn_clicked(){
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString name = button->objectName();
    EncryptionItem *v1 = ui->MidStaWidget->findChild<EncryptionItem*>(name);
    //v1->fileDescription->setText("加密已完成！");
    v1->encryptStaBtn->show();
    v1->encryptStaBtn->setEnabled(false);
    v1->encryptStaBtn->setText("加密已完成！");
    v1->encryptStaBtn->setStyleSheet("background:transparent;text-align: left;");
}

void MainWindow::on_starEncptBtn_clicked(){
    //QString fName = openFileInfo.fileName();
    QString fName = yzipfileUuid;
    startProgressBarThread(fName,openFileInfo);
}

// 开启进度条
void MainWindow::startProgressBarThread( QString  itemName,QFileInfo fileInfo)
{
    if (fileOpenFlag){

        //encryptthread *ecry = new encryptthread(this);
        file_item_name = itemName;
        file_item_fileInfo = fileInfo;
        //ecry->itemName = itemName;
        //enItemThread *enit = new enItemThread(this);
        //enit->item = itemName;
        enitemArr[enitemNum] = new enItemThread(this);
        //enItemThread *workerThread = new enItemThread(this);
        connect(enitemArr[enitemNum], SIGNAL(resultReady(int,QString,double,double)), this, SLOT(handleResults(int,QString,double,double)));
        // 线程结束后，自动销毁
       connect(enitemArr[enitemNum], SIGNAL(finished()), enitemArr[enitemNum], SLOT(deleteLater()));
       enitemArr[enitemNum]->start();
       enitemNum++;
        //startEncryptThread();
        encptThreadArr[encptThreadNum] = new encryptthread(this);
        //encryptthread *ecpThread = new encryptthread(this);
        connect(encptThreadArr[encptThreadNum],SIGNAL(result(int,QString,double,double)),this,SLOT(handleResults(int,QString,double,double)));
        // 线程结束后，自动销毁
        connect(encptThreadArr[encptThreadNum], SIGNAL(finished()), encptThreadArr[encptThreadNum], SLOT(deleteLater()));
        //ecpThread->start();
        encptThreadArr[encptThreadNum]->encpt_rate = extraction_rate;
        encptThreadArr[encptThreadNum]->start();
        encptThreadNum++;
    }

}

//开始加密
void MainWindow::startEncryptThread(QString itemName){
    encryptthread *ecpThread = new encryptthread(this);
    connect(ecpThread,SIGNAL(result(int)),this,SLOT(handleResults(int)));
    // 线程结束后，自动销毁
    connect(ecpThread, SIGNAL(finished()), ecpThread, SLOT(deleteLater()));
    ecpThread->start();
}

//批量删除按钮
void MainWindow::on_pushButton_3_clicked()

{
    int flag = 0;
    QSqlQuery checkQuery(db);
    bool checkSuccess = checkQuery.exec("select * from Decryption where oemp_id='"+User_ID+"'");
    if(!checkSuccess){
        qDebug()<<"查询user失败";
        return;
    }
    else{
        while(checkQuery.next()){
            QString checkID = checkQuery.record().value("id").toString();
            QCheckBox *IsCheck = ui->MidStaWidget->findChild<QCheckBox*>(checkID+"Decheck");
            if(IsCheck!=nullptr){
                if(IsCheck->isChecked())
                    flag = 1;
            }
            else{
                continue;
            }

        }
    }
    if(flag==1){
        MsgBox *msgbox = new MsgBox(1,QStringLiteral("确认删除这些已加密条目吗？"),this);
        int nRes = msgbox->exec();
        if (nRes == QDialog::Accepted){
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
                      if(check!=nullptr){//判断是否找到相应控件
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
                  decryptionViewController->vbox->setMargin(0);
                  decryptionViewController->vbox->setSpacing(0);
                  QWidget *newItemWidget = new QWidget();
                  newItemWidget->setContentsMargins(0,0,0,0);
                  newItemWidget->setLayout(decryptionViewController->vbox);
                  newScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
                  newScrollArea->setWidget(newItemWidget);
//                  newScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                  QVBoxLayout *newVbox = new QVBoxLayout();
                  newVbox->setMargin(0);
                  newVbox->setSpacing(0);
                  newVbox->addWidget(newScrollArea);
                  decryptionViewController->setLayout(newVbox);
        }
    }
    }else if(flag == 0 ){
        MsgBox *msgbox = new MsgBox(3,QStringLiteral("请选择需要删除的条目"),this);
        msgbox->exec();
    }
    flag = 0;
}
//点击全选按钮 QCheckbox处于被选择状态
void MainWindow::on_pushButton_clicked()
{
    QSqlQuery query(db);
    bool success = query.exec("select * from Decryption where oemp_id = '"+User_ID+"'");
    if(!success){
        qDebug()<<"checkBox:查询数据库失败";
        return;
    }
    else{
        while(query.next()){
            QString m_ID = query.record().value("id").toString();
            QCheckBox *checkbox = ui->MidStaWidget->findChild<QCheckBox *>(m_ID+"Decheck");
            if(checkbox==nullptr){
                continue;
            }
            else{
                if(DeSelect_flag==0){
                    checkbox->setChecked(true);
                }
                else if(DeSelect_flag == 1){
                    checkbox->setChecked(false);
                }
            }
        }
        if(DeSelect_flag ==0){
            DeSelect_flag =1;
            ui->pushButton->setStyleSheet(" QPushButton {border-image: url(:/new/mainwindow/pictures/allselect_pressed.png);} "
                                          " QPushButton:hover {border-image: url(:/new/mainwindow/pictures/allselect_hover.png);}");
        }
        else{
            DeSelect_flag =0;
            ui->pushButton->setStyleSheet(" QPushButton {border-image: url(:/new/mainwindow/pictures/allselect.png);} "
                                             " QPushButton:hover {border-image: url(:/new/mainwindow/pictures/allselect_hover.png);}");
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
                          QDateTime time = QDateTime::currentDateTime();
                          QString time_str = time.toString("yyyy-MM-dd hh:mm:ss");//获取当前时间
                          QSqlQuery updatetime(db);
                          bool updateTimeSuccess = updatetime.exec("update Decryption set apply_time = '"+time_str+"' where id ='"+fileID+"'");
                          if(!updateTimeSuccess){
                              qDebug()<<"update failed";
                          }
                          m1->downloadBtn->setText("申请中");
                          m1->fileDescription->setText("正在等待对方应答，请等待！");
                          disconnect(m1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(getFileID()));
                          MsgBox *msgbox = new MsgBox(4,QStringLiteral("申请成功，请等待！"),this);
                          msgbox->exec();
                      }
                      else{
                          MsgBox *msgbox = new MsgBox(2,QStringLiteral("申请失败！"),this);
                          msgbox->exec();
                      }
                  }
              }
        }

}

void MainWindow::OssDownLoadFile(){
    //创建CloundSafe 主目录
    QDir dir;
    dir.cd("C://CloundSafe");  //进入某文件夹
    if(!dir.exists("C://CloundSafe"))//判断需要创建的文件夹是否存在
    {
        dir.mkdir("C://CloundSafe"); //创建文件夹
    }
    //创建用户子目录
    QString qq_Path = "C://CloundSafe//"+User_qqNum;
    dir.cd(qq_Path);  //进入某文件夹
    if(!dir.exists(qq_Path))//判断需要创建的文件夹是否存在
    {
        dir.mkdir(qq_Path); //创建文件夹
    }
//    User_qqPath = qq_Path;//给User_qqPath赋值
    //创建Decrypt子目录
    QString file_path = qq_Path+"//Decrypt";
    dir.cd(file_path);
    if(!dir.exists(file_path)){
        dir.mkdir(file_path);
    }
    QString path_content = file_path+"//content";
    dir.cd(path_content);
    if(!dir.exists(path_content)){
        dir.mkdir(path_content);
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
                    if(f_proccess==nullptr){
                        qDebug()<<"progressBARISnull";
                        return;
                    }
                    f_proccess->show();
                    f_proccess->setValue(20);
                    downThread[threadNum] = new DepDownThread();
//                    DecProFileID = onlyId;
                    decryBarThread[DecryBarNum] = new DecryptProBarThread(this);
                    decryBarThread[DecryBarNum]->ItemID = onlyId;
//                    connect(this,SIGNAL(DecryProBarID(QString)),decryBarThread[DecryBarNum],SLOT(setItemID(QString)));
                    connect(decryBarThread[DecryBarNum],SIGNAL(reslut(int,QString)),this,SLOT(ChangeDecItemProBar(int,QString)));
                    connect(decryBarThread[DecryBarNum],SIGNAL(finished()),decryBarThread[DecryBarNum],SLOT(deleteLater()));
                    connect(downThread[threadNum],SIGNAL(finished()),downThread[threadNum],SLOT(deleteLater()));
                    connect(this,SIGNAL(OSSfileDownFileID(QString,QString)),downThread[threadNum],SLOT(DownContent(QString,QString)));
                    connect(downThread[threadNum],SIGNAL(ChangeBtnText(QString)),decryBarThread[DecryBarNum],SLOT(run()));
                    connect(downThread[threadNum],SIGNAL(sendTime(QString,double)),this,SLOT(setDecItemDowntime(QString,double)));
//                    emit OSSfileDownFileID(onlyId,enkey_id);
                    //给onlyId enkey_id赋值
                    downThread[threadNum]->dekey_id=enkey_id;
                    downThread[threadNum]->d_id=onlyId;
                    qDebug()<<"mainwindow中的fileID为："+onlyId;
                    downThread[threadNum]->start();
//                     emit DecryProBarID(onlyId);
//                    decryBarThread[DecryBarNum]->start();

                    threadNum++;
                    DecryBarNum++;
                    //更新按钮内容
                    DecryptionItem *m1 = ui->MidStaWidget->findChild<DecryptionItem*>(onlyId+"decryption");
                    m1->downloadBtn->setText("正在下载");
                    m1->fileDescription->setText("下载中...");
                    disconnect(m1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(OssDownLoadFile()));//删除原有信号槽
                }
                else{
                    MsgBox *msgbox = new MsgBox(2,QStringLiteral("下载失败！"),this);
                    msgbox->exec();
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
    QSqlQuery query(db);
    QString newID;
    bool success = query.exec("select * from Decryption where oemp_id='"+User_ID+"' order by createtime DESC");
    if(!success){
        qDebug() << "查询user失败";
        return;
     }else{
              qDebug()<<"查询成功";
              //将数据库查到的数据添加到视图中
              decryptionViewController->vbox = new QVBoxLayout();//将原有视图清空
              while(query.next()){
                  DecryptionItem *v1 =  new DecryptionItem();

                  QString fName;
                  fName = query.record().value("file_name").toString();
                  QString rate = query.record().value("file_encrypt_num").toString();
                  QString file_level;
                  if(rate == "20"){
                      file_level = "加密等级：强";
                  }else if(rate == "30"){
                      file_level = "加密等级：中";
                  }else {
                      file_level = "加密等级：弱";
                  }
                  v1->fileLevel->setText(file_level);
                  int fontSize = fontMetrics().width( fName );//获取之前设置的字符串的像素大小
                  int pos = 0;int d_count = 0;
                  pos = fName.indexOf(".");
                  while (pos>-1) {
                      d_count++;
                      pos = fName.indexOf(".",pos+1);
                  }
                  QString filetype_extra = fName.section(".",d_count-1,d_count-1).mid(fName.section(".",d_count-1,d_count-1).length()-2)+"."+fName.section(".",d_count,d_count).trimmed().toStdString().c_str() ;
                  if( fontSize >= v1->fileName->width()-100 ) //与label自身相比较
                  {
                      QString str = fontMetrics().elidedText( fName, Qt::ElideRight, v1->fileName->width()-150 )+filetype_extra;//返回一个带有省略号的字符串
                      v1->fileName->setText( str );       //重新设置label上的字符串
                  }else{
                      v1->fileName->setText(fName);
                  }
                  QString m_filesize = query.record().value("file_size").toString();
                  double filesize_double = m_filesize.toDouble();
                  int filesize_count = 0;
                  while(filesize_double>1024){
                      filesize_double = filesize_double/1024;
                      filesize_count++;
                  }
                  if(filesize_count == 0){
                      v1->fileSize->setText(QString::number(filesize_double, 10, 2)+"KB");
                  }else if(filesize_count == 1){
                      v1->fileSize->setText(QString::number(filesize_double, 10, 2)+"MB");
                  }else if(filesize_count == 2){
                      v1->fileSize->setText(QString::number(filesize_double, 10, 2)+"GB");
                  }else if(filesize_count == 3){
                      v1->fileSize->setText(QString::number(filesize_double, 10, 2)+"TB");
                  }

                  v1->timeLabel->setText(query.record().value("createtime").toString());//设置创建时间
                  //设置fileIcon的图片
                  QString filetype = query.record().value("file_name").toString().section(".",1,1).trimmed().toStdString().c_str();
                  if((filetype=="jpg")||(filetype=="png")||(filetype=="jpeg")||(filetype=="bmp")||(filetype=="gif")||(filetype=="webp")||(filetype=="psd")||(filetype=="svg")||(filetype=="tiff")){
                      QPixmap pixmap(":/new/mainwindow/pictures/pic_icon.png");
                      pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                      v1->fileIcon->setScaledContents(true);
                      v1->fileIcon->setPixmap(pixmap);
                  }else if((filetype=="avi")||(filetype=="rmvb")||(filetype=="rm")||(filetype=="asf")||(filetype=="divx")||(filetype=="wmv")||(filetype=="mp4")||(filetype=="mkv")||(filetype=="vob")||(filetype=="mpeg")){
                      QPixmap pixmap(":/new/mainwindow/pictures/video_icon.png");
                      pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                      v1->fileIcon->setScaledContents(true);
                      v1->fileIcon->setPixmap(pixmap);
                  }else if((filetype=="doc")||(filetype=="docx")||(filetype=="xls")||(filetype=="xlsx")||(filetype=="ppt")||(filetype=="pptx")||(filetype=="txt")||(filetype=="docm")){
                      QPixmap pixmap(":/new/mainwindow/pictures/doc_icon.png");
                      pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                      v1->fileIcon->setScaledContents(true);
                      v1->fileIcon->setPixmap(pixmap);
                  }else if((filetype=="rar")||(filetype=="zip")||(filetype=="arj")||(filetype=="z")){
                      QPixmap pixmap(":/new/mainwindow/pictures/zip_icon.png");
                      pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                      v1->fileIcon->setScaledContents(true);
                      v1->fileIcon->setPixmap(pixmap);
                  }else if((filetype=="mp3")||(filetype=="wma")||(filetype=="wav")||(filetype=="ape")||(filetype=="flac")||(filetype=="ogg")||(filetype=="aac")){
                      QPixmap pixmap(":/new/mainwindow/pictures/music_icon.png");
                      pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                      v1->fileIcon->setScaledContents(true);
                      v1->fileIcon->setPixmap(pixmap);
                  }else{
                      QPixmap pixmap(":/new/mainwindow/pictures/else_icon.png");
                      pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                      v1->fileIcon->setScaledContents(true);
                      v1->fileIcon->setPixmap(pixmap);
                      v1->elseLabel->setText(filetype.left(3));
                      v1->elseLabel->raise();
                  }

                  v1->checkBox->setObjectName((query.record().value("id").toString())+"Decheck");//设置checkbox的ID
                  v1->setObjectName(query.record().value("id").toString()+"decryption");//设置Item的ID
                  v1->downloadBtn->setObjectName((query.record().value("id").toString())+"btn");//设置downloadBtn的ID
                  v1->downloadBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: #3A8CFF;color:white;}QPushButton:pressed{background-color: rgb(139,159,185);}");
                  //连接信号槽
//                  connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                  if(query.record().value("status").toString()=="0"){
                      //查找新消息
                      int isNEW = 1;
                      QString Infor_ID = query.record().value("id").toString();
                      for(int i=0;i<RequsetIndex;i++){
                          if(Infor_ID==RequestIDArray[i]){
                              isNEW =0;
                              break;
                          }
                      }
                      if(isNEW ==1){

                          //如果是新消息则发送信号为InforDlg
                          RequestIDArray[RequsetIndex]=Infor_ID;//将该ID加入到数组中
                          RequsetIndex++;
                          //发送信号给InforDlg让其重新布局
                          QString SendUserid = query.record().value("emp_id").toString();
                          QString SendFileName = query.record().value("file_name").toString();
                          QString time = query.record().value("createtime").toString();
                          QSqlQuery nameQuery(db);
                          bool success1 =nameQuery.exec("select * from employee where emp_id = '"+SendUserid+"'");
                          if(!success1){
                              qDebug()<<"查询发送者ID失败";
                          }
                          else{
                              while (nameQuery.next()) {
                                  QString SendID = nameQuery.record().value("emp_name").toString();
                                  if(IsLinkInfor ==1){
                                      IsLinkInfor = 0;
                                  }
                                  else{
                                      Infor_requestNum++;
                                      emit SendInforToInforDlg(SendID,SendFileName,time);//发送信号
                                  }


                              }
                          }
                          qDebug()<<"新消息ID为:";
                          qDebug()<<Infor_ID;
                          newID = Infor_ID;
                          emit showDownDialog(newID);
                      }
                      v1->fileDescription->setText("主体文件指定分享需确认下载.");
                      v1->downloadBtn->setText("确认下载");
                      connect(v1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(OssDownLoadFile()));
//                      connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                      decryptionViewController->vbox->addWidget(v1);//将v1添加到视图中
                      f_progressBar = new QProgressBar(this);
                      f_progressBar = v1->progressBar;
                      f_progressBar->raise();
                      f_progressBar->setObjectName(v1->objectName());
                      f_progressBar->hide();
                      f_progressBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);  // 对齐方式
                      //decryptionViewController->vbox->addWidget(f_progressBar);
                  }
                  else if(query.record().value("status").toString()=="1"){
                      v1->fileDescription->setText("文件已加密需下载密钥文件.");
                      v1->downloadBtn->setText("申请解密");
                      v1->label->show();
                      connect(v1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(getFileID()));
 //                     connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                      decryptionViewController->vbox->addWidget(v1);//将v1添加到视图中
                  }
                  else if(query.record().value("status").toString()=="2"){//申请等待状态
                      v1->fileDescription->setText("正在申请解密，请等待！");
                      v1->downloadBtn->setText("申请中");
                      v1->label->show();
//                      connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                      decryptionViewController->vbox->addWidget(v1);
                      }else if(query.record().value("status").toString()=="11"){//安卓端本地解密
                      v1->fileDescription->setText("文件已加密需下载密钥文件.");
                      v1->downloadBtn->setText("申请解密");
                      v1->fileSize->setText("文件来自安卓系统");
                      v1->downloadBtn->setObjectName(query.record().value("file_id").toString());
                      v1->label->show();
//                       connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                      connect(v1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(androidDecrypt()));
                      decryptionViewController->vbox->addWidget(v1);//将v1添加到视图中
                      qDebug()<<"android!!!!!!!!!";
                  }else if(query.record().value("status").toString()=="21"){//ios端本地解密
                      v1->fileDescription->setText("文件已加密需下载密钥文件.");
                      v1->downloadBtn->setText("申请解密");
                      v1->fileSize->setText("文件来自iOS系统");
                      v1->downloadBtn->setObjectName(query.record().value("file_id").toString());
                      v1->label->show();
//                       connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                      connect(v1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(getFileID()));
                      decryptionViewController->vbox->addWidget(v1);//将v1添加到视图中
                  }else if(query.record().value("status").toString()=="12"){//安卓端本地解密第二步
                      v1->fileDescription->setText("已申请解密！");
                      v1->downloadBtn->setText("解密");
                      v1->fileSize->setText("文件来自安卓系统");
                      v1->downloadBtn->setObjectName(query.record().value("file_id").toString());
                      v1->label->show();
                      connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                      decryptionViewController->vbox->addWidget(v1);
                  }else if(query.record().value("status").toString()=="22"){//ios端本地解密第二步
                      v1->fileDescription->setText("已申请解密！");
                      v1->downloadBtn->setText("解密");
                      v1->fileSize->setText("文件来自iOS系统");
                      v1->downloadBtn->setObjectName(query.record().value("file_id").toString());
                      v1->label->show();
                      connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                      decryptionViewController->vbox->addWidget(v1);
                  }
              }
              ReLayout();
              int newRequestNum = Infor_requestNum+informationNum+FriendRequestCount;
              QString s = QString::number(newRequestNum,10);
              qDebug()<<"Infor_num_icon:";
              qDebug()<<s;
              if(newRequestNum==0){
                  Infor_num_icon->hide();
              }
              else{
                  Infor_num_icon->show();
                  Infor_num_icon->setText(s);
              }


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
                if(query.record().value("status").toString()=='1'){
                    MsgBox *msgbox = new MsgBox(3,QStringLiteral("已经是你的好友了！"),this);
                    msgbox->exec();
                    return;
                }else if(query.record().value("status").toString()=='2'){
                    //之前添加好友 但被好友拒绝 此时将status置为0 重新添加好友
                    QSqlQuery statusQuery(db);
                    bool statusSuccess = statusQuery.exec("update friend set status = '0' and is_solved = '0' where user_id ='"+User_ID+"' and friend_nickname ='"+name+"'");
                    if(!statusSuccess){
                        qDebug()<<"addFriendToDatabase:update status = 0 failed";
                    }
                    else{
                        qDebug()<<"addFriendToDatabase: update success";
                        return;
                    }
                }else if(query.record().value("status").toString()=='0'){
                    MsgBox *msgbox = new MsgBox(3,QStringLiteral("请等待好友处理申请"),this);
                    msgbox->exec();
                    return;
                }


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
            MsgBox *msgbox = new MsgBox(2,QStringLiteral("好友不存在"),this);
            msgbox->exec();
            return;
        }
        else{
            while (query.next()) {
                if(query.record().value("emp_id").toString().isEmpty()){
                    MsgBox *msgbox = new MsgBox(2,QStringLiteral("好友不存在"),this);
                    msgbox->exec();
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
        MsgBox *msgbox = new MsgBox(2,QStringLiteral("好友不存在"),this);
        msgbox->exec();
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
    QDateTime time = QDateTime::currentDateTime();
    QString time_str = time.toString("yyyy-MM-dd hh:mm:ss");//获取此刻时间
    bool insertSuccess = query.exec("insert into friend values('"+strID+"','"+User_ID+"','"+friendname+"','"+userid+"','"+name+"',0,'"+time_str+"',0)");
    if(insertSuccess){
//        //将好友插入到视图当中
//        int i = friendListWidget->count();
//        i++;
//        QListWidgetItem *add_item = new QListWidgetItem(friendListWidget);
//        add_item->setIcon(QIcon("://pictures/userIcon_1.png"));
//        add_item->setText(name);
//        add_item->setTextAlignment(Qt::AlignLeft|Qt::AlignCenter);
//        add_item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
//        add_item->setSizeHint(QSize(ui->RightWidget->width()-30,34));
//        friendListWidget->insertItem(i,add_item);
        MsgBox *msgbox = new MsgBox(4,QStringLiteral("发送请求成功！"),this);
        msgbox->exec();
    }
    else{
        MsgBox *msgbox = new MsgBox(2,QStringLiteral("添加好友失败！"),this);
        msgbox->exec();
    }
}

void MainWindow::inforDlgaddFriend(QString name){
    //判断是否是好友关系
    QSqlQuery query(db);
    bool judgeSuccess = query.exec("select * from friend where user_id='"+User_ID+"'");
    if(!judgeSuccess){
        qDebug()<<"查询失败";
    }
    else{
        while (query.next()) {
            if(query.record().value("friend_nickname").toString()==name){
                if(query.record().value("status").toString()=='1'){//status为1说明已经成功添加好友
                    MsgBox *msgbox = new MsgBox(2,QStringLiteral("已经是你的好友了"),this);
                    msgbox->exec();
                    return;
                }
                else if(query.record().value("status").toString()=='2'){//status为2说明之前添加但被忽略，此时将status置为1，并添加好友
                    QSqlQuery updateQuery(db);
                    bool updateSuccess = updateQuery.exec("update friend set status = '1' where user_id = '"+User_ID+"' and friend_nickname = '"+name+"'");
                    if(!updateSuccess){
                        qDebug()<<"inforDlgaddFriend:update friend failed";
                    }
                    else{
                        int i = friendListWidget->count();
                        i++;
                        QListWidgetItem *add_item = new QListWidgetItem(friendListWidget);
                        add_item->setIcon(QIcon(":/new/mainwindow/pictures/FriendListIcon_new.png"));
                        add_item->setText(name);
                        add_item->setFont(m);
                        add_item->setTextAlignment(Qt::AlignLeft|Qt::AlignLeft);
                        add_item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                        add_item->setSizeHint(QSize(ui->RightWidget->width()-45,34));
                        add_item->setTextAlignment(Qt::AlignVCenter);
                        friendListWidget->insertItem(i,add_item);
                        MsgBox *msgbox = new MsgBox(4,QStringLiteral("添加好友成功！"),this);
                        msgbox->exec();
                        return;

                    }
                }
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
            MsgBox *msgbox = new MsgBox(2,QStringLiteral("好友不存在！"),this);
            msgbox->exec();
            return;
        }
        else{
            while (query.next()) {
                if(query.record().value("emp_id").toString().isEmpty()){
                    MsgBox *msgbox = new MsgBox(2,QStringLiteral("好友不存在！"),this);
                    msgbox->exec();
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
        MsgBox *msgbox = new MsgBox(2,QStringLiteral("好友不存在!"),this);
        msgbox->exec();
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
    QDateTime time = QDateTime::currentDateTime();
    QString time_str = time.toString("yyyy-MM-dd hh:mm:ss");//获取此刻时间
    bool insertSuccess = query.exec("insert into friend values('"+strID+"','"+User_ID+"','"+friendname+"','"+userid+"','"+name+"',1,'"+time_str+"',0)");
    if(insertSuccess){
        //将好友插入到视图当中
        int i = friendListWidget->count();
        i++;
        QListWidgetItem *add_item = new QListWidgetItem(friendListWidget);
        add_item->setIcon(QIcon(":/new/mainwindow/pictures/FriendListIcon_new.png"));
        add_item->setText(name);
        add_item->setFont(m);
        add_item->setTextAlignment(Qt::AlignLeft|Qt::AlignLeft);
        add_item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        add_item->setSizeHint(QSize(ui->RightWidget->width()-45,34));
        add_item->setTextAlignment(Qt::AlignVCenter);
        friendListWidget->insertItem(i,add_item);
        MsgBox *msgbox = new MsgBox(4,QStringLiteral("添加好友成功！"),this);
        msgbox->exec();
    }
    else{
        MsgBox *msgbox = new MsgBox(2,QStringLiteral("添加好友失败！"),this);
        msgbox->exec();
    }
}


//头像点击槽函数
void MainWindow::HeadClickedSlot(){
//    QPixmap pixmap(":/new/src/head1");
//    pixmap.scaled(userHead->size(),Qt::KeepAspectRatio);
//    userHead->setScaledContents(true);
//    userHead->setPixmap(pixmap);
    Infor_requestNum = 0;
    InforNum_Changed();

    inforDlg->show();
    inforDlg->raise();
}


//显示添加好友窗口槽函数
void MainWindow::showAddfriendWidget(){

        if(AddFriendFlag)//如果存在
        {
//            MsgBox *msgbox = new MsgBox(4,QStringLiteral("已有一个添加好友窗口哟！"),this);
//            msgbox->exec();
        }else{
            friendInputDlg *friendAdd = new friendInputDlg();
            connect(friendAdd,SIGNAL(sendNameToMain(QString)),this,SLOT(addFriendToDatabase(QString)));
            friendAdd->show();
            AddFriendFlag = 1;
        }
}


void MainWindow::on_pushButton_4_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    QString originalText = clipboard->text().section("&&",1,1);
    if(originalText.startsWith("||",Qt::CaseSensitive)){
        MsgBox *msgbox = new MsgBox(1,QStringLiteral("是否读取剪贴板中的链接信息？"),this);
        int nRes = msgbox->exec();
        if (nRes == QDialog::Accepted){
            QString split_file_id;
            QStringList list = originalText.split("||");
            for(int j = 1;j<list.count();j++){
                split_file_id = list[j];
                LinkInsert(split_file_id);
            }
//        qDebug()<<originalText;
//        MsgBox *msgbox = new MsgBox(4,QStringLiteral("已读取链接信息！"),this);
//        msgbox->exec();
        }else{
            if(LinkInsertFlag){
            }else{
            linkDialog = new DelinkDialog();
            connect(linkDialog,SIGNAL(sendLinkToMain(QString)),this,SLOT(LinkInsert(QString)));
            linkDialog->show();
            LinkInsertFlag = 1;
            }

        }
    }else{
        if(LinkInsertFlag){
        }else{
        linkDialog = new DelinkDialog();
        connect(linkDialog,SIGNAL(sendLinkToMain(QString)),this,SLOT(LinkInsert(QString)));
        linkDialog->show();
        LinkInsertFlag = 1;
        }
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
       bool success = query.exec("select * from varticle where emp_id='"+User_ID+"' order by article_uploadtime DESC");
       if(!success){
           qDebug() << "查询密文失败";
           return;
       }else{
           qDebug()<<"查询成功";

           while(query.next())
           {
               file_name = query.record().value("article_name").toString();
               //file_size = query.record().value("article_size").toString()+"kb";
               QString m_filesize = query.record().value("article_size").toString();
               file_status = query.record().value("article_status").toString();
               file_id = query.record().value("article_id").toString();
               QString rate = query.record().value("encrypt_num").toString();
               QString file_level;
               if(rate == "20"){
                   file_level = "加密等级：强";
               }else if(rate == "30"){
                   file_level = "加密等级：中";
               }else {
                   file_level = "加密等级：弱";
               }

               if(file_status == "1"){
                   file_discryption = "文件未上传，请上传。";
               }else if(file_status == "0"){
                   file_discryption = "文件已上传，可传输分享。";
               }

               FinishEncryptionItem *f2 = ui->MidStaWidget->findChild<FinishEncryptionItem*>(file_id);

               delete f2;

               FinishEncryptionItem *f1 = new FinishEncryptionItem();
               f1->setObjectName(file_id);
               f1->fileLevel->setText(file_level);
               QString filetype = file_name.section(".",1,1).trimmed().toStdString().c_str();
               if((filetype=="jpg")||(filetype=="png")||(filetype=="jpeg")||(filetype=="bmp")||(filetype=="gif")||(filetype=="webp")||(filetype=="psd")||(filetype=="svg")||(filetype=="tiff")){
                   QPixmap pixmap(":/new/mainwindow/pictures/pic_icon.png");
                   pixmap.scaled(f1->fileIcon->size(),Qt::KeepAspectRatio);
                   f1->fileIcon->setScaledContents(true);
                   f1->fileIcon->setPixmap(pixmap);
               }else if((filetype=="avi")||(filetype=="rmvb")||(filetype=="rm")||(filetype=="asf")||(filetype=="divx")||(filetype=="wmv")||(filetype=="mp4")||(filetype=="mkv")||(filetype=="vob")||(filetype=="mpeg")){
                   QPixmap pixmap(":/new/mainwindow/pictures/pic_icon.png");
                   pixmap.scaled(f1->fileIcon->size(),Qt::KeepAspectRatio);
                   f1->fileIcon->setScaledContents(true);
                   f1->fileIcon->setPixmap(pixmap);
               }else if((filetype=="doc")||(filetype=="docx")||(filetype=="xls")||(filetype=="xlsx")||(filetype=="ppt")||(filetype=="pptx")||(filetype=="txt")||(filetype=="docm")){
                   QPixmap pixmap(":/new/mainwindow/pictures/doc_icon.png");
                   pixmap.scaled(f1->fileIcon->size(),Qt::KeepAspectRatio);
                   f1->fileIcon->setScaledContents(true);
                   f1->fileIcon->setPixmap(pixmap);
               }else if((filetype=="rar")||(filetype=="zip")||(filetype=="arj")||(filetype=="z")){
                   QPixmap pixmap(":/new/mainwindow/pictures/zip_icon.png");
                   pixmap.scaled(f1->fileIcon->size(),Qt::KeepAspectRatio);
                   f1->fileIcon->setScaledContents(true);
                   f1->fileIcon->setPixmap(pixmap);
               }else if((filetype=="mp3")||(filetype=="wma")||(filetype=="wav")||(filetype=="ape")||(filetype=="flac")||(filetype=="ogg")||(filetype=="aac")){
                   QPixmap pixmap(":/new/mainwindow/pictures/music_icon.png");
                   pixmap.scaled(f1->fileIcon->size(),Qt::KeepAspectRatio);
                   f1->fileIcon->setScaledContents(true);
                   f1->fileIcon->setPixmap(pixmap);
               }else{
                   QPixmap pixmap(":/new/mainwindow/pictures/else_icon.png");
                   pixmap.scaled(f1->fileIcon->size(),Qt::KeepAspectRatio);
                   f1->fileIcon->setScaledContents(true);
                   f1->fileIcon->setPixmap(pixmap);
                   f1->elseLabel->setText(filetype.left(3));
                   f1->elseLabel->raise();
               }
               f1->timeLabel->setText(query.record().value("article_uploadtime").toString());

               f1->checkBox->setObjectName(file_id + "check");
               f1->pathOpenBtn->setObjectName(file_id);
               f1->transprotBtn->setObjectName(file_id);
               f1->shareBtn->setObjectName(file_id);
               f1->deleteBtn->setObjectName(file_id);

               int fontSize = fontMetrics().width( file_name );//获取之前设置的字符串的像素大小
               int pos = 0;int d_count = 0;
               pos = file_name.indexOf(".");
               while (pos>-1) {
                   d_count++;
                   pos = file_name.indexOf(".",pos+1);
               }
               QString filetype_extra = file_name.section(".",d_count-1,d_count-1).mid(file_name.section(".",d_count-1,d_count-1).length()-2)+"."+file_name.section(".",d_count,d_count).trimmed().toStdString().c_str() ;
               if( fontSize >= f1->fileName->width()-100 ) //与label自身相比较
               {
                   QString str = fontMetrics().elidedText( file_name, Qt::ElideRight, f1->fileName->width()-150 )+filetype_extra;//返回一个带有省略号的字符串
                   f1->fileName->setText( str );       //重新设置label上的字符串
               }else{
                   f1->fileName->setText(file_name);
               }

               //f1->fileName->setText(file_name);

               double filesize_double = m_filesize.toDouble();
               qDebug()<<filesize_double;
               qDebug()<<m_filesize;
               int filesize_count = 0;
               while(filesize_double>1024){
                   filesize_double = filesize_double/1024;
                   filesize_count++;
               }
               if(filesize_count == 0){
                   f1->fileSize->setText(QString::number(filesize_double, 10, 2)+"KB");
               }else if(filesize_count == 1){
                   f1->fileSize->setText(QString::number(filesize_double, 10, 2)+"MB");
               }else if(filesize_count == 2){
                   f1->fileSize->setText(QString::number(filesize_double, 10, 2)+"GB");
               }else if(filesize_count == 3){
                   f1->fileSize->setText(QString::number(filesize_double, 10, 2)+"TB");
               }
               f1->fileDescription->setText(file_discryption);
               finishViewController->vbox->setMargin(0);
               finishViewController->vbox->setSpacing(0);
               finishViewController->vbox->addWidget(f1);

               delete finishViewController->layout();
               QWidget *newItemWidget = new QWidget();
               newItemWidget->setContentsMargins(0,0,0,0);

               //QScrollArea *newScrollArea = new QScrollArea();
               newItemWidget->setLayout(finishViewController->vbox);
//               finScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
               finScrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
                                                                 "{"
                                                                 "width:8px;"
                                                                 "background:rgba(0,0,0,0%);"
                                                                 "margin:0px,0px,0px,0px;"
                                                                 "padding-top:9px;"
                                                                 "padding-bottom:9px;"
                                                                 "}"
                                                                 "QScrollBar::handle:vertical"
                                                                 "{"
                                                                 "width:8px;"
                                                                 "background:rgba(0,0,0,25%);"
                                                                 " border-radius:4px;"
                                                                 "min-height:20;"
                                                                 "}"
                                                                 "QScrollBar::handle:vertical:hover"
                                                                 "{"
                                                                 "width:8px;"
                                                                 "background:rgba(0,0,0,50%);"
                                                                 " border-radius:4px;"
                                                                 "min-height:20;"
                                                                 "}"
                                                                 "QScrollBar::add-line:vertical"
                                                                 "{"
                                                                 "height:5px;width:10px;"
                                                                 "border-image:url(:/new/mainwindow/pictures/Scrollbar_bottom.png);"
                                                                 "subcontrol-position:bottom;"
                                                                 "}"
                                                                 "QScrollBar::sub-line:vertical"
                                                                 "{"
                                                                 "height:5px;width:10px;"
                                                                 "border-image:url(:/new/mainwindow/pictures/Scrollbar_top.png);"
                                                                 "subcontrol-position:top;"
                                                                 "}"
                                                                 "QScrollBar::add-line:vertical:hover"
                                                                 "{"
                                                                 "height:9px;width:8px;"
                                                                 "border-image:url(:/images/a/4.png);"
                                                                 "subcontrol-position:bottom;"
                                                                 "}"
                                                                 "QScrollBar::sub-line:vertical:hover"
                                                                 "{"
                                                                 "height:9px;width:8px;"
                                                                 "border-image:url(:/images/a/2.png);"
                                                                 "subcontrol-position:top;"
                                                                 "}"
                                                                 "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
                                                                 "{"
                                                                 "background:rgba(0,0,0,5%);"
                                                                 "border-radius:4px;"
                                                                 "}");
               finScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
               finScrollArea->setStyleSheet("border:0;padding:0;spacing:0;margins:0;");
               finScrollArea->setWidget(newItemWidget);

               QVBoxLayout *newVbox = new QVBoxLayout();
               newVbox->setMargin(0);
               newVbox->setSpacing(0);
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
    QSqlQuery query(db);
    int flag = 0;
    bool success = query.exec("select * from varticle where emp_id='"+User_ID+"'");
    if(!success){
        qDebug() << "查询密文失败";
        return;
    }else{
        qDebug()<<"查询成功";
        while(query.next())
        {
            file_id_d = query.record().value("article_id").toString();
            QCheckBox *checkcheck = ui->MidStaWidget->findChild<QCheckBox*>(file_id_d+"check");
            if(checkcheck->isChecked()){
                flag = 1;
            }
        }
     }
    if(flag == 1){
        MsgBox *msgbox = new MsgBox(5,QStringLiteral("确认删除这些已加密条目吗？"),this);
        int nRes = msgbox->exec();
        if (nRes == QDialog::Accepted){
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
            MsgBox *msgbox = new MsgBox(4,QStringLiteral("成功批量删除条目！"),this);
            msgbox->exec();
        }else if(nRes == 5){
            QString file_id;
            QSqlQuery query3(db);
            bool success = query3.exec("select * from varticle where emp_id='"+User_ID+"'");
            if(!success){
                qDebug() << "查询密文失败";
                return;
            }else{
                qDebug()<<"查询成功";
                while(query3.next())
                {
                    file_id = query3.record().value("article_id").toString();
                    QString d_address ="C:/CloundSafe/"+User_qqNum+"/encrypt/yZip/" + query3.record().value("article_name").toString();
                    QCheckBox *checkcheck = ui->MidStaWidget->findChild<QCheckBox*>(file_id+"check");

                    if(checkcheck->isChecked()){
                        QSqlQuery query9(db);
                        query9.exec("delete from varticle where article_id = '"+file_id+"'");
                        QFileInfo d_file(d_address);
                        if(d_file.isFile()){
                            QFile::remove(d_address);
                        }
                        RequestNum--;
                    }
                    FinishEncryptionItem *f2 = ui->MidStaWidget->findChild<FinishEncryptionItem*>(file_id);
                    delete f2;
                 }
             }
             on_pushButton_8_clicked();
             MsgBox *msgbox = new MsgBox(4,QStringLiteral("成功批量删除条目及文件！"),this);
             msgbox->exec();
        }else{
        }
    }else if(flag == 0){
        MsgBox *msgbox = new MsgBox(3,QStringLiteral("请选择需要删除的条目"),this);
        msgbox->exec();
    }
    flag = 0;


}
//批量传输按钮响应
void MainWindow::on_pushButton_6_clicked()
{
    if(groupSendDialogFlag){

    }else{
        int flag = 0;
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
                    flag = 1;
                    file_id_list = file_id_list + "||" +file_id_d;
                    }
                }
            }
        if(flag == 1){

            grpDlg = new groupSendDialog();
            grpDlg->show();
            groupSendDialogFlag = true;

        }else if(flag == 0){
            MsgBox *msgbox = new MsgBox(3,QStringLiteral("请选择需要批量传输的条目！"),this);
            msgbox->exec();
        }
        flag = 0;
    }
}

//已加密文件单独删除按钮
void MainWindow::on_deleteBtn_clicked(){
    MsgBox *msgbox = new MsgBox(5,QStringLiteral("确认删除这一已加密文件吗？"),this);
    int nRes = msgbox->exec();
    if (nRes == QDialog::Accepted){
        QPushButton* button = qobject_cast<QPushButton*>(sender());
        QString name = button->objectName();

        FinishEncryptionItem *f2 = ui->MidStaWidget->findChild<FinishEncryptionItem*>(name);
        delete f2;
        qDebug()<<name;

        QSqlQuery query(db);
        bool success = query.exec("delete from varticle where article_id = '"+name+"'");
        if(success){
            MsgBox *msgbox = new MsgBox(4,QStringLiteral("成功删除已加密条目！"),this);
            msgbox->exec();
        }

        delete finishViewController->layout();
        finishViewController->vbox->setMargin(0);
        finishViewController->vbox->setSpacing(0);
        QWidget *newItemWidget = new QWidget();
        newItemWidget->setContentsMargins(0,0,0,0);
        newItemWidget->setLayout(finishViewController->vbox);
        finScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
        finScrollArea->setWidget(newItemWidget);
        QVBoxLayout *newVbox = new QVBoxLayout();
        newVbox->setMargin(0);
        newVbox->setSpacing(0);
        newVbox->addWidget(finScrollArea);
        finishViewController->setLayout(newVbox);
        }
    else if (nRes == QDialog::Rejected){
    }else if(nRes == 5){
        QPushButton* button = qobject_cast<QPushButton*>(sender());
        QString name = button->objectName();

        FinishEncryptionItem *f2 = ui->MidStaWidget->findChild<FinishEncryptionItem*>(name);
        delete f2;
        qDebug()<<name;

        QSqlQuery query(db);
        bool success1 = query.exec("select * from varticle where article_id = '"+name+"'");
        if(success1){
            while(query.next()){
                QString address = "C:/CloundSafe/"+User_qqNum+"/encrypt/yZip/" +query.record().value("article_name").toString();
                qDebug()<<address;
                QFileInfo d_file(address);
                if(d_file.isFile()){
                QFile::remove(address);
                }
            }
        }
        bool success = query.exec("delete from varticle where article_id = '"+name+"'");
        if(success){
            MsgBox *msgbox = new MsgBox(4,QStringLiteral("成功删除已加密条目及文件！"),this);
            msgbox->exec();
        }

        delete finishViewController->layout();
        finishViewController->vbox->setMargin(0);
        finishViewController->vbox->setSpacing(0);
        QWidget *newItemWidget = new QWidget();
        newItemWidget->setContentsMargins(0,0,0,0);
        newItemWidget->setLayout(finishViewController->vbox);
        finScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
        finScrollArea->setWidget(newItemWidget);
        QVBoxLayout *newVbox = new QVBoxLayout();
        newVbox->setMargin(0);
        newVbox->setSpacing(0);
        newVbox->addWidget(finScrollArea);
        finishViewController->setLayout(newVbox);
    }
}
//已解密文件单独删除按钮
void MainWindow::on_deleteBtn2_clicked(){
    MsgBox *msgbox = new MsgBox(5,QStringLiteral("确认删除这一已解密文件吗？"),this);
    int nRes = msgbox->exec();
    if (nRes == QDialog::Accepted){
            QPushButton* button = qobject_cast<QPushButton*>(sender());
            QString name = button->objectName();
            for(int i = 0;i<5;i++){
                FinishDecryptionItem *f2 = ui->MidStaWidget->findChild<FinishDecryptionItem*>(name);
                delete f2;
            }
            qDebug()<<name;

            QSqlQuery query(db);
            bool success = query.exec("delete from Decryption where file_id = '"+name+"'");
            if(success){
                MsgBox *msgbox = new MsgBox(4,QStringLiteral("成功删除已解密条目！"),this);
                msgbox->exec();
            }
            delete finishViewController2->layout();
            finishViewController2->vbox->setMargin(0);
            finishViewController2->vbox->setSpacing(0);
            QWidget *newItemWidget = new QWidget();
            newItemWidget->setContentsMargins(0,0,0,0);
           // QScrollArea *newScrollArea = new QScrollArea();
            newItemWidget->setLayout(finishViewController2->vbox);
            finScrollArea->setWidget(newItemWidget);
            finScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
            QVBoxLayout *newVbox = new QVBoxLayout();
            newVbox->setMargin(0);
            newVbox->setSpacing(0);
            newVbox->addWidget(finScrollArea);
            finishViewController2->setLayout(newVbox);
        }
        else if (nRes == QDialog::Rejected){
    }else if(nRes == 5){
        QPushButton* button = qobject_cast<QPushButton*>(sender());
        QString name = button->objectName();
            for(int i = 0;i<5;i++){
                FinishDecryptionItem *f2 = ui->MidStaWidget->findChild<FinishDecryptionItem*>(name);
                delete f2;
            }
        qDebug()<<name;

        QSqlQuery query(db);
        bool success1 = query.exec("select * from Decryption where file_id = '"+name+"'");
        if(success1){
            while(query.next()){
                QString address = "C:/CloundSafe/"+User_qqNum+"/Decrypt/file/" +query.record().value("file_name").toString();
                qDebug()<<address;
                QFileInfo d_file(address);
                if(d_file.isFile()){
                QFile::remove(address);
                }
            }
        }
        bool success = query.exec("delete from Decryption where file_id = '"+name+"'");
        if(success){
            MsgBox *msgbox = new MsgBox(4,QStringLiteral("成功删除已解密条目及文件！"),this);
            msgbox->exec();
        }

        delete finishViewController2->layout();
        finishViewController2->vbox->setMargin(0);
        finishViewController2->vbox->setSpacing(0);
        QWidget *newItemWidget = new QWidget();
        newItemWidget->setContentsMargins(0,0,0,0);
       // QScrollArea *newScrollArea = new QScrollArea();
        newItemWidget->setLayout(finishViewController2->vbox);
        finScrollArea->setWidget(newItemWidget);
        finScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
        QVBoxLayout *newVbox = new QVBoxLayout();
        newVbox->setMargin(0);
        newVbox->setSpacing(0);
        newVbox->addWidget(finScrollArea);
        finishViewController2->setLayout(newVbox);
    }
}
//已解密文件刷新按钮
void MainWindow::on_pushButton_9_clicked()
{

    QString file_id;
    QString file_name;
    QString file_size;
    QString file_discryption;
    //finishViewController2->vbox = new QVBoxLayout();
    QSqlQuery query(db);
       bool success = query.exec("select * from Decryption where (status = 5 or status = 15) and oemp_id ='" + User_ID+"' order by apply_time DESC");
       if(!success){
           qDebug() << "查询密文失败";
           return;
       }else{
           qDebug()<<"查询成功";
           finishViewController2->vbox = new QVBoxLayout();//将原有视图清空
           while(query.next())
           {
               file_name = query.record().value("file_name").toString();
               //file_size = query.record().value("file_size").toString()+"kb";
                QString m_filesize = query.record().value("file_size").toString();
                QString rate = query.record().value("file_encrypt_num").toString();
                QString file_level;
                if(rate == "20"){
                    file_level = "加密等级：强";
                }else if(rate == "30"){
                    file_level = "加密等级：中";
                }else {
                    file_level = "加密等级：弱";
                }

                QString the_id = query.record().value("id").toString();
               file_id = query.record().value("file_id").toString();

               file_discryption = "文件已成功解密。";

//               FinishDecryptionItem *f2 = ui->MidStaWidget->findChild<FinishDecryptionItem*>(file_id);
//               delete f2;

               FinishDecryptionItem *f1 = new FinishDecryptionItem();

                f1->fileLevel->setText(file_level);

               f1->setObjectName(file_id);

               QString filetype = file_name.section(".",1,1).trimmed().toStdString().c_str();
               if((filetype=="jpg")||(filetype=="png")||(filetype=="jpeg")||(filetype=="bmp")||(filetype=="gif")||(filetype=="webp")||(filetype=="psd")||(filetype=="svg")||(filetype=="tiff")){
                   QPixmap pixmap(":/new/mainwindow/pictures/pic_icon.png");
                   pixmap.scaled(f1->fileIcon->size(),Qt::KeepAspectRatio);
                   f1->fileIcon->setScaledContents(true);
                   f1->fileIcon->setPixmap(pixmap);
               }else if((filetype=="avi")||(filetype=="rmvb")||(filetype=="rm")||(filetype=="asf")||(filetype=="divx")||(filetype=="wmv")||(filetype=="mp4")||(filetype=="mkv")||(filetype=="vob")||(filetype=="mpeg")){
                   QPixmap pixmap(":/new/mainwindow/pictures/video_icon.png");
                   pixmap.scaled(f1->fileIcon->size(),Qt::KeepAspectRatio);
                   f1->fileIcon->setScaledContents(true);
                   f1->fileIcon->setPixmap(pixmap);
               }else if((filetype=="doc")||(filetype=="docx")||(filetype=="xls")||(filetype=="xlsx")||(filetype=="ppt")||(filetype=="pptx")||(filetype=="txt")||(filetype=="docm")){
                   QPixmap pixmap(":/new/mainwindow/pictures/doc_icon.png");
                   pixmap.scaled(f1->fileIcon->size(),Qt::KeepAspectRatio);
                   f1->fileIcon->setScaledContents(true);
                   f1->fileIcon->setPixmap(pixmap);
               }else if((filetype=="rar")||(filetype=="zip")||(filetype=="arj")||(filetype=="z")){
                   QPixmap pixmap(":/new/mainwindow/pictures/zip_icon.png");
                   pixmap.scaled(f1->fileIcon->size(),Qt::KeepAspectRatio);
                   f1->fileIcon->setScaledContents(true);
                   f1->fileIcon->setPixmap(pixmap);
               }else if((filetype=="mp3")||(filetype=="wma")||(filetype=="wav")||(filetype=="ape")||(filetype=="flac")||(filetype=="ogg")||(filetype=="aac")){
                   QPixmap pixmap(":/new/mainwindow/pictures/music_icon.png");
                   pixmap.scaled(f1->fileIcon->size(),Qt::KeepAspectRatio);
                   f1->fileIcon->setScaledContents(true);
                   f1->fileIcon->setPixmap(pixmap);
               }else{
                   QPixmap pixmap(":/new/mainwindow/pictures/else_icon.png");
                   pixmap.scaled(f1->fileIcon->size(),Qt::KeepAspectRatio);
                   f1->fileIcon->setScaledContents(true);
                   f1->fileIcon->setPixmap(pixmap);
                   f1->elseLabel->setText(filetype.left(3));
                   f1->elseLabel->raise();
               }
               f1->timeLabel->setText(query.record().value("apply_time").toString());
               f1->checkBox->setObjectName(file_id + "check");
               f1->pathOpenBtn->setObjectName(file_id);
               f1->openBtn->setObjectName(file_id);
               f1->deleteBtn->setObjectName(file_id);

               int fontSize = fontMetrics().width( file_name );//获取之前设置的字符串的像素大小
               int pos = 0;int d_count = 0;
               pos = file_name.indexOf(".");
               while (pos>-1) {
                   d_count++;
                   pos = file_name.indexOf(".",pos+1);
               }
               QString filetype_extra = file_name.section(".",d_count-1,d_count-1).mid(file_name.section(".",d_count-1,d_count-1).length()-2)+"."+file_name.section(".",d_count,d_count).trimmed().toStdString().c_str() ;
               if( fontSize >= f1->fileName->width()-100 ) //与label自身相比较
               {
                   QString str = fontMetrics().elidedText( file_name, Qt::ElideRight, f1->fileName->width()-150 )+filetype_extra;//返回一个带有省略号的字符串
                   f1->fileName->setText( str );       //重新设置label上的字符串
               }else{
                   f1->fileName->setText(file_name);
               }

               //f1->fileName->setText(file_name);
               double filesize_double = m_filesize.toDouble();
               int filesize_count = 0;
               while(filesize_double>1024){
                   filesize_double = filesize_double/1024;
                   filesize_count++;
               }
               if(filesize_count == 0){
                   f1->fileSize->setText(QString::number(filesize_double, 10, 2)+"KB");
               }else if(filesize_count == 1){
                   f1->fileSize->setText(QString::number(filesize_double, 10, 2)+"MB");
               }else if(filesize_count == 2){
                   f1->fileSize->setText(QString::number(filesize_double, 10, 2)+"GB");
               }else if(filesize_count == 3){
                   f1->fileSize->setText(QString::number(filesize_double, 10, 2)+"TB");
               }
               f1->fileDescription->setText(file_discryption);
                if(query.record().value("status").toString() == "15"){
                    f1->fileSize->setText("文件来自安卓系统");
                }
               finishViewController2->vbox->addWidget(f1);
               finishViewController2->vbox->setMargin(0);
               finishViewController2->vbox->setSpacing(0);
               delete finishViewController2->layout();
               QWidget *newItemWidget = new QWidget();
               newItemWidget->setContentsMargins(0,0,0,0);
              // QScrollArea *newScrollArea = new QScrollArea();
               newItemWidget->setLayout(finishViewController2->vbox);
               finScrollArea->setWidget(newItemWidget);
               finScrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
                                                                 "{"
                                                                 "width:8px;"
                                                                 "background:rgba(0,0,0,0%);"
                                                                 "margin:0px,0px,0px,0px;"
                                                                 "padding-top:9px;"
                                                                 "padding-bottom:9px;"
                                                                 "}"
                                                                 "QScrollBar::handle:vertical"
                                                                 "{"
                                                                 "width:8px;"
                                                                 "background:rgba(0,0,0,25%);"
                                                                 " border-radius:4px;"
                                                                 "min-height:20;"
                                                                 "}"
                                                                 "QScrollBar::handle:vertical:hover"
                                                                 "{"
                                                                 "width:8px;"
                                                                 "background:rgba(0,0,0,50%);"
                                                                 " border-radius:4px;"
                                                                 "min-height:20;"
                                                                 "}"
                                                                 "QScrollBar::add-line:vertical"
                                                                 "{"
                                                                 "height:5px;width:10px;"
                                                                 "border-image:url(:/new/mainwindow/pictures/Scrollbar_bottom.png);"
                                                                 "subcontrol-position:bottom;"
                                                                 "}"
                                                                 "QScrollBar::sub-line:vertical"
                                                                 "{"
                                                                 "height:5px;width:10px;"
                                                                 "border-image:url(:/new/mainwindow/pictures/Scrollbar_top.png);"
                                                                 "subcontrol-position:top;"
                                                                 "}"
                                                                 "QScrollBar::add-line:vertical:hover"
                                                                 "{"
                                                                 "height:9px;width:8px;"
                                                                 "border-image:url(:/images/a/4.png);"
                                                                 "subcontrol-position:bottom;"
                                                                 "}"
                                                                 "QScrollBar::sub-line:vertical:hover"
                                                                 "{"
                                                                 "height:9px;width:8px;"
                                                                 "border-image:url(:/images/a/2.png);"
                                                                 "subcontrol-position:top;"
                                                                 "}"
                                                                 "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
                                                                 "{"
                                                                 "background:rgba(0,0,0,5%);"
                                                                 "border-radius:4px;"
                                                                 "}");
               finScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
               finScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
               QVBoxLayout *newVbox = new QVBoxLayout();
               newVbox->setMargin(0);
               newVbox->setSpacing(0);
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
    //创建CloundSafe 主目录
    QDir dir;
    dir.cd("C://CloundSafe");  //进入某文件夹
    if(!dir.exists("C://CloundSafe"))//判断需要创建的文件夹是否存在
    {
        dir.mkdir("C://CloundSafe"); //创建文件夹
    }
    //创建用户子目录
    QString qq_Path = "C://CloundSafe//"+User_qqNum;
    dir.cd(qq_Path);  //进入某文件夹
    if(!dir.exists(qq_Path))//判断需要创建的文件夹是否存在
    {
        dir.mkdir(qq_Path); //创建文件夹
    }
    //创建Decrypt子目录
    QString file_path = qq_Path+"//Decrypt";
    dir.cd(file_path);
    if(!dir.exists(file_path)){
        dir.mkdir(file_path);
    }
    QString path_finished = file_path+"//file";
    //创建子目录
    dir.cd(path_finished);  //进入某文件夹
    if(!dir.exists(path_finished))//判断需要创建的文件夹是否存在
    {
        dir.mkdir(path_finished); //创建文件夹
    }
    QString path_ykey = file_path+"//ykey";
    dir.cd(path_ykey);
    if(!dir.exists(path_ykey))
    {
        dir.mkdir(path_ykey);
    }



     QSqlQuery query(db);
     bool success = query.exec("select * from Decryption where oemp_id = '"+User_ID+"' and status =3");
     if(success){
         while(query.next()){
             QString onlyID = query.record().value("id").toString();
             QString file_decrypt_num = query.record().value("file_encrypt_num").toString();
             QString file_id = query.record().value("file_id").toString();
             DecryptionItem *f1 = ui->MidStaWidget->findChild<DecryptionItem *>(onlyID+"decryption");
             if(f1==nullptr){
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
                 connect(this,SIGNAL(sendFileID(QString,QString,QString,QString)),depThread[DepThreadNum],SLOT(DecryptionThread_RecvID(QString,QString,QString,QString)));
                 connect(depThread[DepThreadNum],SIGNAL(decryptionFailed()),this,SLOT(RecDecryptionFailed()));
                 connect(depThread[DepThreadNum],SIGNAL(sendDecTime(QString,double)),this,SLOT(setDecItemDectime(QString,double)));

                 qDebug()<<"mainwindow:"+id;
//                 emit sendFileID(enkey_id,file_id,fileName,id);
                 //给DecryptionThread中enkey_id,file_id,fileName，id赋值
                 depThread[DepThreadNum]->Denkey_id=enkey_id;
                 depThread[DepThreadNum]->Dfile_id=file_id;
                 depThread[DepThreadNum]->Dfile_name=fileName;
                 depThread[DepThreadNum]->DItem_id=id;
                 depThread[DepThreadNum]->Decrypt_num = file_decrypt_num;
                 depThread[DepThreadNum]->start();
                 DepThreadNum++;
//                 if(decryptionFlag == 0){
//                     //解密完成后将数据库该条数据状态status改成5
//                     QSqlQuery updateQuery(db);
//                     bool updataSuccess = updateQuery.exec("update Decryption set status = 5 where id ='"+id+"'");
//                     if(updataSuccess){
//                         RequsetAllowNum--;
//                         qDebug()<<"status=5 update success!";
//                     }
//                     else{
//                         qDebug()<<"status=5 update failed";
//                     }
//                     //解密完成后删除本地密钥和密文
//                     QString dec_time = CreateTimeTitle(f1->dec_time);
//                     QString dec_title = "文件解密同意并成功解密，解密时间为：";
//                     dec_title.append(dec_time);
//                     //解密完成后删除控件
//                     delete f1;
//                     qDebug()<<"删除后的打印";
//                     //重新布局
//                     delete decryptionViewController->layout();
//                     decryptionViewController->vbox->setMargin(0);
//                     decryptionViewController->vbox->setSpacing(0);
//                     QWidget *newItemWidget = new QWidget();
//                     newItemWidget->setContentsMargins(0,0,0,0);
//                     newItemWidget->setLayout(decryptionViewController->vbox);
//                     newScrollArea->setWidget(newItemWidget);
//                     newScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
////                     newScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//                     QVBoxLayout *newVbox = new QVBoxLayout();
//                     newVbox->setMargin(0);
//                     newVbox->setSpacing(0);
//                     newVbox->addWidget(newScrollArea);
//                     decryptionViewController->setLayout(newVbox);
//                     MsgBox *msgbox = new MsgBox(4,dec_title,this);
//                     msgbox->exec();
//                 }
//                 else{
//                     return;
//                 }
             }
         }
     }
     else{
         qDebug()<<"查询失败";
     }
}
void MainWindow::ChangeItemBtnText(QString fileID){
    DecryptionItem *m1 = ui->MidStaWidget->findChild<DecryptionItem*>(fileID+"decryption");
    m1->fileDescription->setText("文件已加密需下载密钥文件");
    m1->downloadBtn->setText("申请解密");
    m1->label->show();
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
//设置主界面用户名和用户个人信息
void MainWindow::setEmp_name(){
    qDebug()<<"函数执行";

    QSqlQuery query(db);
    bool success = query.exec("select * from employee where emp_id = '"+User_ID+"'");
    if(success){
        while (query.next()) {
            QString nickName = query.record().value("emp_name").toString();
            qDebug()<<nickName;
            ui->nameLabel->setFont(f_h);
            ui->nameLabel->setStyleSheet("color:#3D3939");
            ui->nameLabel->setText(nickName);

        }
        ui->UserPhonelabe->setText("手机号："+UserPhoneNum);
        ui->UserPhonelabe->hide();
        ui->UserIDlabel->hide();
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
    int flag = 0;
    QString file_id;
    QSqlQuery query3(db);
    bool success = query3.exec("select * from Decryption where (status = 5 or status = 15)and oemp_id ='" + User_ID+"'");
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
                flag = 1;
            }
        }
     }
    if(flag == 1){
        MsgBox *msgbox = new MsgBox(5,QStringLiteral("确认删除这些已解密条目吗？"),this);
        int nRes = msgbox->exec();
        if (nRes == QDialog::Accepted){
            QSqlQuery query3(db);
            bool success = query3.exec("select * from Decryption where (status = 5 or status = 15) and oemp_id ='" + User_ID+"'");
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
            MsgBox *msgbox = new MsgBox(4,QStringLiteral("成功批量删除条目！"),this);
            msgbox->exec();
        }else if(nRes == 5){
            QString file_id;
            QSqlQuery query3(db);
            bool success = query3.exec("select * from Decryption where (status = 5 or status = 15) and oemp_id ='" + User_ID+"'");
            if(!success){
                qDebug() << "查询密文失败";
                return;
            }else{
                qDebug()<<"查询成功";
                while(query3.next())
                {
                    file_id = query3.record().value("file_id").toString();
                    QString d_address ="C:/CloundSafe/"+User_qqNum+"/Decrypt/file/" + query3.record().value("file_name").toString();
                    QCheckBox *checkcheck = ui->MidStaWidget->findChild<QCheckBox*>(file_id+"check");

                    if(checkcheck->isChecked()){
                        QSqlQuery query9(db);
                        query9.exec("delete from Decryption where file_id = '"+file_id+"'");
                        QFileInfo d_file(d_address);
                        if(d_file.isFile()){
                            QFile::remove(d_address);
                        }
                        RequestNum--;
                    }
                    FinishDecryptionItem *f2 = ui->MidStaWidget->findChild<FinishDecryptionItem*>(file_id);
                    delete f2;
                 }
             }
             on_pushButton_9_clicked();
             MsgBox *msgbox = new MsgBox(4,QStringLiteral("成功批量删除条目及文件！"),this);
             msgbox->exec();
        }else{

        }
    }else if(flag == 0){
        MsgBox *msgbox = new MsgBox(3,QStringLiteral("请选择需要删除的条目！"),this);
        msgbox->exec();
    }
    flag = 0;
}

//已解密页面全选按钮
void MainWindow::on_pushButton_10_clicked()
{

    QSqlQuery query(db);
    bool success = query.exec("select * from Decryption where (status = 5 or status = 15) and oemp_id ='" + User_ID+"'");
    if(!success){
        qDebug() << "查询密文失败";
        return;
    }else{
        qDebug()<<"查询成功";

        while(query.next())
        {
            QString file_id = query.record().value("file_id").toString();
            QCheckBox *checkcheck = ui->MidStaWidget->findChild<QCheckBox*>(file_id+"check");
            if(finencrypt_flag == 0){
                checkcheck->setChecked(true);
            }else if(finencrypt_flag == 1){
                checkcheck->setChecked(false);
            }
        }
    }
    if(finencrypt_flag == 0){
        finencrypt_flag = 1;
        ui->pushButton_10->setStyleSheet(" QPushButton {border-image: url(:/new/mainwindow/pictures/allselect_pressed.png);} "
                                         " QPushButton:hover {border-image: url(:/new/mainwindow/pictures/allselect_hover.png);}");
    }else if(finencrypt_flag == 1){
        finencrypt_flag =0;
        ui->pushButton_10->setStyleSheet(" QPushButton {border-image: url(:/new/mainwindow/pictures/allselect.png);} "
                                         " QPushButton:hover {border-image: url(:/new/mainwindow/pictures/allselect_hover.png);}");
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
            if(findecrypt_flag == 0){
                checkcheck->setChecked(true);
            }else if(findecrypt_flag == 1){
                checkcheck->setChecked(false);
            }
        }
        if(findecrypt_flag == 0){
            findecrypt_flag = 1;
            ui->pushButton_11->setStyleSheet(" QPushButton {border-image: url(:/new/mainwindow/pictures/allselect_pressed.png);} "
                                             " QPushButton:hover {border-image: url(:/new/mainwindow/pictures/allselect_hover.png);}");
        }else if(findecrypt_flag == 1){
            findecrypt_flag =0;
            ui->pushButton_11->setStyleSheet(" QPushButton {border-image: url(:/new/mainwindow/pictures/allselect.png);} "
                                             " QPushButton:hover {border-image: url(:/new/mainwindow/pictures/allselect_hover.png);}");
        }

    }
}

void MainWindow::CleanButtonClicked(){
    ui->FinDepBtn->setFont(m_w4);
    ui->DecryptionBtn->setFont(m_w4);
    ui->EncryptionBtn->setFont(m_w4);
    ui->FinishedBtn->setFont(m_w4);
    ui->FinEnpBtn->setFont(m_w4);
    ui->FinDepBtn->setStyleSheet("border-image: url(:/new/mainwindow/pictures/mainwindow_button_bg.png);");
    ui->DecryptionBtn->setStyleSheet("border-image: url(:/new/mainwindow/pictures/mainwindow_button_bg.png);");
    ui->DecryptionBtn->setIcon(QIcon(":/new/mainwindow/pictures/decryption_icon.png"));
    ui->EncryptionBtn->setStyleSheet("border-image: url(:/new/mainwindow/pictures/mainwindow_button_bg.png);");
    ui->EncryptionBtn->setIcon(QIcon(":/new/mainwindow/pictures/encryption_icon.png"));
    ui->FinEnpBtn->setStyleSheet("border-image: url(:/new/mainwindow/pictures/mainwindow_button_bg.png);");
    ui->FinishedBtn->setStyleSheet("border-image: url(:/new/mainwindow/pictures/mainwindow_button_bg.png);");
    ui->FinishedBtn->setIcon(QIcon(":/new/mainwindow/pictures/fin_icon.png"));
    //ui->blue_encrypt->setStyleSheet("background-color: #3A8CFF");
    ui->blue_encrypt->setStyleSheet("background-color: white");
    ui->blue_encrypt_2->setStyleSheet("background-color: white");
    ui->blue_encrypt_3->setStyleSheet("background-color: white");
    ui->blue_encrypt_4->setStyleSheet("background-color: white");
    ui->blue_encrypt_5->setStyleSheet("background-color: white");
}

void MainWindow::LinkInsert(QString link){

    QString GetLink = link;
    QSqlQuery query(db);
    QString Link_empid;
    QString Link_filename;
    QString Link_filesize;
    QString Link_extraRate;
    QDateTime time = QDateTime::currentDateTime();
    QString time_str = time.toString("yyyy-MM-dd hh:mm:ss");
    QString file_level;
    bool success = query.exec("select * from varticle where article_id = '"+GetLink+"'");
    if(!success){
        qDebug()<<"linkSelectFailed";
    }
    else{
        while(query.next()){
            Link_empid = query.record().value("emp_id").toString();
            Link_filename = query.record().value("article_name").toString();
            Link_filesize = query.record().value("article_size").toString();
            Link_extraRate = query.record().value("encrypt_num").toString();
            QString rate = query.record().value("file_encrypt_num").toString();

            if(rate == "20"){
                file_level = "加密等级：强";
            }else if(rate == "30"){
                file_level = "加密等级：中";
            }else {
                file_level = "加密等级：弱";
            }

        }
    }
    if(Link_empid == nullptr){
        MsgBox *msgbox = new MsgBox(6,QStringLiteral("无效链接！"),this);
        msgbox->exec();
        return;
    }
    QUuid strid = QUuid::createUuid();
    QString id = strid.toString();
    QSqlQuery insertQuery(db);
    bool insertSuccess = insertQuery.exec("insert into Decryption values('"+id+"','"+GetLink+"','"+Link_filename+"','"+Link_empid+"','','"+User_ID+"','',0,'"+time_str+"','"+Link_filesize+"',0,0,"+Link_extraRate+")");
    if(!insertSuccess){
        qDebug()<<"LinkInsertFailed";
    }
    else{
        DecryptionItem *a1 = new DecryptionItem();
        a1->fileLevel->setText(file_level);
        QString filetype = Link_filename.section(".",1,1).trimmed().toStdString().c_str();
        if((filetype=="jpg")||(filetype=="png")||(filetype=="jpeg")||(filetype=="bmp")||(filetype=="gif")||(filetype=="webp")||(filetype=="psd")||(filetype=="svg")||(filetype=="tiff")){
            QPixmap pixmap(":/new/mainwindow/pictures/pic_icon.png");
            pixmap.scaled(a1->fileIcon->size(),Qt::KeepAspectRatio);
            a1->fileIcon->setScaledContents(true);
            a1->fileIcon->setPixmap(pixmap);
        }else if((filetype=="avi")||(filetype=="rmvb")||(filetype=="rm")||(filetype=="asf")||(filetype=="divx")||(filetype=="wmv")||(filetype=="mp4")||(filetype=="mkv")||(filetype=="vob")||(filetype=="mpeg")){
            QPixmap pixmap(":/new/mainwindow/pictures/video_icon.png");
            pixmap.scaled(a1->fileIcon->size(),Qt::KeepAspectRatio);
            a1->fileIcon->setScaledContents(true);
            a1->fileIcon->setPixmap(pixmap);
        }else if((filetype=="doc")||(filetype=="docx")||(filetype=="xls")||(filetype=="xlsx")||(filetype=="ppt")||(filetype=="pptx")||(filetype=="txt")||(filetype=="docm")){
            QPixmap pixmap(":/new/mainwindow/pictures/doc_icon.png");
            pixmap.scaled(a1->fileIcon->size(),Qt::KeepAspectRatio);
            a1->fileIcon->setScaledContents(true);
            a1->fileIcon->setPixmap(pixmap);
        }else if((filetype=="rar")||(filetype=="zip")||(filetype=="arj")||(filetype=="z")){
            QPixmap pixmap(":/new/mainwindow/pictures/zip_icon.png");
            pixmap.scaled(a1->fileIcon->size(),Qt::KeepAspectRatio);
            a1->fileIcon->setScaledContents(true);
            a1->fileIcon->setPixmap(pixmap);
        }else if((filetype=="mp3")||(filetype=="wma")||(filetype=="wav")||(filetype=="ape")||(filetype=="flac")||(filetype=="ogg")||(filetype=="aac")){
            QPixmap pixmap(":/new/mainwindow/pictures/music_icon.png");
            pixmap.scaled(a1->fileIcon->size(),Qt::KeepAspectRatio);
            a1->fileIcon->setScaledContents(true);
            a1->fileIcon->setPixmap(pixmap);
        }else{
            QPixmap pixmap(":/new/mainwindow/pictures/else_icon.png");
            pixmap.scaled(a1->fileIcon->size(),Qt::KeepAspectRatio);
            a1->fileIcon->setScaledContents(true);
            a1->fileIcon->setPixmap(pixmap);
            a1->elseLabel->setText(filetype.left(3));
            a1->elseLabel->raise();
        }
        int fontSize = fontMetrics().width( Link_filename );//获取之前设置的字符串的像素大小
        int pos = 0;int d_count = 0;
        pos = Link_filename.indexOf(".");
        while (pos>-1) {
            d_count++;
            pos = Link_filename.indexOf(".",pos+1);
        }
        QString filetype_extra = Link_filename.section(".",d_count-1,d_count-1).mid(Link_filename.section(".",d_count-1,d_count-1).length()-2)+"."+Link_filename.section(".",d_count,d_count).trimmed().toStdString().c_str() ;
        if( fontSize >= a1->fileName->width()-100 ) //与label自身相比较
        {
            QString str = fontMetrics().elidedText( Link_filename, Qt::ElideRight, a1->fileName->width()-150 )+filetype_extra;//返回一个带有省略号的字符串
            a1->fileName->setText( str );       //重新设置label上的字符串
        }else{
            a1->fileName->setText(Link_filename);
        }
        //a1->fileSize->setText(Link_filesize+"KB");
        double filesize_double = Link_filesize.toDouble();
        int filesize_count = 0;
        while(filesize_double>1024){
            filesize_double = filesize_double/1024;
            filesize_count++;
        }
        if(filesize_count == 0){
            a1->fileSize->setText(QString::number(filesize_double, 10, 2)+"KB");
        }else if(filesize_count == 1){
            a1->fileSize->setText(QString::number(filesize_double, 10, 2)+"MB");
        }else if(filesize_count == 2){
            a1->fileSize->setText(QString::number(filesize_double, 10, 2)+"GB");
        }else if(filesize_count == 3){
            a1->fileSize->setText(QString::number(filesize_double, 10, 2)+"TB");
        }
        a1->checkBox->setObjectName(id+"Decheck");
        a1->setObjectName(id+"decryption");
        a1->downloadBtn->setObjectName(id+"btn");
        a1->downloadBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: #3A8CFF;color:white;}QPushButton:pressed{background-color: rgb(139,159,185);}");
//        connect(ui->pushButton,SIGNAL(clicked(bool)),a1,SLOT(changeCheckBox()));
        a1->fileDescription->setText("主体文件指定分享需确认下载.");
        a1->downloadBtn->setText("确认下载");
        connect(a1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(OssDownLoadFile()));
        decryptionViewController->vbox->addWidget(a1);
        f_progressBar = new QProgressBar(this);
        f_progressBar->raise();
//        a1->fileIcon->stackUnder(f_progressBar);
//        a1->timeLabel->stackUnder(f_progressBar);
        f_progressBar = a1->progressBar;
        f_progressBar->setObjectName(a1->objectName());
//        f_progressBar->setMinimum(0);
//        f_progressBar->setMaximum(100);
//        f_progressBar->setOrientation(Qt::Horizontal);
        f_progressBar->hide();
        f_progressBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);  // 对齐方式
        //decryptionViewController->vbox->addWidget(f_progressBar);
        IsLinkInfor = 1;
        ReLayout();
    }

}
//信号槽：接收到解密线程发送的解密失败信号
void MainWindow::RecDecryptionFailed(){
    MsgBox *msgbox = new MsgBox(2,QStringLiteral("文件解密失败"),this);
    msgbox->exec();
    decryptionFlag=1;
}

void MainWindow::ReLayout(){
    delete decryptionViewController->layout();
    decryptionViewController->vbox->setMargin(0);
    decryptionViewController->vbox->setSpacing(0);
    QWidget *newItemWidget = new QWidget();
    newItemWidget->setContentsMargins(0,0,0,0);
    newItemWidget->setLayout(decryptionViewController->vbox);
    newScrollArea->setWidget(newItemWidget);
    newScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
//    newScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QVBoxLayout *newVbox = new QVBoxLayout();
    newVbox->setMargin(0);
    newVbox->setSpacing(0);
    newVbox->addWidget(newScrollArea);
    decryptionViewController->setLayout(newVbox);
}

void MainWindow::DeProgressBarStart(){
    //d_progressBar->setValue(proccessValue);
}




void MainWindow::on_pushButton_12_clicked()
{
    QString m_fileName = QFileDialog::getOpenFileName(this,"二维码打开文件","./","任何文件(*.*)"";;文本文件(*.png)");

    if(m_fileName.isEmpty() == false)
    {
        QImage img;        //加载图像
        if(!(img.load(m_fileName)))
        {
            MsgBox *msgbox = new MsgBox(2,QStringLiteral("打开图像失败！"),this);
            msgbox->exec();
            return;
        }

        QZXing *decoder = new QZXing(QZXing::DecoderFormat_QR_CODE);
        QString strQRCode = decoder->decodeImage(img);
        QString originalText = strQRCode.section("&&",1,1);
        if(originalText.startsWith("{",Qt::CaseSensitive)){
            LinkInsert(originalText);
            MsgBox *msgbox = new MsgBox(4,QStringLiteral("已读取二维码信息！"),this);
            msgbox->exec();
        }else{
            MsgBox *msgbox = new MsgBox(2,QStringLiteral("二维码读取失败！"),this);
            msgbox->exec();
        }
    }

}
//初始化消息按钮
void MainWindow::Init_InforIcon(){
    //初始化消息ICON
    Infor_icon = new Mylabel(ui->TopWidget);
    Infor_icon->setGeometry(952,29,12,15);
    QPixmap Inforpixmap(":/new/mainwindow/pictures/information_alarm.png");
    Inforpixmap.scaled(Infor_icon->size(),Qt::KeepAspectRatio);
    Infor_icon->setScaledContents(true);
    Infor_icon->setPixmap(Inforpixmap);
    Infor_icon->setCursor(QCursor(Qt::PointingHandCursor));
    connect(Infor_icon,SIGNAL(LabelClicked()),this,SLOT(HeadClickedSlot()));
    //初始化消息数量ICON
    Infor_num_icon = new Mylabel(ui->TopWidget);
    Infor_num_icon->setGeometry(957,21,17,17);
    Infor_num_icon->setStyleSheet("background-color:red;"
                                  "border-radius:8px;"
                                  "color:white");

    Infor_num_icon->setAlignment(Qt::AlignCenter);
    Infor_num_icon->setCursor(QCursor(Qt::PointingHandCursor));
    connect(Infor_num_icon,SIGNAL(LabelClicked()),this,SLOT(HeadClickedSlot()));
    QSqlQuery query(db);
    int num=0;
    bool success = query.exec("select * from Decryption where emp_id='"+User_ID+"' and status =2 and (is_solved = '0' or is_solved = '2')");
    if(!success){
        qDebug() << "Thread:查询user失败";
    }else{
         while(query.next()){
             num++;
         }
    }
    QSqlQuery Init_FriendQuery(db);
    bool Init_friendSuccess = Init_FriendQuery.exec("select * from friend where friend_id = '"+User_ID+"' and status = '0' and is_solved ='0'");
    if(!Init_friendSuccess){
        qDebug()<<"init friendRequest num fail";
    }
    else {
        while (Init_FriendQuery.next()) {
            FriendRequestCount++;
            num++;
        }
    }
    QString s = QString::number(num,10);
    QFont font("冬青黑体简体",8,75);
    Infor_num_icon->setFont(font);
    if(num==0){
        Infor_num_icon->hide();
    }
    else{
        Infor_num_icon->setText(s);
    }

}

void MainWindow::InforNum_Changed(){
    int num = informationNum+FriendRequestCount;
    if(num>0){
        Infor_num_icon->show();
    }
    else{
        Infor_num_icon->hide();//等于0的时候隐藏数量Label
    }
    QString s = QString::number(num,10);
    qDebug()<<"此时information数量为：";
    qDebug()<<s;
    Infor_num_icon->setText(s);

}

void MainWindow::on_pushButton_13_clicked()
{
    QString searchcontent = ui->SearchEdit->text();
   //用户输入手机号 获取用户昵称
    QNetworkRequest request;
    request.setUrl(QUrl("http://www.yunjiami1.com/cloud/Employee/FindUser.do"));
    QByteArray postData;
    postData.append("data=");//请求参数名
    postData.append(searchcontent);
    QNetworkReply *reply = d_accessManager->post(request,postData);



}

void MainWindow::ShowNewDownDialog(QString id){
    newDownloadFileIdList.append(id);
    if(newdownloadDlg->isOpened){
        emit newDownload();
    }else{
        newdownloadDlg = new newDownloadDialog(this);
        connect(newdownloadDlg,SIGNAL(Infor_numChange()),this,SLOT(newDownDialogInforInit()));
        connect(this,SIGNAL(newDownload()),newdownloadDlg,SLOT(change_list_view()));

        emit newDownload();
        newdownloadDlg->show();
        newdownloadDlg->isOpened = true;
    }


}
void MainWindow::downloadOneFile(QString id){
            //跳转到解密页面，开始下载
            InforNum_Changed();
            DecryptionItem *m1 = ui->MidStaWidget->findChild<DecryptionItem *>(id+"decryption");
            if(m1==nullptr){
                qDebug()<<"error";
            }
            else{
                 m1->downloadBtn->click();
//                Infor_requestNum--;
                on_DecryptionBtn_clicked();
            }
}
bool MainWindow::DeleteFileOrFolder( const QString& strPath )
{
    if( strPath.isEmpty() || !QDir().exists( strPath ) )
        return false;
    QFileInfo fileInfo( strPath );
    if( fileInfo.isFile() )
        QFile::remove( strPath );
    else if( fileInfo.isDir() )
    {
        QDir qDir( strPath );
        qDir.setFilter( QDir::AllEntries | QDir::NoDotAndDotDot );
        QFileInfoList fileInfoLst = qDir.entryInfoList();
        foreach( QFileInfo qFileInfo, fileInfoLst )
        {
            if( qFileInfo.isFile() )
                qDir.remove( qFileInfo.absoluteFilePath() );
            else
            {
                DeleteFileOrFolder( qFileInfo.absoluteFilePath() );
                qDir.rmdir( qFileInfo.absoluteFilePath() );
            }
        }
        qDir.rmdir( fileInfo.absoluteFilePath() );
    }
    return true;
}
void MainWindow::closeEvent(QCloseEvent *event){
    if(forceFlag == false){
        MsgBox *msgbox = new MsgBox(1,"确认关闭系统吗？",this);
        int reply = msgbox->exec();
        if(reply == QDialog::Accepted){
            QString downPath = "C:/CloundSafe/"+User_qqNum+"/Decrypt/ykey/";
            DeleteFileOrFolder(downPath);
            QSqlQuery query(db);
            query.exec("update UserStatus set status = 0 where emp_phone = '" + UserPhoneNum + "'");
            thread_11->exit(0);
            workThread->exit(0);
            //qApp->exit(0);
            QApplication::closeAllWindows();
            event->accept();
        }else{
            event->ignore();
        }
    }else{
        thread_11->exit(0);
        workThread->exit(0);
        //ConnectionPool::release();
        QApplication::setQuitOnLastWindowClosed(true);

        event->accept();
        qApp->exit(773);
        QProcess::startDetached(qApp->applicationFilePath(),QStringList());
    }
}

void MainWindow::on_pushButton_15_clicked()
{

    this->close();
}

void MainWindow::on_pushButton_14_clicked()
{
    this->showMinimized();
}
void MainWindow::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        mouse_press = true;
            dragPosition = event->globalPos() - frameGeometry().topLeft();
            event->accept();
        }
    event->ignore();
}
void MainWindow::mouseMoveEvent(QMouseEvent *event){
    if (event->buttons() & Qt::LeftButton) {
        if(mouse_press == true){
            move(event->globalPos() - dragPosition);
            event->accept();
        }
        }
    event->ignore();
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    mouse_press = false;
    event->ignore();
}
void MainWindow::paintEvent(QPaintEvent *event)
{

}
void MainWindow::FriendListWidgetHide(){
    if(isFriendListHide == 0){
      friendListWidget->hide();
      friendIcon->setStyleSheet("border-image: url(:/new/mainwindow/pictures/newfriend_hide.png);");
      friendIcon->setGeometry(ui->RightWidget->width()/2-81,7,8,15);
      isFriendListHide = 1;
    }
    else{
      friendListWidget->show();
      friendIcon->setStyleSheet("border-image: url(:/new/mainwindow/pictures/newfriend_show.png);");
      friendIcon->setGeometry(ui->RightWidget->width()/2-85,11,15,8);
      isFriendListHide = 0;
    }
}

void MainWindow::internet_Disconnected(){
    MsgBox *msgbox = new MsgBox(2,QStringLiteral("网络连接错误！"),this);
    msgbox->exec();
}

void MainWindow::RecvNewFriendReq(){
    FriendRequestCount++;
    InforNum_Changed();
}
//同意添加好友处理函数
void MainWindow::NewFriendAgree(){
    //刷新好友列表
    friendListWidget->clear();//清空好友列表
    //重新加载好友列表
    QSqlQuery query(db);
    bool friendSelSuc = query.exec("select * from friend where user_id ='"+User_ID+"' and status = '1'");
    if(!friendSelSuc){
        qDebug()<<"查询好友失败";
        return;
    }
    else{
        qDebug()<<"查询好友成功";
        //将查询的好友插入视图中
        int count = 0;
        while(query.next()){
            QString Friend_nickname = query.record().value("friend_nickname").toString();
            QListWidgetItem *add_item = new QListWidgetItem(friendListWidget);
            add_item->setIcon(QIcon(":/new/mainwindow/pictures/FriendListIcon_new.png"));
            add_item->setText(Friend_nickname);
            add_item->setFont(m);
            add_item->setTextAlignment(Qt::AlignLeft|Qt::AlignLeft);
            add_item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            add_item->setSizeHint(QSize(ui->RightWidget->width()-45,34));
            add_item->setTextAlignment(Qt::AlignVCenter);
            friendListWidget->insertItem(count,add_item);
            count++;
        }
    }
}
void MainWindow::forceShut(){
    MsgBox *msgbox = new MsgBox(2,"您的账号在其他地方登陆，您已被迫下线，\n如非本人操作，请修改密码",this);
    msgbox->exec();
    forceFlag = true;
    this->close();
}
void MainWindow::FileIsIgnored(){
    //查询数据库找到当前状态为4的数据
    QSqlQuery query(db);
    QString DeleteID;
    bool success = query.exec("select * from Decryption where oemp_id = '"+User_ID+"' and status = 4");
    if(!success){
        qDebug()<<"MainWindow:FileIsIgnore查询失败";
        return;
    }else{
        while(query.next()){
            DeleteID = query.record().value("id").toString();
            DecryptionItem *d1 = ui->MidStaWidget->findChild<DecryptionItem *>(DeleteID+"decryption");
            if(d1 == nullptr){
                continue;
                qDebug()<<"FileIsIgnored:查找Item失败";
            }
            else{
                //找到被忽略的Item,并删除Item
                delete d1;
            }
        }
        //在删除完成后重新布局
        delete decryptionViewController->layout();
        decryptionViewController->vbox->setMargin(0);
        decryptionViewController->vbox->setSpacing(0);
        QWidget *newItemWidget = new QWidget();
        newItemWidget->setContentsMargins(0,0,0,0);
        newItemWidget->setLayout(decryptionViewController->vbox);
        newScrollArea->setWidget(newItemWidget);
        newScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
//        newScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        QVBoxLayout *newVbox = new QVBoxLayout();
        newVbox->setMargin(0);
        newVbox->setSpacing(0);
        newVbox->addWidget(newScrollArea);
        decryptionViewController->setLayout(newVbox);
    }

}

//批量分享按钮
void MainWindow::on_pushButton_groupshare_clicked()
{
    if(groupShareDialogFlag){

    }else{
    int flag = 0;
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
                flag = 1;
                file_id_list = file_id_list + "||" +file_id_d;
                }
            }
        }
    if(flag == 1){

        grpShareDlg = new groupshareDialog();
        grpShareDlg->show();
        groupShareDialogFlag = true;

    }else if(flag == 0){
        MsgBox *msgbox = new MsgBox(3,QStringLiteral("请选择需要批量分享的条目！"),this);
        msgbox->exec();
    }
    flag = 0;
    }
}

void MainWindow::ChangeDecItemProBar(int value, QString itemID){
    DecryptionItem *m1 = ui->MidStaWidget->findChild<DecryptionItem *>(itemID+"decryption");
    QProgressBar *n1 = ui->MidStaWidget->findChild<QProgressBar *>(itemID+"decryption");
    m1->progressBar->setValue(value);
    if(value ==100){
        m1->fileDescription->setText("文件已加密需下载密钥文件");
        m1->downloadBtn->setText("申请解密");
        m1->label->show();
        delete n1;
        ReLayout();
        //跳出弹框显示下载时间
        qDebug()<<"downtime现在为：";
        qDebug()<<m1->down_time;
        QString time_get = CreateTimeTitle(m1->down_time);
        QString time_title = "下载完成！下载时间为：";
        time_title.append(time_get);
        MsgBox *msgbox = new MsgBox(4,time_title,this);
        msgbox->exec();
        //解除原有信号槽
        disconnect(m1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(DeProgressBarStart()));
        //连接新的信号槽
        connect(m1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(getFileID()));
    }
}
void MainWindow::newDownDialogInforInit(){
    Infor_requestNum = 0;
    InforNum_Changed();
}

void MainWindow::on_finen_checkBox_stateChanged(int arg1)
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
            if(findecrypt_flag == 0){
                checkcheck->setChecked(true);
            }else if(findecrypt_flag == 1){
                checkcheck->setChecked(false);
            }
        }
        if(findecrypt_flag == 0){
            findecrypt_flag = 1;
//            ui->finen_checkBox->setStyleSheet("QCheckBox::indicator {width: 13px;height: 13px;}"
//                                              "QCheckBox {color:#3A8CFF}");
        }else if(findecrypt_flag == 1){
            findecrypt_flag =0;
//            ui->finen_checkBox->setStyleSheet("QCheckBox::indicator {width: 13px;height: 13px;}"
//                                              "QCheckBox {color:black}");
        }

    }
}

void MainWindow::on_finen_checkBox_2_stateChanged(int arg1)
{
    QSqlQuery query(db);
    bool success = query.exec("select * from Decryption where status = 5 and oemp_id ='" + User_ID+"'");
    if(!success){
        qDebug() << "查询密文失败";
        return;
    }else{
        qDebug()<<"查询成功";
        QList<QCheckBox*> checkboxList = ui->MidStaWidget->currentWidget()->findChildren<QCheckBox*>();
            for(int i = 0; i < checkboxList.size(); i++)
            {

                QCheckBox *checkcheck = checkboxList.at(i);
                qDebug()<< checkcheck->objectName();
                if(finencrypt_flag == 0){
                    checkcheck->setChecked(true);
                }else if(finencrypt_flag == 1){
                    checkcheck->setChecked(false);
                }
            }
//        while(query.next())
//        {
//            QString file_id = query.record().value("file_id").toString();
//            QCheckBox *checkcheck = ui->MidStaWidget->findChild<QCheckBox*>(file_id+"check");
//            if(finencrypt_flag == 0){
//                checkcheck->setChecked(true);
//            }else if(finencrypt_flag == 1){
//                checkcheck->setChecked(false);
//            }
//        }
    }
    if(finencrypt_flag == 0){
        finencrypt_flag = 1;
//        ui->finen_checkBox_2->setStyleSheet("QCheckBox::indicator {width: 13px;height: 13px;}"
//                                            "QCheckBox {color:#3A8CFF}");
    }else if(finencrypt_flag == 1){
        finencrypt_flag =0;
//        ui->finen_checkBox_2->setStyleSheet("QCheckBox::indicator {width: 13px;height: 13px;}"
//                                            "QCheckBox {color:black}");
    }
}

void MainWindow::on_de_checkBox_stateChanged(int arg1)
{
    QSqlQuery query(db);
    bool success = query.exec("select * from Decryption where oemp_id = '"+User_ID+"'");
    if(!success){
        qDebug()<<"checkBox:查询数据库失败";
        return;
    }
    else{
        while(query.next()){
            QString m_ID = query.record().value("id").toString();
            QCheckBox *checkbox = ui->MidStaWidget->findChild<QCheckBox *>(m_ID+"Decheck");
            if(checkbox==nullptr){
                continue;
            }
            else{
                if(DeSelect_flag==0){
                    checkbox->setChecked(true);
                }
                else if(DeSelect_flag == 1){
                    checkbox->setChecked(false);
                }
            }
        }
        if(DeSelect_flag ==0){
            DeSelect_flag =1;
//            ui->pushButton->setStyleSheet(" QPushButton {border-image: url(:/new/mainwindow/pictures/allselect_pressed.png);} "
//                                          " QPushButton:hover {border-image: url(:/new/mainwindow/pictures/allselect_hover.png);}");
        }
        else{
            DeSelect_flag =0;
//            ui->pushButton->setStyleSheet(" QPushButton {border-image: url(:/new/mainwindow/pictures/allselect.png);} "
//                                             " QPushButton:hover {border-image: url(:/new/mainwindow/pictures/allselect_hover.png);}");
        }
    }
}
void MainWindow::clearCheckBox(){
    findecrypt_flag = 0;//已解密全选判断变量
    finencrypt_flag = 0;//已加密全选判断变量
    ui->finen_checkBox->setChecked(false);
    ui->finen_checkBox_2->setChecked(false);
}
void MainWindow::httpDowload(){
    if(avatorFile){
         avatorFile->write(avatorReply->readAll());
    }
}
//请求完成 文件下载成功
void MainWindow::httpDowloadFinished(){
    //刷新文件
    avatorFile->flush();
    avatorFile->close();

    emit decryptSignal(dcrypt_ykeyName,dcrypt_contentName);
    dcrypt_ykeyName = "";
    dcrypt_contentName = "";
    avatorFile=nullptr;
}
void MainWindow::decryptSlot(QString keypath,QString filepath){
    DecryptionFile *fileD = new DecryptionFile();
    QString decryptpath = User_qqPath+"//Decrypt//file//" + keypath;
    QString key_path = User_qqPath+"//Decrypt//ykey//" + keypath;
    QString content_path = User_qqPath+"//Decrypt//content//" + filepath;
    qDebug()<<key_path+"::::"+content_path+"::::"+decryptpath;
    int a = fileD->decryptFile(key_path,content_path,decryptpath,100,1);
    if(a==54){
        emit FinishedDecryption(dcrypt_ykeyId);
    }
    else{
        qDebug()<<a;
    }
    dcrypt_ykeyId = "";
}
void MainWindow::finishDecryption(QString decryption_id){
    QSqlQuery query(db);
    bool updateRequest = query.exec("update Decryption set status = 15 where file_id = '"+decryption_id.toUtf8()+"'");
    if(updateRequest){
        QDateTime time = QDateTime::currentDateTime();
        QString time_str = time.toString("yyyy-MM-dd hh:mm:ss");//获取当前时间
        QSqlQuery updatetime(db);
        bool updateTimeSuccess = updatetime.exec("update Decryption set apply_time = '"+time_str+"'where file_id = '"+decryption_id.toUtf8()+"'");
        if(!updateTimeSuccess){
            qDebug()<<"update failed";
        }else{
            //ReLayout();
            QSqlQuery query1(db);
            bool success = query1.exec("select * from Decryption where file_id='"+decryption_id.toUtf8()+"'");
            if(!success){
                qDebug() << "查询user失败";
                return;
            }else{
                while(query1.next()){
                    QString delete_id = query1.record().value("id").toString();
                    DecryptionItem *m1 = ui->MidStaWidget->findChild<DecryptionItem *>(delete_id+"decryption");
                    delete m1;
                    ReLayout();
                }
            }
        }
    }
    else{
        qDebug()<<"shibai";
    }
}
void MainWindow::finishedSlot(QNetworkReply *reply){
    if(reply->error()==QNetworkReply::NoError){
        QByteArray bytes = reply->readAll();
        qDebug()<<bytes;
        if(httpFlag == 3){
            QJsonParseError jsonError;//Qt5新类
            QJsonDocument json = QJsonDocument::fromJson(bytes, &jsonError);//Qt5新类
            if (jsonError.error == QJsonParseError::NoError)
            {
                if (json.isObject())
                {
                    QJsonObject rootObj = json.object();
                    QString rootpath;
                    //是否含有key  rootpath
                    if (rootObj.contains("status"))
                    {
                        //取出key为rootpath的值
                        QJsonValue value = rootObj.value("status");
                        //判断是否是string类型
                        if (value.isString())
                        {
                            qDebug()<<rootpath;
                            rootpath = value.toString();
                            if(rootpath!="Success")
                            {
                            }
                        }
                    }
                    if (rootObj.contains("content"))
                    {
                        //取出key为rootpath的值
                        QJsonValue value = rootObj.value("content");
                        QString content;
                        //判断是否是string类型
                        if (value.isString())
                        {
                            qDebug()<<rootpath;
                            content = value.toString();
                            if(content.contains("employee is null",Qt::CaseSensitive))
                            {
                                MsgBox *msgbox = new MsgBox(3,QStringLiteral("本账号未获授权！"),this);
                                msgbox->exec();
                            }else if(content.contains("file is null",Qt::CaseSensitive))
                            {
                                MsgBox *msgbox = new MsgBox(3,QStringLiteral("该密钥已被删除！"),this);
                                msgbox->exec();
                            }else if(content.contains("request",Qt::CaseSensitive))
                            {
                                MsgBox *msgbox = new MsgBox(4,QStringLiteral("请求已提交，请等待！"),this);
                                msgbox->exec();
                            }else if(content.contains("file under review",Qt::CaseSensitive))
                            {
                                MsgBox *msgbox = new MsgBox(4,QStringLiteral("请求提交成功，请等待审核！"),this);
                                msgbox->exec();
                            }else if(content.contains("file audit not passed",Qt::CaseSensitive))
                            {
                                MsgBox *msgbox = new MsgBox(2,QStringLiteral("请求已被驳回！"),this);
                                msgbox->exec();
                            }else if(content.contains("fp_id",Qt::CaseSensitive))
                            {
                                QJsonParseError jsonError1;//Qt5新类
                                QJsonDocument json1 = QJsonDocument::fromJson(content.toLocal8Bit().data(), &jsonError1);//Qt5新类

                               //QJsonValue valueArray = rootObj.value("content");
                               if (json1.isArray()){
                                   qDebug()<<"是!";
                                   QJsonArray array = json1.array();
                                   for (int i = 0; i < array.count();i++)
                                   {
                                       QJsonValue childValue = array[i];
                                       if (childValue.isObject())
                                       {
                                           QString link;
                                           QJsonObject  childObject = childValue.toObject();
                                           if (childObject.contains("fp_origin_path"))
                                           {
                                               QJsonValue valueJson = childObject.value("fp_origin_path");
                                               if (valueJson.isString())
                                               {
                                                   QStringList tmp = valueJson.toString().split("/");
                                                   dcrypt_ykeyName = tmp.at(tmp.length()-1);
                                                   qDebug()<<dcrypt_ykeyName;
                                               }
                                           }
                                           if (childObject.contains("fp_content"))
                                           {
                                               QJsonValue valueJson = childObject.value("fp_content");
                                               if (valueJson.isString())
                                               {
                                                   link = valueJson.toString();
                                                   QDir file;
                                                   QString fileName="C://CloundSafe//"+User_qqNum+"//Decrypt//ykey//"+dcrypt_ykeyName;
                                                   //dcrypt_contentName = "tmp.content";
                                                   avatorFile=new QFile(fileName);
                                                   //判断文件是否可写入 不可写删除 指针赋值0
                                                   if(!avatorFile->open(QIODevice::WriteOnly)){
                                                      delete avatorFile;
                                                      avatorFile = nullptr;
                                                      return;
                                                   }
                                                   //开始请求 下载文件
                                                   QUrl  serviceUrl = QUrl(link);
                                                   avatorManager = new QNetworkAccessManager(this);
                                                   //get方式请求 如需加密用post
                                                   avatorReply=avatorManager->get(QNetworkRequest(serviceUrl));
                                                   connect(avatorReply,SIGNAL(readyRead()),this,SLOT(httpDowload()));//数据写入
                                                   connect(avatorReply,SIGNAL(finished()),this,SLOT(httpDowloadFinished()));//请求完成
                                               }
                                           }
                                       }
                                   }
                                }
                            }
                        }
                    }
                }
            }
            httpFlag = 0;
        }else{
            QJsonParseError jsonError;//Qt5新类
            QJsonDocument json = QJsonDocument::fromJson(bytes, &jsonError);//Qt5新类
            if(jsonError.error==QJsonParseError::NoError){
                if(json.isObject()){
                    QJsonObject rootObj = json.object();
                    QString rootpath;
                    if(rootObj.contains("status")){
                        QJsonValue value = rootObj.value("status");
                        if(value.isString()){
                            rootpath = value.toString();
                            if(rootpath=="success"){
                                QJsonObject contentValue = rootObj.value("content").toObject();
                                QJsonValue CV = rootObj.value("content");
                                QJsonValue a = contentValue.take("emp_name");
                                QString output = a.toString();
                                qDebug()<<output;
                                output = CV.toString();
                                qDebug()<<output;
                                QString ename = "emp_name";
                                QString lastindex = "emp_password";
                                QString key = "qq_num";
                                int index = output.indexOf(ename);
                                int last = output.indexOf(lastindex);
                                int qq = output.indexOf(key);
                                qq +=9;

                                //last-=5;
                                qDebug()<<last;
                                index+=11;
                                qDebug()<<index;
                                int lenth = last-index-3;
                                QString ddd = output.mid(index,lenth);
                                if(output==NULL){

                                }
                                else{
                                   QListWidgetItem *finditem = new QListWidgetItem();
                                   QList <QListWidgetItem *> item = friendListWidget->findItems(ddd,Qt::MatchContains);
                                   if(item.size()==0){
                                       qDebug()<<"查找0失败";
                                   }
                                   else{
                                       finditem = item[0];
                                   }
                                   if(finditem ==nullptr){
                                       qDebug()<<"查找失败！";
                                   }
                                   else{
                                        friendListWidget->setCurrentItem(finditem);
                                   }
                                }

                            }
                            else{
                                //查询失败
                                qDebug()<<"查询失败";

                            }
                        }
                    }
                }
            }
        }
    }
    else{
        qDebug()<<"handle errors here";
        QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        qDebug( "found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
        qDebug(qPrintable(reply->errorString()));

    }
    reply->deleteLater();
}

void MainWindow::setDecItemDowntime(QString id, double time){
    DecryptionItem *m1 = ui->MidStaWidget->findChild<DecryptionItem *>(id+"decryption");
    m1->down_time=time;
    qDebug()<<"downtime赋值为：";
    qDebug()<<m1->down_time;
}

void MainWindow::setDecItemDectime(QString id, double time){
    qDebug()<<"mainWINDOW:SETDECITEMDECTIME："+id;
    DecryptionItem *m1 = ui->MidStaWidget->findChild<DecryptionItem *>(id+"decryption");
    if(m1==nullptr){
        qDebug()<<"查找ID失败";
        return;
    }
    m1->dec_time=time;
    qDebug()<<"dec赋值为";
    qDebug()<<m1->dec_time;
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
        QString dec_time = CreateTimeTitle(m1->dec_time);
        QString dec_title = "文件解密同意并成功解密，解密时间为：";
        dec_title.append(dec_time);
        //解密完成后删除控件
        delete m1;
        //重新布局
        delete decryptionViewController->layout();
        decryptionViewController->vbox->setMargin(0);
        decryptionViewController->vbox->setSpacing(0);
        QWidget *newItemWidget = new QWidget();
        newItemWidget->setContentsMargins(0,0,0,0);
        newItemWidget->setLayout(decryptionViewController->vbox);
        newScrollArea->setWidget(newItemWidget);
        newScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
        QVBoxLayout *newVbox = new QVBoxLayout();
        newVbox->setMargin(0);
        newVbox->setSpacing(0);
        newVbox->addWidget(newScrollArea);
        decryptionViewController->setLayout(newVbox);
        MsgBox *msgbox = new MsgBox(4,dec_title,this);
        msgbox->exec();
    }
    else{
        return;
    }
}

QString MainWindow::CreateTimeTitle(double time){
    QString temstr;
    int ss = 1;
    int mi = ss * 60;
    int hh = mi * 60;
    long hour = time / hh;
    long minute = (time - hour * hh) / mi;
    long second = (time - hour * hh - minute * mi) / ss;

    QString hou = QString::number(hour,10);
    QString min = QString::number(minute,10);
    QString sec = QString::number(second,10);
    if (time < 60){
        temstr.append(QString::number(time,10,2));
        temstr.append("秒");
    }else{
        //显示时间
        if (hour >0){
            temstr.append(hou);
            temstr.append("时");
        }
        if (minute >0){
            temstr.append(min);
            temstr.append("分");
        }
        if (second >0){
            temstr.append(sec);
            temstr.append("秒");
        }
    }
    return temstr;
}

void MainWindow::on_pushButton_16_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
       this,
       "请选择需要解密的文件",
       QDir::currentPath(),
       "All files(*.*)");
    QFileInfo fi;
    fi = QFileInfo(filename);
    QString this_cyphertext_name = fi.fileName();
    DecryptionFile *fileD = new DecryptionFile();
    char v_dest[100];//version
    char f_dest[100];//fileId
    char u_dest[100];//userId
    if (!filename.isNull()) { //用户选择了文件
        qDebug()<<filename;

        int __result = fileD->VerifyFile(filename,v_dest,f_dest,u_dest);
        QString __version = QString(v_dest);
        QString __keyId = QString(f_dest).toUtf8();//.section("}",0,0)+"}";
        QString __userId = QString(QLatin1String(u_dest));;
        qDebug()<<__version;
        qDebug()<<__keyId;
        qDebug()<<__userId;
        qDebug()<<__result;
        qDebug()<<__userId;
        qDebug()<<__keyId;
        if(__version.length()){

            if(__version == "version1011"){
                QSqlQuery query(db);
                //QString _fileId;
                QString empid;
                //QString _filename;
                //QString filesize = "安卓系统文件";
                QString extraRate;
                QDateTime time = QDateTime::currentDateTime();
                QString time_str = time.toString("yyyy-MM-dd hh:mm:ss");
                QString file_level;
                bool success = query.exec("select * from vfile where file_id = '"+__keyId.toUtf8()+"'");
                bool trueSuccess = false;
                if(!success){
                    qDebug()<<"Failed";
                }
                else{
                    qDebug()<<"1";
                    while(query.next()){
                        trueSuccess = true;
                        qDebug()<<"2.1";
                        //_fileId = query.record().value("article_id").toString();
                        empid = __userId;
                        //_filename = query.record().value("article_name").toString();
                        //filesize = query.record().value("article_size").toString();
                        extraRate = "100";
                        //QString rate = query.record().value("file_encrypt_num").toString();
                        file_level = "加密等级：弱";
                        QUuid _strid = QUuid::createUuid();
                        QString _id = _strid.toString();
                        QString path = "C://CloundSafe//"+User_qqNum+"//Decrypt//content//"+_id;
                        //dcrypt_contentName = _id;
                        if(copyFileToPath(filename,path,1)){
                            qDebug()<<"2";
                            QUuid strid = QUuid::createUuid();
                            QString id = strid.toString();
                            QSqlQuery insertQuery(db);
                            bool insertSuccess = insertQuery.exec("insert into Decryption values('"+id+"','"+__keyId+"','"+this_cyphertext_name+"','"+empid+"','"+_id+"','"+User_ID+"','',11,'"+time_str+"','" "',0,0,"+extraRate+")");
                            if(!insertSuccess){
                                qDebug()<<"LinkInsertFailed";
                            }else{
                                ReLayout();
                                qDebug()<<"3";
                                QSqlQuery query1(db);
                                bool success = query1.exec("select * from Decryption where oemp_id='"+User_ID+"' order by createtime DESC");
                                if(!success){
                                    qDebug() << "查询user失败";
                                    return;
                                }else{
                                    qDebug()<<"查询成功";
                                    //将数据库查到的数据添加到视图中
                                    while(query1.next()){
                                        qDebug()<<"4";
                                        DecryptionItem *v1 =  new DecryptionItem();
                                        QString fName = query1.record().value("file_name").toString();
                                        QString rate = query1.record().value("file_encrypt_num").toString();
                                        QString file_level;
                                        if(rate == "20"){
                                            file_level = "加密等级：强";
                                        }else if(rate == "30"){
                                            file_level = "加密等级：中";
                                        }else {
                                            file_level = "加密等级：弱";
                                        }
                                        v1->fileLevel->setText(file_level);
                                        int fontSize = fontMetrics().width( fName );//获取之前设置的字符串的像素大小
                                        int pos = 0;int d_count = 0;
                                        pos = fName.indexOf(".");
                                        while (pos>-1) {
                                            d_count++;
                                            pos = fName.indexOf(".",pos+1);
                                        }
                                        QString filetype_extra = fName.section(".",d_count-1,d_count-1).mid(fName.section(".",d_count-1,d_count-1).length()-2)+"."+fName.section(".",d_count,d_count).trimmed().toStdString().c_str() ;
                                        if( fontSize >= v1->fileName->width()-100 ) //与label自身相比较
                                        {
                                            QString str = fontMetrics().elidedText( fName, Qt::ElideRight, v1->fileName->width()-150 )+filetype_extra;//返回一个带有省略号的字符串
                                            v1->fileName->setText( str );       //重新设置label上的字符串
                                        }else{
                                            v1->fileName->setText(fName);
                                        }
                                        QString m_filesize = query1.record().value("file_size").toString();
                                        double filesize_double = m_filesize.toDouble();
                                        int filesize_count = 0;
                                        while(filesize_double>1024){
                                            filesize_double = filesize_double/1024;
                                            filesize_count++;
                                        }
                                        if(filesize_count == 0){
                                            v1->fileSize->setText(QString::number(filesize_double, 10, 2)+"KB");
                                        }else if(filesize_count == 1){
                                            v1->fileSize->setText(QString::number(filesize_double, 10, 2)+"MB");
                                        }else if(filesize_count == 2){
                                            v1->fileSize->setText(QString::number(filesize_double, 10, 2)+"GB");
                                        }else if(filesize_count == 3){
                                            v1->fileSize->setText(QString::number(filesize_double, 10, 2)+"TB");
                                        }
                                        //设置文件大小
                                        v1->timeLabel->setText(query1.record().value("createtime").toString());
                                        //设置fileIcon的图片
                                        QString filetype = query1.record().value("file_name").toString().section(".",1,1).trimmed().toStdString().c_str();
                                        if((filetype=="jpg")||(filetype=="png")||(filetype=="jpeg")||(filetype=="bmp")||(filetype=="gif")||(filetype=="webp")||(filetype=="psd")||(filetype=="svg")||(filetype=="tiff")){
                                            QPixmap pixmap(":/new/mainwindow/pictures/pic_icon.png");
                                            pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                                            v1->fileIcon->setScaledContents(true);
                                            v1->fileIcon->setPixmap(pixmap);
                                        }else if((filetype=="avi")||(filetype=="rmvb")||(filetype=="rm")||(filetype=="asf")||(filetype=="divx")||(filetype=="wmv")||(filetype=="mp4")||(filetype=="mkv")||(filetype=="vob")||(filetype=="mpeg")){
                                            QPixmap pixmap(":/new/mainwindow/pictures/video_icon.png");
                                            pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                                            v1->fileIcon->setScaledContents(true);
                                            v1->fileIcon->setPixmap(pixmap);
                                        }else if((filetype=="doc")||(filetype=="docx")||(filetype=="xls")||(filetype=="xlsx")||(filetype=="ppt")||(filetype=="pptx")||(filetype=="txt")||(filetype=="docm")){
                                            QPixmap pixmap(":/new/mainwindow/pictures/doc_icon.png");
                                            pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                                            v1->fileIcon->setScaledContents(true);
                                            v1->fileIcon->setPixmap(pixmap);
                                        }else if((filetype=="rar")||(filetype=="zip")||(filetype=="arj")||(filetype=="z")){
                                            QPixmap pixmap(":/new/mainwindow/pictures/zip_icon.png");
                                            pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                                            v1->fileIcon->setScaledContents(true);
                                            v1->fileIcon->setPixmap(pixmap);
                                        }else if((filetype=="mp3")||(filetype=="wma")||(filetype=="wav")||(filetype=="ape")||(filetype=="flac")||(filetype=="ogg")||(filetype=="aac")){
                                            QPixmap pixmap(":/new/mainwindow/pictures/music_icon.png");
                                            pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                                            v1->fileIcon->setScaledContents(true);
                                            v1->fileIcon->setPixmap(pixmap);
                                        }else{
                                            QPixmap pixmap(":/new/mainwindow/pictures/else_icon.png");
                                            pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                                            v1->fileIcon->setScaledContents(true);
                                            v1->fileIcon->setPixmap(pixmap);
                                            v1->elseLabel->setText(filetype.left(3));
                                            v1->elseLabel->raise();
                                        }
                                        QString initID = query1.record().value("id").toString();
                                        v1->checkBox->setObjectName(initID+"Decheck");//设置checkbox的ID
                                        v1->setObjectName(initID+"decryption");//设置Item的ID
                                        v1->downloadBtn->setObjectName(initID+"btn");//设置downloadBtn的ID
                                        v1->downloadBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: #3A8CFF;color:white;}QPushButton:pressed{background-color: rgb(139,159,185);}");
                                        //连接信号槽
                   //                   connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                                        if(query1.record().value("status").toString()=="0"){//待下载状态
                                            RequestIDArray[RequsetIndex]=initID;
                                            RequsetIndex++;
                                            v1->fileDescription->setText("主体文件指定分享需确认下载.");
                                            v1->downloadBtn->setText("确认下载");
                                            connect(v1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(OssDownLoadFile()));
                                            decryptionViewController->vbox->addWidget(v1);//将v1添加到视图中
                    //                      connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                                            f_progressBar = new QProgressBar();
                                            f_progressBar = v1->progressBar;
                                            f_progressBar->setObjectName(v1->objectName());
                   //                       v1->fileIcon->stackUnder(f_progressBar);
                   //                       v1->timeLabel->stackUnder(f_progressBar);
                                            f_progressBar->raise();
                                            f_progressBar->hide();
                                            f_progressBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);  // 对齐方式
                                            //decryptionViewController->vbox->addWidget(f_progressBar);
                                        }
                                        else if(query1.record().value("status").toString()=="1"){//待申请状态
                                            v1->fileDescription->setText("文件已加密需下载密钥文件.");
                                            v1->downloadBtn->setText("申请解密");
                                            v1->label->show();
                   //                       connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                                            connect(v1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(getFileID()));
                                            decryptionViewController->vbox->addWidget(v1);//将v1添加到视图中
                                        }
                                        else if(query1.record().value("status").toString()=="2"){//申请等待状态
                                            v1->fileDescription->setText("正在申请解密，请等待！");
                                            v1->downloadBtn->setText("申请中");
                                            v1->label->show();
                   //                       connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                                            decryptionViewController->vbox->addWidget(v1);
                                        }
                                        else if(query1.record().value("status").toString()=="3"){//申请请求已同意
                                            v1->fileDescription->setText("正在申请解密，请等待！");
                                            v1->downloadBtn->setText("申请中");
                                            v1->label->show();
                                            decryptionViewController->vbox->addWidget(v1);
                                        }else if(query1.record().value("status").toString()=="11"){//安卓端本地解密
                                            v1->fileDescription->setText("文件已加密需下载密钥文件.");
                                            v1->downloadBtn->setText("解密");
                                            v1->downloadBtn->setObjectName(query1.record().value("file_id").toString());
                                            v1->fileSize->setText("文件来自安卓系统");
                                            v1->label->show();
                   //                       connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                                            connect(v1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(androidDecrypt()));
                                            decryptionViewController->vbox->addWidget(v1);//将v1添加到视图中
                                            qDebug()<<"android!!!!!!!!!";
                                        }else if(query1.record().value("status").toString()=="21"){//ios端本地解密
                                            v1->fileDescription->setText("文件已加密需下载密钥文件.");
                                            v1->downloadBtn->setText("解密");
                                            v1->fileSize->setText("文件来自iOS系统");
                                            v1->downloadBtn->setObjectName(query1.record().value("file_id").toString());
                                            v1->label->show();
                   //                       connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                                            connect(v1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(getFileID()));
                                            decryptionViewController->vbox->addWidget(v1);//将v1添加到视图中
                                        }else if(query1.record().value("status").toString()=="12"){//安卓端本地解密第二步
                                            v1->fileDescription->setText("已申请解密！");
                                            v1->downloadBtn->setText("解密");
                                            v1->fileSize->setText("文件来自安卓系统");
                                            v1->downloadBtn->setObjectName(query1.record().value("file_id").toString());
                                            v1->label->show();
                                            connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                                            decryptionViewController->vbox->addWidget(v1);
                                        }else if(query1.record().value("status").toString()=="22"){//ios端本地解密第二步
                                            v1->fileDescription->setText("已申请解密！");
                                            v1->downloadBtn->setText("解密");
                                            v1->fileSize->setText("文件来自iOS系统");
                                            v1->downloadBtn->setObjectName(query1.record().value("file_id").toString());
                                            v1->label->show();
                                            connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                                            decryptionViewController->vbox->addWidget(v1);
                                        }
                                    }
                                    decryptionViewController->vbox->setMargin(0);
                                    decryptionViewController->vbox->setSpacing(0);
                                    QWidget *newItemWidget = new QWidget();
                                    newScrollArea = new QScrollArea();
                   //               newScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                                    newScrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
                                                                                    "{"
                                                                                    "width:8px;"
                                                                                    "background:rgba(0,0,0,0%);"
                                                                                    "margin:0px,0px,0px,0px;"
                                                                                    "padding-top:9px;"
                                                                                    "padding-bottom:9px;"
                                                                                    "}"
                                                                                    "QScrollBar::handle:vertical"
                                                                                    "{"
                                                                                    "width:8px;"
                                                                                    "background:rgba(0,0,0,25%);"
                                                                                    " border-radius:4px;"
                                                                                    "min-height:20;"
                                                                                    "}"
                                                                                    "QScrollBar::handle:vertical:hover"
                                                                                    "{"
                                                                                    "width:8px;"
                                                                                    "background:rgba(0,0,0,50%);"
                                                                                    " border-radius:4px;"
                                                                                    "min-height:20;"
                                                                                    "}"
                                                                                    "QScrollBar::add-line:vertical"
                                                                                    "{"
                                                                                    "height:5px;width:10px;"
                                                                                    "border-image:url(:/new/mainwindow/pictures/Scrollbar_bottom.png);"
                                                                                    "subcontrol-position:bottom;"
                                                                                    "}"
                                                                                    "QScrollBar::sub-line:vertical"
                                                                                    "{"
                                                                                    "height:5px;width:10px;"
                                                                                    "border-image:url(:/new/mainwindow/pictures/Scrollbar_top.png);"
                                                                                    "subcontrol-position:top;"
                                                                                    "}"
                                                                                    "QScrollBar::add-line:vertical:hover"
                                                                                    "{"
                                                                                    "height:9px;width:8px;"
                                                                                    "border-image:url(:/images/a/4.png);"
                                                                                    "subcontrol-position:bottom;"
                                                                                    "}"
                                                                                    "QScrollBar::sub-line:vertical:hover"
                                                                                    "{"
                                                                                    "height:9px;width:8px;"
                                                                                    "border-image:url(:/images/a/2.png);"
                                                                                    "subcontrol-position:top;"
                                                                                    "}"
                                                                                    "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
                                                                                    "{"
                                                                                    "background:rgba(0,0,0,5%);"
                                                                                    "border-radius:4px;"
                                                                                    "}");
                                    newItemWidget->setLayout(decryptionViewController->vbox);
                                    newScrollArea->setWidget(newItemWidget);
                                    newScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
                                    QVBoxLayout *newVbox = new QVBoxLayout();
                                    newVbox->setMargin(0);
                                    newVbox->setSpacing(0);
                                    newVbox->addWidget(newScrollArea);
                                    decryptionViewController->setLayout(newVbox);
                                    }
                            }

                        }else{
                            qDebug()<<"文件拷贝失败";
                        }
                    }
                    if(!trueSuccess){
                        MsgBox *msgbox = new MsgBox(3,QStringLiteral("所选文件是密文，但文件信息已被删除！"),this);
                        msgbox->exec();
                    }else{
                        MsgBox *msgbox = new MsgBox(3,QStringLiteral("所选文件是密文！"),this);
                        msgbox->exec();
                    }
                }
            }else if(__version == "version1012"){
            }else{
                __keyId = __keyId.section("}",0,0)+"}";
                QSqlQuery query(db);
                QString _fileId = "";
                QString empid;
                QString _filename;
                QString filesize;
                QString extraRate;
                QDateTime time = QDateTime::currentDateTime();
                QString time_str = time.toString("yyyy-MM-dd hh:mm:ss");
                QString file_level;
                bool success = query.exec("select * from varticle where key_id = '"+__keyId+"'");
                bool trueSuccess = false;
                qDebug()<<"id:::::::::::::::::::::::::::::::::"+__keyId;
                if(success == false){
                    qDebug()<<"Failed";
                }
                else{
                    while(query.next()){
                        trueSuccess = true;
                        _fileId = query.record().value("article_id").toString();
                        empid = query.record().value("emp_id").toString();
                        _filename = query.record().value("article_name").toString();
                        filesize = query.record().value("article_size").toString();
                        extraRate = query.record().value("encrypt_num").toString();
                        QString rate = query.record().value("file_encrypt_num").toString();
                        if(rate == "20"){
                            file_level = "加密等级：强";
                        }else if(rate == "30"){
                            file_level = "加密等级：中";
                        }else {
                            file_level = "加密等级：弱";
                        }
                        QString path = "C://CloundSafe//"+User_qqNum+"//Decrypt//content//"+_fileId;
                        if(copyFileToPath(filename,path,1)){
                            QUuid strid = QUuid::createUuid();
                            QString id = strid.toString();
                            QSqlQuery insertQuery(db);
                            bool insertSuccess = insertQuery.exec("insert into Decryption values('"+id+"','"+_fileId+"','"+_filename+"','"+empid+"','','"+User_ID+"','',1,'"+time_str+"','"+filesize+"',0,0,"+extraRate+")");
                            if(!insertSuccess){
                                qDebug()<<"LinkInsertFailed";
                            }
                            else{
                                ReLayout();
                                QSqlQuery query1(db);
                                bool success = query1.exec("select * from Decryption where oemp_id='"+User_ID+"' order by createtime DESC");
                                if(!success){
                                qDebug() << "查询user失败";
                                return;
                                }else{
                                    qDebug()<<"查询成功";
                                    //将数据库查到的数据添加到视图中
                                    while(query1.next()){
                                        DecryptionItem *v1 =  new DecryptionItem();
                                        QString fName = query1.record().value("file_name").toString();
                                        QString rate = query1.record().value("file_encrypt_num").toString();
                                        QString file_level;
                                        if(rate == "20"){
                                            file_level = "加密等级：强";
                                        }else if(rate == "30"){
                                            file_level = "加密等级：中";
                                        }else {
                                            file_level = "加密等级：弱";
                                        }
                                        v1->fileLevel->setText(file_level);
                                        int fontSize = fontMetrics().width( fName );//获取之前设置的字符串的像素大小
                                        int pos = 0;int d_count = 0;
                                        pos = fName.indexOf(".");
                                        while (pos>-1) {
                                            d_count++;
                                            pos = fName.indexOf(".",pos+1);
                                        }
                                        QString filetype_extra = fName.section(".",d_count-1,d_count-1).mid(fName.section(".",d_count-1,d_count-1).length()-2)+"."+fName.section(".",d_count,d_count).trimmed().toStdString().c_str() ;
                                        if( fontSize >= v1->fileName->width()-100 ) //与label自身相比较
                                        {
                                            QString str = fontMetrics().elidedText( fName, Qt::ElideRight, v1->fileName->width()-150 )+filetype_extra;//返回一个带有省略号的字符串
                                            v1->fileName->setText( str );       //重新设置label上的字符串
                                        }else{
                                            v1->fileName->setText(fName);
                                        }
                                        QString m_filesize = query1.record().value("file_size").toString();
                                        double filesize_double = m_filesize.toDouble();
                                        int filesize_count = 0;
                                        while(filesize_double>1024){
                                            filesize_double = filesize_double/1024;
                                            filesize_count++;
                                        }
                                        if(filesize_count == 0){
                                            v1->fileSize->setText(QString::number(filesize_double, 10, 2)+"KB");
                                        }else if(filesize_count == 1){
                                            v1->fileSize->setText(QString::number(filesize_double, 10, 2)+"MB");
                                        }else if(filesize_count == 2){
                                            v1->fileSize->setText(QString::number(filesize_double, 10, 2)+"GB");
                                        }else if(filesize_count == 3){
                                            v1->fileSize->setText(QString::number(filesize_double, 10, 2)+"TB");
                                        }
                                        //设置文件大小
                                        v1->timeLabel->setText(query1.record().value("createtime").toString());
                                        //设置fileIcon的图片
                                        QString filetype = query1.record().value("file_name").toString().section(".",1,1).trimmed().toStdString().c_str();
                                        if((filetype=="jpg")||(filetype=="png")||(filetype=="jpeg")||(filetype=="bmp")||(filetype=="gif")||(filetype=="webp")||(filetype=="psd")||(filetype=="svg")||(filetype=="tiff")){
                                            QPixmap pixmap(":/new/mainwindow/pictures/pic_icon.png");
                                            pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                                            v1->fileIcon->setScaledContents(true);
                                            v1->fileIcon->setPixmap(pixmap);
                                        }else if((filetype=="avi")||(filetype=="rmvb")||(filetype=="rm")||(filetype=="asf")||(filetype=="divx")||(filetype=="wmv")||(filetype=="mp4")||(filetype=="mkv")||(filetype=="vob")||(filetype=="mpeg")){
                                            QPixmap pixmap(":/new/mainwindow/pictures/video_icon.png");
                                            pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                                            v1->fileIcon->setScaledContents(true);
                                            v1->fileIcon->setPixmap(pixmap);
                                        }else if((filetype=="doc")||(filetype=="docx")||(filetype=="xls")||(filetype=="xlsx")||(filetype=="ppt")||(filetype=="pptx")||(filetype=="txt")||(filetype=="docm")){
                                            QPixmap pixmap(":/new/mainwindow/pictures/doc_icon.png");
                                            pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                                            v1->fileIcon->setScaledContents(true);
                                            v1->fileIcon->setPixmap(pixmap);
                                        }else if((filetype=="rar")||(filetype=="zip")||(filetype=="arj")||(filetype=="z")){
                                            QPixmap pixmap(":/new/mainwindow/pictures/zip_icon.png");
                                            pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                                            v1->fileIcon->setScaledContents(true);
                                            v1->fileIcon->setPixmap(pixmap);
                                        }else if((filetype=="mp3")||(filetype=="wma")||(filetype=="wav")||(filetype=="ape")||(filetype=="flac")||(filetype=="ogg")||(filetype=="aac")){
                                            QPixmap pixmap(":/new/mainwindow/pictures/music_icon.png");
                                            pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                                            v1->fileIcon->setScaledContents(true);
                                            v1->fileIcon->setPixmap(pixmap);
                                        }else{
                                            QPixmap pixmap(":/new/mainwindow/pictures/else_icon.png");
                                            pixmap.scaled(v1->fileIcon->size(),Qt::KeepAspectRatio);
                                            v1->fileIcon->setScaledContents(true);
                                            v1->fileIcon->setPixmap(pixmap);
                                            v1->elseLabel->setText(filetype.left(3));
                                            v1->elseLabel->raise();
                                        }
                                        QString initID = query1.record().value("id").toString();
                                        v1->checkBox->setObjectName(initID+"Decheck");//设置checkbox的ID
                                        v1->setObjectName(initID+"decryption");//设置Item的ID
                                        v1->downloadBtn->setObjectName(initID+"btn");//设置downloadBtn的ID
                                        v1->downloadBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: #3A8CFF;color:white;}QPushButton:pressed{background-color: rgb(139,159,185);}");
                                        //连接信号槽
                   //                   connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                                        if(query1.record().value("status").toString()=="0"){//待下载状态
                                            RequestIDArray[RequsetIndex]=initID;
                                            RequsetIndex++;
                                            v1->fileDescription->setText("主体文件指定分享需确认下载.");
                                            v1->downloadBtn->setText("确认下载");
                                            connect(v1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(OssDownLoadFile()));
                                            decryptionViewController->vbox->addWidget(v1);//将v1添加到视图中
                    //                      connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                                            f_progressBar = new QProgressBar();
                                            f_progressBar = v1->progressBar;
                                            f_progressBar->setObjectName(v1->objectName());
                   //                       v1->fileIcon->stackUnder(f_progressBar);
                   //                       v1->timeLabel->stackUnder(f_progressBar);
                                            f_progressBar->raise();
                                            f_progressBar->hide();
                                            f_progressBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);  // 对齐方式
                                            //decryptionViewController->vbox->addWidget(f_progressBar);
                                        }
                                        else if(query1.record().value("status").toString()=="1"){//待申请状态
                                            v1->fileDescription->setText("文件已加密需下载密钥文件.");
                                            v1->downloadBtn->setText("申请解密");
                                            v1->label->show();
                   //                       connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                                            connect(v1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(getFileID()));
                                            decryptionViewController->vbox->addWidget(v1);//将v1添加到视图中
                                        }
                                        else if(query1.record().value("status").toString()=="2"){//申请等待状态
                                            v1->fileDescription->setText("正在申请解密，请等待！");
                                            v1->downloadBtn->setText("申请中");
                                            v1->label->show();
                   //                       connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                                            decryptionViewController->vbox->addWidget(v1);
                                        }
                                        else if(query1.record().value("status").toString()=="3"){//申请请求已同意
                                            v1->fileDescription->setText("正在申请解密，请等待！");
                                            v1->downloadBtn->setText("申请中");
                                            v1->label->show();
                                            decryptionViewController->vbox->addWidget(v1);
                                        }else if(query1.record().value("status").toString()=="11"){//安卓端本地解密
                                            v1->fileDescription->setText("文件已加密需下载密钥文件.");
                                            v1->downloadBtn->setText("申请解密");
                                            v1->fileSize->setText("文件来自安卓系统");
                                            v1->downloadBtn->setObjectName(query1.record().value("file_id").toString());
                                            v1->label->show();
                   //                       connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                                            connect(v1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(androidDecrypt()));
                                            decryptionViewController->vbox->addWidget(v1);//将v1添加到视图中
                                            qDebug()<<"android!!!!!!!!!";
                                        }else if(query1.record().value("status").toString()=="21"){//ios端本地解密
                                            v1->fileDescription->setText("文件已加密需下载密钥文件.");
                                            v1->downloadBtn->setText("申请解密");
                                            v1->fileSize->setText("文件来自iOS系统");
                                            v1->downloadBtn->setObjectName(query1.record().value("file_id").toString());
                                            v1->label->show();
                   //                       connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                                            connect(v1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(getFileID()));
                                            decryptionViewController->vbox->addWidget(v1);//将v1添加到视图中
                                        }else if(query1.record().value("status").toString()=="12"){//安卓端本地解密第二步
                                            v1->fileDescription->setText("已申请解密！");
                                            v1->downloadBtn->setText("解密");
                                            v1->fileSize->setText("文件来自安卓系统");
                                            v1->downloadBtn->setObjectName(query1.record().value("file_id").toString());
                                            v1->label->show();
                                            connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                                            decryptionViewController->vbox->addWidget(v1);
                                        }else if(query1.record().value("status").toString()=="22"){//ios端本地解密第二步
                                            v1->fileDescription->setText("已申请解密！");
                                            v1->downloadBtn->setText("解密");
                                            v1->fileSize->setText("文件来自iOS系统");
                                            v1->downloadBtn->setObjectName(query1.record().value("file_id").toString());
                                            v1->label->show();
                                            connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                                            decryptionViewController->vbox->addWidget(v1);
                                        }
                                    }
                                    decryptionViewController->vbox->setMargin(0);
                                    decryptionViewController->vbox->setSpacing(0);
                                    QWidget *newItemWidget = new QWidget();
                                    newScrollArea = new QScrollArea();
                   //               newScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                                    newScrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
                                                                                    "{"
                                                                                    "width:8px;"
                                                                                    "background:rgba(0,0,0,0%);"
                                                                                    "margin:0px,0px,0px,0px;"
                                                                                    "padding-top:9px;"
                                                                                    "padding-bottom:9px;"
                                                                                    "}"
                                                                                    "QScrollBar::handle:vertical"
                                                                                    "{"
                                                                                    "width:8px;"
                                                                                    "background:rgba(0,0,0,25%);"
                                                                                    " border-radius:4px;"
                                                                                    "min-height:20;"
                                                                                    "}"
                                                                                    "QScrollBar::handle:vertical:hover"
                                                                                    "{"
                                                                                    "width:8px;"
                                                                                    "background:rgba(0,0,0,50%);"
                                                                                    " border-radius:4px;"
                                                                                    "min-height:20;"
                                                                                    "}"
                                                                                    "QScrollBar::add-line:vertical"
                                                                                    "{"
                                                                                    "height:5px;width:10px;"
                                                                                    "border-image:url(:/new/mainwindow/pictures/Scrollbar_bottom.png);"
                                                                                    "subcontrol-position:bottom;"
                                                                                    "}"
                                                                                    "QScrollBar::sub-line:vertical"
                                                                                    "{"
                                                                                    "height:5px;width:10px;"
                                                                                    "border-image:url(:/new/mainwindow/pictures/Scrollbar_top.png);"
                                                                                    "subcontrol-position:top;"
                                                                                    "}"
                                                                                    "QScrollBar::add-line:vertical:hover"
                                                                                    "{"
                                                                                    "height:9px;width:8px;"
                                                                                    "border-image:url(:/images/a/4.png);"
                                                                                    "subcontrol-position:bottom;"
                                                                                    "}"
                                                                                    "QScrollBar::sub-line:vertical:hover"
                                                                                    "{"
                                                                                    "height:9px;width:8px;"
                                                                                    "border-image:url(:/images/a/2.png);"
                                                                                    "subcontrol-position:top;"
                                                                                    "}"
                                                                                    "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
                                                                                    "{"
                                                                                    "background:rgba(0,0,0,5%);"
                                                                                    "border-radius:4px;"
                                                                                    "}");
                                    newItemWidget->setLayout(decryptionViewController->vbox);
                                    newScrollArea->setWidget(newItemWidget);
                                    newScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
                                    QVBoxLayout *newVbox = new QVBoxLayout();
                                    newVbox->setMargin(0);
                                    newVbox->setSpacing(0);
                                    newVbox->addWidget(newScrollArea);
                                    decryptionViewController->setLayout(newVbox);
                                    }
                            }
                        }else{
                            qDebug()<<"文件拷贝失败";
                        }
                    }
                    if(!trueSuccess){
                        MsgBox *msgbox = new MsgBox(3,QStringLiteral("所选文件是密文，但文件信息已被删除！"),this);
                        msgbox->exec();
                    }else{
                        MsgBox *msgbox = new MsgBox(3,QStringLiteral("所选文件是密文！"),this);
                        msgbox->exec();
                    }
                }
            }
        }
        /*
         * 解密需要发的https请求
        QNetworkRequest request;
        request.setUrl(QUrl("http://www.yunjiami1.com/cloud/File/DownLoadOSSFile.do"));
        QByteArray postData;
        postData.append("user_identify=");//请求参数名
        postData.append(__userId);
        postData.append("file_id=");//请求参数名
        postData.append(_fileId);
        QNetworkReply *reply = d_accessManager->post(request,postData);
        */
        else{
            MsgBox *msgbox = new MsgBox(3,QStringLiteral("所选文件并非密文！"),this);
            msgbox->exec();
        }
    }else // 用户取消选择
    {

    }
}
//拷贝文件：
bool MainWindow::copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist)
{
    toDir.replace("\\","/");
    if (sourceDir == toDir){
        return true;
    }
    if (!QFile::exists(sourceDir)){
        return false;
    }
    QDir *createfile     = new QDir;
    bool exist = createfile->exists(toDir);
    if (exist){
        if(coverFileIfExist){
            createfile->remove(toDir);
        }
    }//end if

    if(!QFile::copy(sourceDir, toDir))
    {
        return false;
    }
    qDebug()<<"copy成功";
    return true;
}
void MainWindow::androidDecrypt(){
    QPushButton *pt = qobject_cast<QPushButton *>(sender());
    QString key_id = pt->objectName();
    QSqlQuery query(db);
    bool success = query.exec("select * from Decryption where file_id='"+key_id.toUtf8()+"'");
    if(!success){
        qDebug() << "查询失败";
        return;
     }else{
        qDebug()<<"查询成功";
        while(query.next()){
            dcrypt_contentName = query.record().value("emp_phone").toString();
        }
    }
    QNetworkRequest request;
    request.setUrl(QUrl("http://www.yunjiami1.com/cloud/File/DownLoadOSSFile.do"));
    QByteArray postData;
    postData.append("file_id=");//请求参数名
    postData.append(key_id);
    dcrypt_ykeyId = key_id;
    postData.append("&user_identify=");//请求参数名
    postData.append(UserIdentify);
    qDebug()<<postData;
    httpFlag = 3;
    QNetworkReply *reply = d_accessManager->post(request,postData);
}
