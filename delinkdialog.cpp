#include "delinkdialog.h"

DelinkDialog::DelinkDialog(QWidget *parent):QDialog(parent)
{
    setWindowTitle(tr("链接添加"));
    titleLabel = new QLabel(this);
    titleLabel->setText(tr("请输入链接"));
    inputLineEdit = new QLineEdit(this);
    titleLabel->setGeometry(10,10,150,40);
    inputLineEdit->setGeometry(10,60,200,30);
    inputLineEdit->setPlaceholderText(tr("输入链接"));
    okBtn = new QPushButton(this);
    cancelBtn = new QPushButton(this);
    okBtn->setText(tr("确定"));
    cancelBtn->setText(tr("取消"));
    okBtn->setGeometry(40,100,40,30);
    cancelBtn->setGeometry(200,100,40,30);
    connect(okBtn,SIGNAL(clicked(bool)),this,SLOT(sendLink()));
    connect(cancelBtn,SIGNAL(clicked(bool)),this,SLOT(cancel()));
}

void DelinkDialog::sendLink(){
    QString link = inputLineEdit->text();
    emit sendLinkToMain(link);
    this->accept();
}

void DelinkDialog::cancel(){
    this->accept();
}
