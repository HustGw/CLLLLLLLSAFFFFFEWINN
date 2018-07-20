#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include<QtSql/QSqlQuery>
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
namespace Ui {
class MainWindow;
}
extern QString User_ID;
extern QString URL;
class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void fileData(QString);
    void sendUserId(QString);
public:
    explicit MainWindow(QWidget *parent = 0);
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
    QScrollArea *finScrollArea;
    QScrollArea *newScrollArea;
    QSqlDatabase db;
    QListWidget *friendListWidget;
    QPushButton *addFriendBtn;
    QLabel *friendListLab;
    informationDlg *inforDlg;

private slots:

    void on_FinishedBtn_clicked();

    void on_DecryptionBtn_clicked();

    void on_MidStaWidget_currentChanged(int arg1);

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

    void getFileID();

    void OssDownLoadFile();

    void ReceiveNewReq();

    void showAddfriendWidget();

    void addFriendToDatabase(QString name);

    void HeadClickedSlot();

    void HeadChanged();
signals:
    void sendUserID(QString user_id);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
