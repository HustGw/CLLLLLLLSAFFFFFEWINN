#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFont>
extern int isFinishedBtn=0;//用于判断是否已经点击
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
    ui->MidStaWidget->addWidget(encryptionPage);
    ui->MidStaWidget->addWidget(decryptionViewController);
//    ui->BtnStaWidget->addWidget(encryptionBtnItem);
//    ui->BtnStaWidget->addWidget(decryptionBtnItem);
    QLabel *label1 = new QLabel(tr("这是页面3"));
    ui->MidStaWidget->addWidget(label1);
    ui->FinDepBtn->hide();
    ui->FinEnpBtn->hide();
    this->setFixedSize(this->width(),this->height());
    QFont font("Microsoft YaHei",10,75);
    this->setFont(font);

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
    ui->MidStaWidget->setCurrentWidget(encryptionPage);
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

}
