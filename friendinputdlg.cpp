#include "friendinputdlg.h"

friendInputDlg::friendInputDlg(QWidget *parent):QDialog(parent)
{
    setWindowTitle(tr("好友添加"));
    titleLabel = new QLabel(this);
    titleLabel->setText(tr("请输入对方名称"));
    inputLineEdit = new QLineEdit(this);
    titleLabel->setGeometry(10,10,150,40);
    inputLineEdit->setGeometry(10,60,200,30);
    inputLineEdit->setPlaceholderText(tr("输入对方昵称"));
    okBtn = new QPushButton(this);
    cancelBtn = new QPushButton(this);
    okBtn->setText(tr("确定"));
    cancelBtn->setText(tr("取消"));
    okBtn->setGeometry(40,100,40,30);
    cancelBtn->setGeometry(200,100,40,30);
    connect(okBtn,SIGNAL(clicked(bool)),this,SLOT(sendName()));
    connect(cancelBtn,SIGNAL(clicked(bool)),this,SLOT(cancel()));
 //   this->setFixedSize(this->width(),this->height());

}
void friendInputDlg::sendName(){
    QString name = inputLineEdit->text();
    emit sendNameToMain(name);
    this->accept();

}
void friendInputDlg::cancel(){
    this->accept();
}
