#include "msgbox.h"
MsgBox::MsgBox(int style,QString text)
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
    if(style == 4){
        titleText->setText(QStringLiteral("成功"));
    }
    else if(style == 2){
        titleText->setText(QStringLiteral("文件传输"));
    }else{
        titleText->setText(QStringLiteral("系统提示"));
    }
    titleLabel->setStyleSheet("QLabel{font-family :微软雅黑;font-size : 5em;color : rgb(255,255,255);background-color: #EEF0F5;}");

    closeBtn = new QPushButton(this);
    closeBtn->setGeometry(430, 6, 13, 13);
    closeBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/delete_button.png);background-color: #EEF0F5;}QPushButton:hover{border-image:url(:/new/mainwindow/pictures/delete_button_hover.png);background-color: #EEF0F5;}");
    closeBtn->setCursor(QCursor(Qt::PointingHandCursor));

    msgBtn = new QPushButton(this);
    msgBtn->setGeometry(40,69,36,36);
    //1 疑问 ？
    //2 失败 ！（红）
    //3 提示 ！（蓝）
    //4 成功 √
    //2、3、4没有取消按钮
    if(style == 1){
        msgBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/system_waring.png);background: transparent;border:none;}");
    }
    else if(style == 2){
        msgBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/system_waring.png);background: transparent;border:none;}");
    }
    else if(style == 3){
        msgBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/system_waring.png);background: transparent;border:none;}");
    }else if(style == 4){
        msgBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/system_success.png);background: transparent;border:none;}");
    }
    askLabel = new QLabel(this);
    askLabel->setGeometry(94,79,350,20);
    askLabel->setText(text);

    okBtn=new QPushButton(this);
    if((style==2)||(style==3)||(style == 4)){
        okBtn->setGeometry(336,138,89,27);
    }
    else{
        okBtn->setGeometry(222,138,89,27);
    }
    okBtn->setText(QStringLiteral("确定"));
    okBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: rgb(119,146,183);}QPushButton:pressed{background-color: rgb(139,159,185);}");

    cancleBtn=new QPushButton(this);
    cancleBtn->setGeometry(336,138,89,27);
    cancleBtn->setText(QStringLiteral("取消"));
    cancleBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: rgb(119,146,183);}QPushButton:pressed{background-color: rgb(139,159,185);}");
    if((style==2)||(style==3)||(style == 4)){
        cancleBtn->hide();
    }
    else{
        cancleBtn->show();
    }
    connect(okBtn,SIGNAL(clicked()),this,SLOT(okBtn_press()));
    connect(closeBtn,SIGNAL(clicked()),this,SLOT(closeBtn_press()));
    connect(cancleBtn,SIGNAL(clicked()),this,SLOT(cancleBtn_press()));
}




MsgBox::~MsgBox(void)
{
}



void MsgBox::mousePressEvent(QMouseEvent *qevent)
{
    if(qevent->button() == Qt::LeftButton)
    {
        mouse_press = true;
        //鼠标相对于窗体的位置（或者使用event->globalPos() - this->pos()）
        move_point = qevent->pos();;
    }
}
void MsgBox::mouseMoveEvent(QMouseEvent *qevent)
{
    //若鼠标左键被按下
    if(mouse_press)
    {
        //鼠标相对于屏幕的位置
        QPoint move_pos = qevent->globalPos();

        //移动主窗体位置
        this->move(move_pos - move_point);
    }
}
void MsgBox::mouseReleaseEvent(QMouseEvent *qevent)
{
    //设置鼠标为未被按下
    mouse_press = false;
}

void MsgBox::okBtn_press()
{
    //qDebug()<<1;
    this->accept();
}
void MsgBox::cancleBtn_press()
{
    this->reject();
}
void MsgBox::closeBtn_press()
{
    close();
}
