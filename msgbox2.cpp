#include "msgbox2.h"

QStringList m_fontList3;
MsgBox2::MsgBox2(int style,QString text,QWidget* parent):QDialog(parent)
{
    QString dir = QCoreApplication::applicationDirPath();
    m_fontList3.clear();

    int lcdFontId = QFontDatabase::addApplicationFont(":/pictures/W3.ttf"); // 从source资源文件
    // int lcdFontId = QFontDatabase::addApplicationFont(dir + "/fonts/DS-DIGI.ttf"); //从外部资源文件
    if (lcdFontId != -1) // -1为加载失败
    {
        m_fontList3 << QFontDatabase::applicationFontFamilies(lcdFontId);
    }
    this->resize(450,190);
    QFont f3;
    f3.setFamily(m_fontList3.at(0));
    f3.setPixelSize(14);
    f3.setWeight(QFont::Normal);
    QFont f4;
    f4.setFamily(m_fontList3.at(0));
    f4.setPixelSize(14);
    f4.setWeight(QFont::Bold);

  //获取主界面的宽度
    int width = this->width();
    setStyleSheet("QWidget{background-color: #FFFFFF;}");
  //初始化为未按下鼠标左键

  //设置标题栏隐藏
    this->setWindowFlags(windowFlags()|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    titleLabel = new QLabel(this);
    titleLabel->setGeometry(6,6,width-12,38);
    titleText = new QLabel(this);
    titleText->setGeometry(53,18,200,15);
    titleText->setFont(f4);
    titleText->setStyleSheet("background-color: #EEF0F5;");
    titleIcon = new QLabel(this);
    titleIcon->setGeometry(21,17,23,17);
    titleIcon->setStyleSheet("border-image: url(:/new/mainwindow/pictures/system_icon.png);"
                             "background-color: #EEF0F5;");
    if(style == 4){
        titleText->setText(QStringLiteral("成功"));
    }else if(style==5){
        titleText->setText(QStringLiteral("删除"));
    }else if(style == 6){
        titleText->setText(QStringLiteral("文件传输"));
    }else{
        titleText->setText(QStringLiteral("系统提示"));
    }
    titleLabel->setStyleSheet("QLabel{font-family :微软雅黑;font-size : 5em;color : rgb(255,255,255);background-color: #EEF0F5;}");
    titleLabel->setFont(f4);
    closeBtn = new QPushButton(this);
    closeBtn->setGeometry(414, 12, 13, 13);
    closeBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/delete_button.png);background-color: #EEF0F5;}QPushButton:hover{border-image:url(:/new/mainwindow/pictures/delete_button_hover.png);background-color: #EEF0F5;}");
    closeBtn->setCursor(QCursor(Qt::PointingHandCursor));

    msgBtn = new QPushButton(this);
    msgBtn->setGeometry(35,64,36,36);
    //1 疑问 ？
    //2 失败 ！（红）
    //3 提示 ！（蓝）
    //4 成功 √
    //2、3、4没有取消按钮
    if((style == 1)||(style == 5)){
        msgBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/system_question.png);background: transparent;border:none;}");
    }
    else if((style == 2)||(style == 6)){
        msgBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/system_fail.png);background: transparent;border:none;}");
    }
    else if(style == 3){
        msgBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/system_warning.png);background: transparent;border:none;}");
    }else if(style == 4){
        msgBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/system_success.png);background: transparent;border:none;}");
    }
    askLabel = new QLabel(this);
    askLabel->setGeometry(89,64,300,40);
    askLabel->setText(text);
    askLabel->setFont(f3);
    deleteBtn = new QPushButton(this);
    deleteBtn->setText(QStringLiteral("删除条目及文件"));
    deleteBtn->setGeometry(73,133,119,27);
    deleteBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: #3A8CFF;color:white;}QPushButton:pressed{background-color: rgb(139,159,185);}");
    deleteBtn->setCursor(QCursor(Qt::PointingHandCursor));
    deleteBtn->setFont(f3);
    deleteBtn->hide();
    okBtn=new QPushButton(this);
    if((style==2)||(style==3)||(style == 4)||(style == 6)){
        okBtn->setGeometry(331,133,89,27);
    }
    else{
        okBtn->setGeometry(217,133,89,27);
    }
    if(style == 5){
        okBtn->setText(QStringLiteral("删除条目"));
        deleteBtn->show();
    }else{
        okBtn->setText(QStringLiteral("确定"));
    }
    okBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: #3A8CFF;color:white;}QPushButton:pressed{background-color: rgb(139,159,185);}");
    okBtn->setCursor(QCursor(Qt::PointingHandCursor));
    okBtn->setFont(f3);
    cancleBtn=new QPushButton(this);
    cancleBtn->setGeometry(331,133,89,27);
    cancleBtn->setText(QStringLiteral("取消"));
    cancleBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: #3A8CFF;color:white;}QPushButton:pressed{background-color: rgb(139,159,185);}");
    if((style==2)||(style==3)||(style == 4)||(style == 6)){
        cancleBtn->hide();
    }
    else{
        cancleBtn->show();
    }
    cancleBtn->setCursor(QCursor(Qt::PointingHandCursor));
    cancleBtn->setFont(f3);
    connect(okBtn,SIGNAL(clicked()),this,SLOT(okBtn_press()));
    connect(deleteBtn,SIGNAL(clicked()),this,SLOT(deletBtn_press()));
    connect(closeBtn,SIGNAL(clicked()),this,SLOT(closeBtn_press()));
    connect(cancleBtn,SIGNAL(clicked()),this,SLOT(cancleBtn_press()));
}




MsgBox2::~MsgBox2(void)
{
}



void MsgBox2::mousePressEvent(QMouseEvent *qevent)
{
    if(qevent->button() == Qt::LeftButton)
    {
        mouse_press = true;
        //鼠标相对于窗体的位置（或者使用event->globalPos() - this->pos()）
        move_point = qevent->pos();;
    }
}
void MsgBox2::mouseMoveEvent(QMouseEvent *qevent)
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
void MsgBox2::mouseReleaseEvent(QMouseEvent *qevent)
{
    //设置鼠标为未被按下
    mouse_press = false;
}

void MsgBox2::okBtn_press()
{
    //qDebug()<<1;
    this->accept();
}
void MsgBox2::cancleBtn_press()
{
    this->reject();
}
void MsgBox2::closeBtn_press()
{
    close();
}
void MsgBox2::deletBtn_press(){
    this->done(5);
}
void MsgBox2::paintEvent(QPaintEvent *event)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(5, 5, this->width()-10, this->height()-10);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));

    QColor color(128, 128, 128, 50);
    for(int i=0; i<5; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(5-i, 5-i, this->width()-(5-i)*2, this->height()-(5-i)*2);
        color.setAlpha(120 - qSqrt(i)*50);
        painter.setPen(color);
        painter.drawPath(path);
    }
}
