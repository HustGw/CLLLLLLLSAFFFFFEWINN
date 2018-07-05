#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "decryptionitem.h"
#include <QStackedWidget>
#include "encryptionitem.h"
#include <QListWidget>
#include "encryptionbtnview.h"
#include "decryptionbtnview.h"
#include "decryptionviewcontroller.h"
#include "decryptionviewcontroller.h"
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
    EncryptionBtnView *encryptionBtnItem;
    DecryptionBtnView *decryptionBtnItem;
    DecryptionViewController *decryptionViewController;
    QListWidget *list;
private slots:

    void on_FinishedBtn_clicked();

    void on_DecryptionBtn_clicked();

    void on_MidStaWidget_currentChanged(int arg1);
    
    void on_EncryptionBtn_clicked();

    void on_FinEnpBtn_clicked();

    void on_FinDepBtn_clicked();

    void on_OpenFileBtn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
