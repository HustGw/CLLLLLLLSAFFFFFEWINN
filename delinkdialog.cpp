#include "delinkdialog.h"

DelinkDialog::DelinkDialog(QWidget *parent):QDialog(parent)
{
    this->resize(450,190);

  //获取主界面的宽度
    int width = this->width();
    setStyleSheet("QWidget{background-color: #FFFFFF;}");
  //初始化为未按下鼠标左键

  //设置标题栏隐藏
    this->setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    titleLabel = new QLabel(this);
    titleLabel->setGeometry(0,0,width,38);
    titleText = new QLabel(this);
    titleText->setGeometry(48,13,200,15);
    titleText->setFont(QFont("Timers",9,QFont::Bold));
    titleText->setStyleSheet("background-color: #EEF0F5;");
    titleIcon = new QLabel(this);
    titleIcon->setGeometry(16,12,23,17);
    titleIcon->setStyleSheet("border-image: url(:/new/mainwindow/pictures/system_icon.png);"
                             "background-color: #EEF0F5;");
    titleText->setText(QStringLiteral("链接下载"));

    titleLabel->setStyleSheet("QLabel{font-family :微软雅黑;font-size : 5em;color : rgb(255,255,255);background-color: #EEF0F5;}");

    closeBtn = new QPushButton(this);
    closeBtn->setGeometry(430, 6, 13, 13);
    closeBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/delete_button.png);background-color: #EEF0F5;}QPushButton:hover{border-image:url(:/new/mainwindow/pictures/delete_button_hover.png);background-color: #EEF0F5;}");
    closeBtn->setCursor(QCursor(Qt::PointingHandCursor));


    askLabel = new QLabel(this);
    askLabel->setGeometry(22,50,150,15);
    askLabel->setText(QStringLiteral("链接地址："));

    okBtn=new QPushButton(this);
    okBtn->setGeometry(222,138,89,27);
    okBtn->setText(QStringLiteral("确认下载"));
    okBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: rgb(119,146,183);}QPushButton:pressed{background-color: rgb(139,159,185);}");

    cancleBtn=new QPushButton(this);
    cancleBtn->setGeometry(336,138,89,27);
    cancleBtn->setText(QStringLiteral("取消下载"));
    cancleBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: rgb(119,146,183);}QPushButton:pressed{background-color: rgb(139,159,185);}");

    inputLineEdit = new QLineEdit(this);
    inputLineEdit->setGeometry(34,74,391,31);
    inputLineEdit->setPlaceholderText(tr("输入链接"));

    connect(okBtn,SIGNAL(clicked(bool)),this,SLOT(sendLink()));
    connect(cancleBtn,SIGNAL(clicked(bool)),this,SLOT(cancel()));
    connect(closeBtn,SIGNAL(clicked()),this,SLOT(closeBtn_press()));

}

void DelinkDialog::sendLink(){
        QString link = inputLineEdit->text().section("&&",1,1);
        emit sendLinkToMain(link);
        this->accept();
}

void DelinkDialog::cancel(){
    this->accept();
}
void DelinkDialog::closeBtn_press()
{
    close();
}
void DelinkDialog::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        mouse_press = true;
            dragPosition = event->globalPos() - frameGeometry().topLeft();
            event->accept();
        }
}
void DelinkDialog::mouseMoveEvent(QMouseEvent *event){
    if (event->buttons() & Qt::LeftButton) {
        if(mouse_press == true){
            move(event->globalPos() - dragPosition);
            event->accept();
        }
        }

}
void DelinkDialog::mouseReleaseEvent(QMouseEvent *event){
    mouse_press = false;
}


