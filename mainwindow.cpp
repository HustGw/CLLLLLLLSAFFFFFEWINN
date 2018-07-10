#include "mainwindow.h"
#include "ui_mainwindow.h"
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
//批量删除按钮
void MainWindow::on_pushButton_3_clicked()
{


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
