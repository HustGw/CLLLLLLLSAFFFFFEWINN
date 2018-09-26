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
#include <qt_windows.h>
#include <QFile>
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
bool initLableFlag;
bool initPageFlag=true;
int decryptionFlag =0;
int threadNum = 0;
int DepThreadNum = 0;
int encptThreadNum = 0;
int enitemNum = 0;
QFont f("HiraginoSansGB",10,75);
QFileInfo openFileInfo;
QString orfileUuid;
QString yzipfileUuid;
QString file_id_list; //批量分享时用的文件表
QString DecProFileID = nullptr;
QString file_item_name;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //初始化用户User_ID
    User_ID = LoginUserID;
    ui->setupUi(this);
    setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    ui->BtnStaWidget->setCurrentIndex(0);//BtnStaWidget跳转到加密界面
    ui->pushButton_3->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_4->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_5->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_6->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_7->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_8->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_9->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_10->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_11->setCursor(QCursor(Qt::PointingHandCursor));
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
    // 连接进度条信号槽
    //connect(ui->OpenFileBtn, SIGNAL(clicked(bool)), encptThreadArr[encptThreadNum], SLOT(startProgressBarThread()));
    //connect(this, SIGNAL(starEncptItem(QString)), this, SLOT(startProgressBarThread(QString)));
    //开始加密信号槽
    //connect(ui->OpenFileBtn,SIGNAL(clicked(bool)),this,SLOT(starEncryptThread()));
    connect(this,SIGNAL(showDownDialog(QString)),this,SLOT(ShowNewDownDialog(QString)));
    finScrollArea = new QScrollArea();
    ui->MidStaWidget->addWidget(encryptionPage);
    ui->EncryptionBtn->clicked();
    initPageFlag=true;
    friendListLab = new Mylabel(ui->RightWidget);
    friendListLab->setText(tr("好友列表"));
    friendListLab->setGeometry(ui->RightWidget->width()/2-55,5,80,30);
    friendIcon = new QLabel(ui->RightWidget);
    friendIcon->setGeometry(ui->RightWidget->width()/2-85,14,10,10);
    friendIcon->setStyleSheet("border-image: url(:/new/login/pictures/login_accounts_management.png);");
    connect(friendListLab,SIGNAL(LabelClicked()),this,SLOT(FriendListWidgetHide()));
    friendListWidget = new QListWidget(ui->RightWidget);
    addFriendBtn = new QPushButton(ui->RightWidget);
    addFriendBtn->setGeometry(22,505,155,26);//设置添加好友BUTTON位置
    friendListWidget->setGeometry(20,40,ui->RightWidget->width()-19,ui->RightWidget->height()-44);
    friendListWidget->setIconSize(QSize(50,20));//设置Item图标大小
    friendListWidget->setStyleSheet("border:0;padding:0;spacing:0;");
    addFriendBtn->setCursor(QCursor(Qt::PointingHandCursor));
    addFriendBtn->setStyleSheet("border-image: url(://pictures/AddFriend_icon.png);");
    connect(addFriendBtn,SIGNAL(clicked(bool)),this,SLOT(showAddfriendWidget()));//信号槽连接
    ui->MidStaWidget->addWidget(encryptionViewController);
    ui->MidStaWidget->addWidget(decryptionViewController);
    ui->MidStaWidget->addWidget(finishViewController);
    ui->MidStaWidget->addWidget(finishViewController2);
    ui->SearchEdit->setPlaceholderText(tr("好友昵称/手机号"));
    QSize editSize(190,20);
    QSize btsize(30,20);
    ui->SearchEdit->setFixedSize(editSize);
    QPushButton *m_bt = new QPushButton();
    Mylabel *bt_icon = new Mylabel(this);
    bt_icon->setGeometry(1047,100,10,11);
    bt_icon->setStyleSheet("border-image: url(:/new/mainwindow/pictures/search.png);background-color:#398CFF;");
    bt_icon->setCursor(QCursor(Qt::PointingHandCursor));
    connect(bt_icon,SIGNAL(LabelClicked()),m_bt,SLOT(click()));
    m_bt->setFixedSize(btsize);
    m_bt->setStyleSheet("background-color:#398CFF;");
    QHBoxLayout *m_layout=new QHBoxLayout();
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    m_layout->addStretch();
    m_layout->addWidget(m_bt);
    ui->SearchEdit->setLayout(m_layout);
    ui->SearchEdit->setTextMargins(0,0,m_bt->width()-1,0);
    m_bt->setCursor(QCursor(Qt::PointingHandCursor));
    ui->pushButton_13->hide();
    connect(m_bt,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_13_clicked()));
    //ui->SearchEdit->setStyleSheet("border-radius:10px;");
    //查询数据库 获取用户名称
    setEmp_name();
    //使用Mylabel添加头像
    userHead = new Mylabel(ui->TopWidget);
    userHead->setGeometry(15,14,45,45);
    userHead->setStyleSheet("min-width:  45px;"
                            "max-width:  45px;"
                            "min-height: 45px;"
                            "max-height: 45px;"

                            "border-radius:22px;"
                            "border-width: 0 0 0 0;"
                            "border-image: url(:/new/src/head1) 0 0 0 0 stretch strectch;"
                            );
//    QPixmap pixmap(":/new/src/head1");
//    pixmap.scaled(userHead->size(),Qt::KeepAspectRatio);
//    userHead->setScaledContents(true);
//    userHead->setPixmap(pixmap);
    //连接头像信号槽
    //connect(userHead,SIGNAL(LabelClicked()),this,SLOT(HeadClickedSlot()));
    ui->FinDepBtn->hide();
    ui->FinEnpBtn->hide();
    ui->line_10->hide();
    ui->line_11->hide();
    ui->pushButton_12->hide();
    this->setFixedSize(this->width(),this->height());
    QFont font("Microsoft YaHei",10,75);
    this->setFont(font);
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
                   int fontSize = fontMetrics().width( fName );//获取之前设置的字符串的像素大小
                   if( fontSize >= v1->fileName->width()-50 ) //与label自身相比较
                   {
                       QString str = fontMetrics().elidedText( fName, Qt::ElideRight, v1->fileName->width()-50 );//返回一个带有省略号的字符串
                       v1->fileName->setText( str );       //重新设置label上的字符串
                   }else{
                       v1->fileName->setText(fName);
                   }

                   QString m_filesize = query.record().value("file_size").toString();
                   v1->fileSize->setText(m_filesize+"KB");//设置文件大小
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
                   v1->downloadBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: rgb(119,146,183);}QPushButton:pressed{background-color: rgb(139,159,185);}");
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

                   }
               }
               QWidget *newItemWidget = new QWidget();
               newScrollArea = new QScrollArea();
               newItemWidget->setLayout(decryptionViewController->vbox);
               newScrollArea->setWidget(newItemWidget);
               newScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
               QVBoxLayout *newVbox = new QVBoxLayout();
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
             add_item->setIcon(QIcon("://pictures/userIcon_1.png"));
             add_item->setText(Friend_nickname);
             add_item->setFont(QFont("Microsoft YaHei",10,50));
             add_item->setTextAlignment(Qt::AlignLeft|Qt::AlignLeft);
             add_item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
             add_item->setSizeHint(QSize(ui->RightWidget->width()-30,34));
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
                         "QPushButton#pushButton_11:hover { border-image: url(:/new/mainwindow/pictures/allselect_hover.png); }");
     ui->pushButton_8->hide();
     ui->pushButton_9->hide();

        thread_11 = new QThread(this);
        workThread = new heartThread(); //工作线程，具体的业务实现在此中完成，继承自Object
        workThread->moveToThread( thread_11 );       //加入到子线程
        thread_11->start();                        //启动线程

        threadTimer = new QTimer();
        QObject::connect( threadTimer, SIGNAL(timeout()), workThread, SLOT( work() ),Qt::AutoConnection );
        threadTimer->start(10000);   // 计时器，每隔几秒钟唤起工作线程；

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
    ui->FinishedBtn->setStyleSheet("border-image: url(:/new/mainwindow/pictures/mainwindow_button_bg_selected.png);color:#3A8CFF;");
    ui->FinishedBtn->setIcon(QIcon(":/new/mainwindow/pictures/fin_icon_selected.png"));

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
    ui->DecryptionBtn->setStyleSheet("border-image: url(:/new/mainwindow/pictures/mainwindow_button_bg_selected.png);color:#3A8CFF;");
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
    ui->EncryptionBtn->setStyleSheet("border-image: url(:/new/mainwindow/pictures/mainwindow_button_bg_selected.png);color:#3A8CFF;");
    ui->EncryptionBtn->setIcon(QIcon(":/new/mainwindow/pictures/encryption_icon_selected.png"));
    ui->MidStaWidget->setCurrentWidget(encryptionViewController);
    if (encryptionViewController->vbox->count()==0){
        //ui->BtnStaWidget->setCurrentWidget(encryptionPage);
        ui->BtnStaWidget->setCurrentIndex(0);
        //initPageFlag=true;
        ui->MidStaWidget->setCurrentWidget(encryptionPage);
        //ui->MidStaWidget->addWidget(encryptionPage);

    }else {
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
    ui->FinEnpBtn->setStyleSheet("border-image: url(:/new/mainwindow/pictures/mainwindow_button_bg_selected.png);color:#3A8CFF;");
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
    ui->FinDepBtn->setStyleSheet("border-image: url(:/new/mainwindow/pictures/mainwindow_button_bg_selected.png);color:#3A8CFF;");
    ui->MidStaWidget->setCurrentWidget(finishViewController2);
    ui->BtnStaWidget->setCurrentIndex(3);
    on_pushButton_9_clicked();
}

void MainWindow::on_OpenFileBtn_clicked()
{
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
        initLableFlag = false;

        if (initPageFlag){

            delete encryptionPage;
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

        v1->setObjectName(fName);
        int fontSize = fontMetrics().width( fName );//获取之前设置的字符串的像素大小
        if( fontSize >= v1->fileName->width() ) //与label自身相比较
        {
            QString str = fontMetrics().elidedText( fName, Qt::ElideRight, v1->fileName->width()-50 );//返回一个带有省略号的字符串
            v1->fileName->setText( str );       //重新设置label上的字符串
        }else{
            v1->fileName->setText(fName);
        }

        v1->fileSize->setText(amfSize+codec->toUnicode(" KB"));
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

        v1->encryptStaBtn->setObjectName(fName);
        v1->encryptStaBtn->show();
        v1->encryptStaBtn->setEnabled(false);
        v1->encryptStaBtn->setText("正在加密...");
        v1->encryptStaBtn->setFlat(true);
        v1->encryptStaBtn->setStyleSheet("background:transparent");
        v1->starEncptBtn->setFlat(true);
        //v1->checkBox->setObjectName(fName);

        //qDebug()<<v1->checkBox->objectName();

        v1->progressBar->setObjectName(fName);
        encryptionViewController->vbox->addWidget(v1);

        //ecptProgressbarItem *v2 = new ecptProgressbarItem();

        //encryptionViewController2->vbox->addWidget(v2);

        f_progressBar = new QProgressBar();
        f_progressBar = v1->progressBar;
        QString strQSS = "QProgressBar { \
                text-align: center; \
                border: 1px ; \
                background: transparent; \
            } \
            QProgressBar::chunk { \
                background-color: rgba(230, 277, 255,180); \
            }";
//        f_progressBar->setStyleSheet(strQSS);
        f_progressBar->setObjectName(fName);
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
        QScrollArea *newScrollArea = new QScrollArea();
        newItemWidget->setLayout(encryptionViewController->vbox);
        newScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
        newScrollArea->setWidget(newItemWidget);
        QVBoxLayout *newVbox = new QVBoxLayout();
        newVbox->addWidget(newScrollArea);
        encryptionViewController->setLayout(newVbox);

}
    }

}

// 更新进度条
void MainWindow::handleResults(int value,QString itemName)
{
    EncryptionItem *v1 = ui->MidStaWidget->findChild<EncryptionItem*>(itemName);
    //QProgressBar *f_progressBar = f_progressBar->findChild<QProgressBar *>(itemName);
    //f_progressBar->setValue(value);
    v1->progressBar->setValue(value);

    if (value==100){

        v1->encryptStaBtn->clicked();
        //QMessageBox::information(NULL,tr("成功"),tr("加密完成！"),QMessageBox::Yes,NULL);
        QString str = itemName + "加密已完成！";
        str.toStdString();
        MsgBox *msgbox = new MsgBox(4,QStringLiteral("加密已完成！"),this);
        msgbox->exec();
///////////////////////////////删除加密完成的项目
        //EncryptionItem *v1 = ui->MidStaWidget->findChild<EncryptionItem*>(f_progressBar->objectName());
        //qDebug()<<name;
        delete v1;
        //ReLayout();
        delete encryptionViewController->layout();
        QWidget *newItemWidget = new QWidget();
        QScrollArea *newScrollArea = new QScrollArea();
        newItemWidget->setLayout(encryptionViewController->vbox);
        newScrollArea->setWidget(newItemWidget);
        newScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
        QVBoxLayout *newVbox = new QVBoxLayout();
        newVbox->addWidget(newScrollArea);
        encryptionViewController->setLayout(newVbox);


///////////////////////////////////////////////////////////////////////
        //initPageFlag=true;
        if (encryptionViewController->vbox->count()==0){
//            ui->FinishedBtn->close();
            if (isFinishedBtn == 0)
                ui->FinishedBtn->clicked(true);
           ui->FinEnpBtn->clicked(true);
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
        QScrollArea *newScrollArea = new QScrollArea();
        newItemWidget->setLayout(encryptionViewController->vbox);
        newScrollArea->setWidget(newItemWidget);
        QVBoxLayout *newVbox = new QVBoxLayout();
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
}

void MainWindow::on_starEncptBtn_clicked(){
    QString fName = openFileInfo.fileName();
    startProgressBarThread(fName);
}

// 开启进度条
void MainWindow::startProgressBarThread( QString  itemName)
{
    if (fileOpenFlag){

        encryptthread *ecry = new encryptthread(this);
        file_item_name = itemName;
        //ecry->itemName = itemName;
        enItemThread *enit = new enItemThread(this);
        //enit->item = itemName;
        enitemArr[enitemNum] = new enItemThread(this);
        //enItemThread *workerThread = new enItemThread(this);
        connect(enitemArr[enitemNum], SIGNAL(resultReady(int,QString)), this, SLOT(handleResults(int,QString)));
        // 线程结束后，自动销毁
        connect(enitemArr[enitemNum], SIGNAL(finished()), enitemArr[enitemNum], SLOT(deleteLater()));
       enitemArr[enitemNum]->start();
       enitemNum++;
        //startEncryptThread();
        encptThreadArr[encptThreadNum] = new encryptthread(this);
        //encryptthread *ecpThread = new encryptthread(this);
        connect(encptThreadArr[encptThreadNum],SIGNAL(result(int,QString)),this,SLOT(handleResults(int,QString)));
        // 线程结束后，自动销毁
        connect(encptThreadArr[encptThreadNum], SIGNAL(finished()), encptThreadArr[encptThreadNum], SLOT(deleteLater()));
        //ecpThread->start();
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
                  QWidget *newItemWidget = new QWidget();
                  newItemWidget->setLayout(decryptionViewController->vbox);
                  newScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
                  newScrollArea->setWidget(newItemWidget);
                  QVBoxLayout *newVbox = new QVBoxLayout();
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
                    DecProFileID = onlyId;
                    decryBarThread[DecryBarNum] = new DecryptProBarThread(this);
//                    connect(this,SIGNAL(DecryProBarID(QString)),decryBarThread[DecryBarNum],SLOT(setItemID(QString)));
                    connect(decryBarThread[DecryBarNum],SIGNAL(reslut(int,QString)),this,SLOT(ChangeDecItemProBar(int,QString)));
                    connect(decryBarThread[DecryBarNum],SIGNAL(finished()),decryBarThread[DecryBarNum],SLOT(deleteLater()));
                    connect(downThread[threadNum],SIGNAL(finished()),downThread[threadNum],SLOT(deleteLater()));
                    connect(this,SIGNAL(OSSfileDownFileID(QString,QString)),downThread[threadNum],SLOT(DownContent(QString,QString)));
                    connect(downThread[threadNum],SIGNAL(ChangeBtnText(QString)),decryBarThread[DecryBarNum],SLOT(run()));
                    downThread[threadNum]->start();
//                     emit DecryProBarID(onlyId);
//                    decryBarThread[DecryBarNum]->start();
                    emit OSSfileDownFileID(onlyId,enkey_id);
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
                  int fontSize = fontMetrics().width( fName );//获取之前设置的字符串的像素大小
                  if( fontSize >= v1->fileName->width()-50 ) //与label自身相比较
                  {
                      QString str = fontMetrics().elidedText( fName, Qt::ElideRight, v1->fileName->width()-50 );//返回一个带有省略号的字符串
                      v1->fileName->setText( str );       //重新设置label上的字符串
                  }else{
                      v1->fileName->setText(fName);
                  }
                  QString m_filesize = query.record().value("file_size").toString();
                  v1->fileSize->setText(m_filesize+"KB");//设置文件大小
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
                  v1->downloadBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: rgb(119,146,183);}QPushButton:pressed{background-color: rgb(139,159,185);}");
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
                      f_progressBar->setObjectName(v1->objectName());
                      f_progressBar->hide();
                      f_progressBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);  // 对齐方式
                      //decryptionViewController->vbox->addWidget(f_progressBar);
                  }
                  else if(query.record().value("status").toString()=="1"){
                      v1->fileDescription->setText("文件已加密需下载密钥文件.");
                      v1->downloadBtn->setText("申请解密");
                      connect(v1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(getFileID()));
 //                     connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
                      decryptionViewController->vbox->addWidget(v1);//将v1添加到视图中
                  }
                  else if(query.record().value("status").toString()=="2"){//申请等待状态
                      v1->fileDescription->setText("正在申请解密，请等待！");
                      v1->downloadBtn->setText("申请中");
//                      connect(ui->pushButton,SIGNAL(clicked()),v1,SLOT(changeCheckBox()));
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
                MsgBox *msgbox = new MsgBox(2,QStringLiteral("已经是你的好友了"),this);
                msgbox->exec();
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
        add_item->setIcon(QIcon("://pictures/userIcon_1.png"));
        add_item->setText(name);
        add_item->setFont(QFont("Microsoft YaHei",10,50));
        add_item->setTextAlignment(Qt::AlignLeft|Qt::AlignLeft);
        add_item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        add_item->setSizeHint(QSize(ui->RightWidget->width()-30,34));
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
               if( fontSize >= f1->fileName->width()-50 ) //与label自身相比较
               {
                   QString str = fontMetrics().elidedText( file_name, Qt::ElideRight, f1->fileName->width()-50 );//返回一个带有省略号的字符串
                   f1->fileName->setText( str );       //重新设置label上的字符串
               }else{
                   f1->fileName->setText(file_name);
               }

               //f1->fileName->setText(file_name);
               f1->fileSize->setText(file_size);
               f1->fileDescription->setText(file_discryption);

               finishViewController->vbox->addWidget(f1);
               delete finishViewController->layout();
               QWidget *newItemWidget = new QWidget();
               //QScrollArea *newScrollArea = new QScrollArea();
               newItemWidget->setLayout(finishViewController->vbox);
               finScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
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
        MsgBox *msgbox = new MsgBox(1,QStringLiteral("确认删除这些已加密条目吗？"),this);
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
        }else if(flag == 0){
            MsgBox *msgbox = new MsgBox(3,QStringLiteral("请选择需要批量传输的条目！"),this);
            msgbox->exec();
        }
        flag = 0;
}

//已加密文件单独删除按钮
void MainWindow::on_deleteBtn_clicked(){
    MsgBox *msgbox = new MsgBox(1,QStringLiteral("确认删除这一已加密条目吗？"),this);
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
        QWidget *newItemWidget = new QWidget();
        newItemWidget->setLayout(finishViewController->vbox);
        finScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
        finScrollArea->setWidget(newItemWidget);
        QVBoxLayout *newVbox = new QVBoxLayout();
        newVbox->addWidget(finScrollArea);
        finishViewController->setLayout(newVbox);
        }
    else if (nRes == QDialog::Rejected){
    }
}
//已解密文件单独删除按钮
void MainWindow::on_deleteBtn2_clicked(){
    MsgBox *msgbox = new MsgBox(5,QStringLiteral("确认删除这一已解密条目吗？"),this);
    int nRes = msgbox->exec();
    if (nRes == QDialog::Accepted){
            QPushButton* button = qobject_cast<QPushButton*>(sender());
            QString name = button->objectName();

            FinishDecryptionItem *f2 = ui->MidStaWidget->findChild<FinishDecryptionItem*>(name);
            delete f2;
            qDebug()<<name;

            QSqlQuery query(db);
            bool success = query.exec("delete from Decryption where file_id = '"+name+"'");
            if(success){
                MsgBox *msgbox = new MsgBox(4,QStringLiteral("成功删除已解密条目！"),this);
                msgbox->exec();
            }
            delete finishViewController2->layout();
            QWidget *newItemWidget = new QWidget();
           // QScrollArea *newScrollArea = new QScrollArea();
            newItemWidget->setLayout(finishViewController2->vbox);
            finScrollArea->setWidget(newItemWidget);
            finScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
            QVBoxLayout *newVbox = new QVBoxLayout();
            newVbox->addWidget(finScrollArea);
            finishViewController2->setLayout(newVbox);
        }
        else if (nRes == QDialog::Rejected){
    }else if(nRes == 5){
        QPushButton* button = qobject_cast<QPushButton*>(sender());
        QString name = button->objectName();

        FinishDecryptionItem *f2 = ui->MidStaWidget->findChild<FinishDecryptionItem*>(name);
        delete f2;
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
        QWidget *newItemWidget = new QWidget();
       // QScrollArea *newScrollArea = new QScrollArea();
        newItemWidget->setLayout(finishViewController2->vbox);
        finScrollArea->setWidget(newItemWidget);
        finScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
        QVBoxLayout *newVbox = new QVBoxLayout();
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
    finishViewController2->vbox = new QVBoxLayout();
    QSqlQuery query(db);
       bool success = query.exec("select * from Decryption where status = 5 and oemp_id ='" + User_ID+"' order by apply_time DESC");
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
               if( fontSize >= f1->fileName->width()-50 ) //与label自身相比较
               {
                   QString str = fontMetrics().elidedText( file_name, Qt::ElideRight, f1->fileName->width()-50 );//返回一个带有省略号的字符串
                   f1->fileName->setText( str );       //重新设置label上的字符串
               }else{
                   f1->fileName->setText(file_name);
               }

               //f1->fileName->setText(file_name);
               f1->fileSize->setText(file_size);
               f1->fileDescription->setText(file_discryption);

               finishViewController2->vbox->addWidget(f1);

               delete finishViewController2->layout();
               QWidget *newItemWidget = new QWidget();
              // QScrollArea *newScrollArea = new QScrollArea();
               newItemWidget->setLayout(finishViewController2->vbox);
               finScrollArea->setWidget(newItemWidget);
               finScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
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
    //创建密文文件夹
//    QDir cdir;
//    cdir.cd("C://CloundSafe");  //进入某文件夹
//    if(!cdir.exists("C://CloundSafe"))//判断需要创建的文件夹是否存在
//    {
//        cdir.mkdir("C://CloundSafe"); //创建文件夹
//    }
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
                     newScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
                     QVBoxLayout *newVbox = new QVBoxLayout();
                     newVbox->addWidget(newScrollArea);
                     decryptionViewController->setLayout(newVbox);
                     MsgBox *msgbox = new MsgBox(4,QStringLiteral("文件解密同意并成功解密"),this);
                     msgbox->exec();
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
    m1->fileDescription->setText("文件已加密需下载密钥文件");
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
//设置主界面用户名和用户个人信息
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
                flag = 1;
            }
        }
     }
    if(flag == 1){
        MsgBox *msgbox = new MsgBox(5,QStringLiteral("确认删除这些已解密条目吗？"),this);
        int nRes = msgbox->exec();
        if (nRes == QDialog::Accepted){
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
            MsgBox *msgbox = new MsgBox(4,QStringLiteral("成功批量删除条目！"),this);
            msgbox->exec();
        }else if(nRes == 5){
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
        MsgBox *msgbox = new MsgBox(2,QStringLiteral("请选择需要删除的条目！"),this);
        msgbox->exec();
    }
    flag = 0;
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
    ui->FinDepBtn->setFont(f);
    ui->DecryptionBtn->setFont(f);
    ui->EncryptionBtn->setFont(f);
    ui->FinishedBtn->setFont(f);
    ui->FinEnpBtn->setFont(f);
    ui->FinDepBtn->setStyleSheet("border-image: url(:/new/mainwindow/pictures/mainwindow_button_bg.png);");
    ui->DecryptionBtn->setStyleSheet("border-image: url(:/new/mainwindow/pictures/mainwindow_button_bg.png);");
    ui->DecryptionBtn->setIcon(QIcon(":/new/mainwindow/pictures/decryption_icon.png"));
    ui->EncryptionBtn->setStyleSheet("border-image: url(:/new/mainwindow/pictures/mainwindow_button_bg.png);");
    ui->EncryptionBtn->setIcon(QIcon(":/new/mainwindow/pictures/encryption_icon.png"));
    ui->FinEnpBtn->setStyleSheet("border-image: url(:/new/mainwindow/pictures/mainwindow_button_bg.png);");
    ui->FinishedBtn->setStyleSheet("border-image: url(:/new/mainwindow/pictures/mainwindow_button_bg.png);");
    ui->FinishedBtn->setIcon(QIcon(":/new/mainwindow/pictures/fin_icon.png"));
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
    if(Link_empid == nullptr){
        MsgBox *msgbox = new MsgBox(6,QStringLiteral("无效链接！"),this);
        msgbox->exec();
        return;
    }
    QUuid strid = QUuid::createUuid();
    QString id = strid.toString();
    QSqlQuery insertQuery(db);
    bool insertSuccess = insertQuery.exec("insert into Decryption values('"+id+"','"+GetLink+"','"+Link_filename+"','"+Link_empid+"','','"+User_ID+"','',0,'"+time_str+"','"+Link_filesize+"',0,0)");
    if(!insertSuccess){
        qDebug()<<"LinkInsertFailed";
    }
    else{
        DecryptionItem *a1 = new DecryptionItem();

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
        if( fontSize >= a1->fileName->width()-50 ) //与label自身相比较
        {
            QString str = fontMetrics().elidedText( Link_filename, Qt::ElideRight, a1->fileName->width()-50 );//返回一个带有省略号的字符串
            a1->fileName->setText( str );       //重新设置label上的字符串
        }else{
            a1->fileName->setText(Link_filename);
        }
        a1->fileSize->setText(Link_filesize+"KB");
        a1->checkBox->setObjectName(id+"Decheck");
        a1->setObjectName(id+"decryption");
        a1->downloadBtn->setObjectName(id+"btn");
        a1->downloadBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: rgb(119,146,183);}QPushButton:pressed{background-color: rgb(139,159,185);}");
//        connect(ui->pushButton,SIGNAL(clicked(bool)),a1,SLOT(changeCheckBox()));
        a1->fileDescription->setText("主体文件指定分享需确认下载.");
        a1->downloadBtn->setText("确认下载");
        connect(a1->downloadBtn,SIGNAL(clicked(bool)),this,SLOT(OssDownLoadFile()));
        decryptionViewController->vbox->addWidget(a1);
        f_progressBar = new QProgressBar(this);
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
    QWidget *newItemWidget = new QWidget();
    newItemWidget->setLayout(decryptionViewController->vbox);
    newScrollArea->setWidget(newItemWidget);
    newScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
    QVBoxLayout *newVbox = new QVBoxLayout();
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
    bool success = query.exec("select * from Decryption where emp_id='"+User_ID+"' and status =2 and is_solved = '0'");
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
    QFont font("Microsoft YaHei",8,75);
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
    QListWidgetItem *finditem = new QListWidgetItem();
    QList <QListWidgetItem *> item = friendListWidget->findItems(searchcontent,Qt::MatchContains);
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

void MainWindow::ShowNewDownDialog(QString id){
    newDownloadFileIdList.append(id);
    if(newdownloadDlg->isOpened){
        emit newDownload();
    }else{
        newdownloadDlg->show();
        newdownloadDlg->isOpened = true;
        emit newDownload();
    }


}
void MainWindow::downloadOneFile(QString id){
            //跳转到解密页面，开始下载
            DecryptionItem *m1 = ui->MidStaWidget->findChild<DecryptionItem *>(id+"decryption");
            if(m1==nullptr){
                qDebug()<<"error";
            }
            else{
                 m1->downloadBtn->click();
                Infor_requestNum--;
                InforNum_Changed();
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
        MsgBox *msgbox = new MsgBox(1,QStringLiteral("确认关闭系统吗？"),this);
        int reply = msgbox->exec();
        if(reply == QDialog::Accepted){
            QString downPath = "C:/CloundSafe/"+User_qqNum+"/Decrypt/ykey/";
            DeleteFileOrFolder(downPath);
            QSqlQuery query(db);
            query.exec("update UserStatus set status = 0 where emp_phone = '" + UserPhoneNum + "'");
            thread_11->exit(0);
            workThread->exit(0);
            event->accept();
        }else{
            event->ignore();
        }
    }else{
        thread_11->exit(0);
        workThread->exit(0);
        event->accept();
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
}
void MainWindow::mouseMoveEvent(QMouseEvent *event){
    if (event->buttons() & Qt::LeftButton) {
        if(mouse_press == true){
            move(event->globalPos() - dragPosition);
            event->accept();
        }
        }

}
void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    mouse_press = false;
}
void MainWindow::paintEvent(QPaintEvent *event)
{

}
void MainWindow::FriendListWidgetHide(){
    if(isFriendListHide == 0){
      friendListWidget->hide();
      friendIcon->setStyleSheet("border-image: url(://pictures/login_accounts_management_hide.png);");
      isFriendListHide = 1;
    }
    else{
      friendListWidget->show();
      friendIcon->setStyleSheet("border-image: url(:/new/login/pictures/login_accounts_management.png);");
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
            add_item->setIcon(QIcon("://pictures/userIcon_1.png"));
            add_item->setText(Friend_nickname);
            add_item->setFont(QFont("Microsoft YaHei",10,50));
            add_item->setTextAlignment(Qt::AlignLeft|Qt::AlignLeft);
            add_item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            add_item->setSizeHint(QSize(ui->RightWidget->width()-30,34));
            friendListWidget->insertItem(count,add_item);
            count++;
        }
    }
}
void MainWindow::forceShut(){
    MsgBox *msgbox = new MsgBox(2,QStringLiteral("您的账号在其他地方登陆，您已被迫下线，如非本人操作，请修改密码"),this);
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
        QWidget *newItemWidget = new QWidget();
        newItemWidget->setLayout(decryptionViewController->vbox);
        newScrollArea->setWidget(newItemWidget);
        newScrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
        QVBoxLayout *newVbox = new QVBoxLayout();
        newVbox->addWidget(newScrollArea);
        decryptionViewController->setLayout(newVbox);
    }

}

//批量分享按钮
void MainWindow::on_pushButton_groupshare_clicked()
{
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
    }else if(flag == 0){
        MsgBox *msgbox = new MsgBox(3,QStringLiteral("请选择需要批量分享的条目！"),this);
        msgbox->exec();
    }
    flag = 0;
}

void MainWindow::ChangeDecItemProBar(int value, QString itemID){
    DecryptionItem *m1 = ui->MidStaWidget->findChild<DecryptionItem *>(itemID+"decryption");
    QProgressBar *n1 = ui->MidStaWidget->findChild<QProgressBar *>(itemID+"decryption");
    m1->progressBar->setValue(value);
    if(value ==100){
        m1->fileDescription->setText("文件已加密需下载密钥文件");
        m1->downloadBtn->setText("申请解密");
        delete n1;
        ReLayout();
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
