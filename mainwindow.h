#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define MAXSIZE 10      //定义最多并发进行线程为10
#include <QMainWindow>
#include <QPushButton>
#include "decryptionitem.h"
#include <QStackedWidget>
#include "encryptionitem.h"
#include <QListWidget>
#include <QVBoxLayout>
#include "encryptionbtnview.h"
#include "encryptionviewcontroller.h"
#include "decryptionbtnview.h"
#include "decryptionviewcontroller.h"
#include "decryptionviewcontroller.h"
#include "encryption.h"
#include "finishencryptionItem.h"
#include "finishviewcontroller.h"
#include "finishdecryptionItem.h"
#include "finishviewcontroller2.h"
#include <QLabel>
#include <QImage>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QTextStream>
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QtSql/QSqlError>
#include <QUuid>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QString>
#include <QScrollArea>
#include <QMessageBox>
#include <QDir>
#include "requestrecthread.h"
#include "connectionpool.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QFont>
#include <QListWidget>
#include "friendinputdlg.h"
#include "mylabel.h"
#include "informationdlg.h"
#include "informationthread.h"
#include "downloadoss.h"//测试下载
#include "decryptionfile.h"
#include "depdownthread.h"
#include "QProgressBar"
#include "enitemthread.h"
#include "encryptthread.h"
#include <QFile>
#include "decryptionthread.h"
#include <QPalette>
#include "tcpclient.h"
#include "delinkdialog.h"
#include <QDateTime>
#include  "QZXing/QZXing.h"
#include "groupsenddialog.h"
#include "msgbox.h"
#include<QPainter>
#include<QtMath>
#include <enitemthread.h>
#include <encryptthread.h>
#include "groupsharedialog.h"
#include "newdownloaddialog.h"
#include "decryptprobarthread.h"
#include <QNetworkAccessManager>
#include "heartthread.h"
#include <QNetworkInterface>
#include <QList>
namespace Ui {
class MainWindow;
}
extern QStringList newDownloadFileIdList;
extern int AddFriendFlag;
extern int LinkInsertFlag;
extern int findecrypt_flag;//已解密全选判断变量
extern int finencrypt_flag;//已加密全选判断变量
extern int DeSelect_flag;//解密全选判断变量
extern QString User_ID;
extern QString URL;
extern bool fileOpenFlag;
extern bool initLableFlag;
extern bool initPageFlag;
extern int threadNum;
extern int DepThreadNum;
extern int encptThreadNum;
extern int enitemNum;
extern int isFinishedBtn;
extern QFont f;
extern int informationNum;
extern int RequsetAllowNum;
extern int RequestNum;
extern int decryptionFlag;
extern int Infor_requestNum;
extern QString UserPhoneNum;
extern QString RequestIDArray[50];//存储消息ID数组
extern QString FriendNickNameArray[50];//存放好友昵称数组
extern int FriendCount;
extern int FriendRequestCount;
extern int FriendArrayIndex;//好友昵称数据目录
extern int RequsetIndex;//消息ID数组的目录
extern int isFriendListHide;//判断消息列表是否隐藏
extern QString User_qqNum;//用户qq_num
extern QString User_qqPath;//用户本地存放文件路径
extern QFileInfo openFileInfo;
extern QString orfileUuid;
extern QString yzipfileUuid;
extern QString file_item_name;
extern QString DecProFileID;
extern QNetworkAccessManager *m_accessManagerHeart;
class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void fileData(QString);
    void sendUserId(QString);
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    DecryptionItem *decryptionPage;
    EncryptionItem *encryptionPage;
    EncryptionViewController *encryptionViewController;
    EncryptionBtnView *encryptionBtnItem; //加密单元
    DecryptionBtnView *decryptionBtnItem; //解密单元
    DecryptionViewController *decryptionViewController; //解密界面控制器 对解密单元进行布局管理
    FinishEncryptionItem *finishEncryptionItem;
    FinishViewController *finishViewController;
    FinishViewController2 *finishViewController2;
    FinishDecryptionItem *finishDecryptionItem;
    QListWidget *list;
    Mylabel *userHead;
    Mylabel *Infor_num_icon;
    Mylabel *Infor_icon;
    QScrollArea *finScrollArea;
    QScrollArea *newScrollArea;
    QSqlDatabase db;
    QListWidget *friendListWidget;
    QPushButton *addFriendBtn;
    Mylabel *friendListLab;
    QLabel *friendIcon;
    informationDlg *inforDlg;
    DepDownThread *downThread[MAXSIZE];
    DecryptionThread *depThread[MAXSIZE];
    heartThread *heartTread_;
    QProgressBar *f_progressBar;
    QProgressBar *d_progressBar;
    encryption *contest = new encryption();
    DelinkDialog *linkDialog;
    groupSendDialog *grpDlg;
    groupshareDialog *grpShareDlg;
    encryptthread *encptThreadArr[MAXSIZE];
    enItemThread *enitemArr[MAXSIZE];
    newDownloadDialog *newdownloadDlg;
    DecryptProBarThread *decryBarThread[MAXSIZE];
    bool forceFlag = false;
public slots:

    void FileIsAllowed();//请求同意槽函数

private slots:

    void on_FinishedBtn_clicked();

    void on_DecryptionBtn_clicked();

    void on_EncryptionBtn_clicked();

    void on_FinEnpBtn_clicked();

    void on_FinDepBtn_clicked();

    void on_OpenFileBtn_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_5_clicked();

    void on_deleteBtn_clicked();

    void on_deleteBtn2_clicked();

    void on_pushButton_9_clicked();

    void on_encryptStaBtn_clicked();

    void on_starEncptBtn_clicked();

    void getFileID();

    void OssDownLoadFile();//解密下载OSS文件

    void ReceiveNewReq();//接收到新消息

    void showAddfriendWidget();//显示或隐藏好友列表

    void addFriendToDatabase(QString name);//添加好友槽函数

    void HeadClickedSlot();//头像点击函数

    void HeadChanged();

//    void FileIsAllowed();//请求同意槽函数

    void FileIsIgnored();//请求忽略槽函数

    void ChangeItemBtnText(QString fileID);

    // 更新进度条
    void handleResults(int value,QString itemName);
    // 开启进度条线程
    void startProgressBarThread(QString itemName);
    //开启加密进程
    void startEncryptThread(QString itemName);


    //设置主界面用户名称
    void setEmp_name();
    //清楚左侧Button点击效果
    void CleanButtonClicked();

    void NumDown();

    void on_pushButton_7_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void LinkInsert(QString link);

    void RecDecryptionFailed();

    void ReLayout();

    void DeProgressBarStart();

    void on_pushButton_12_clicked();

    void Init_InforIcon();

    void InforNum_Changed();//消息数量改变响应函数

    void inforDlgaddFriend(QString name);

    void on_pushButton_13_clicked();

    void ShowNewDownDialog(QString id);


    void on_pushButton_6_clicked();
    void on_pushButton_15_clicked();

    void on_pushButton_14_clicked();

    void FriendListWidgetHide();

    void internet_Disconnected();


    void starEcpt(QString);

    void RecvNewFriendReq();

    void NewFriendAgree();//添加好友，对方同意函数

    void on_pushButton_groupshare_clicked();
    bool DeleteFileOrFolder( const QString& strPath );
    void downloadOneFile(QString id);

    void ChangeDecItemProBar(int value, QString itemID);

    void newDownDialogInforInit();//newDownloadDiaglog点击按钮后消息数量置0并刷新

    void forceShut();
protected:
    void closeEvent(QCloseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
signals:
//    void sendUserID(QString user_id);
    void sendFileID(QString enkey_id,QString file_id,QString file_name);
    void OSSfileDownFileID(QString id,QString enkey_id);
    void SendInforToInforDlg(QString nickname,QString fileName,QString time);
    void showDownDialog(QString id);
    void starEcptItem(QString itemName);
    void newDownload();
    void DecryProBarID(QString id);
private:
    Ui::MainWindow *ui;
    QPoint dragPosition;
    bool mouse_press;
};

#endif // MAINWINDOW_H
