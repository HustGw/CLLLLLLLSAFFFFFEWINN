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
#include "decryptionbtnview.h"
#include "decryptionviewcontroller.h"
#include "encryptionviewcontroller.h"
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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    DecryptionItem *decryptionPage;
    EncryptionItem *encryptionPage;
    EncryptionBtnView *encryptionBtnItem; //加密单元
    DecryptionBtnView *decryptionBtnItem; //解密单元
    DecryptionViewController *decryptionViewController; //解密界面控制器 对解密单元进行布局管理
    EncryptionViewController *encryptionViewController;
    QListWidget *list;
    QLabel *userHead;
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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
