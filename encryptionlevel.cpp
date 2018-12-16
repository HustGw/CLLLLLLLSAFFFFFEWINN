#include "encryptionlevel.h"
extern int extraction_rate;
QFont f_4;
QFont f_5;
encryptionLevel::encryptionLevel(QWidget* parent):QDialog(parent)
{
    this->resize(450,275);
    QFont f_1;
    //f_1.setFamily(m_fontList.at(0));
    f_1.setPixelSize(14);
    f_1.setWeight(QFont::Normal);
    f_1.setFamily("微软雅黑");

    QFont f_2;
    //f_2.setFamily(m_fontList.at(0));
    f_2.setPixelSize(14);
    f_2.setWeight(QFont::Bold);
    f_2.setFamily("微软雅黑");

    QFont f_3;
    f_3.setPixelSize(12);
    f_3.setWeight(QFont::Normal);
    f_3.setFamily("微软雅黑");


    f_4.setPixelSize(16);
    f_4.setWeight(QFont::Normal);
    f_4.setFamily("微软雅黑");


    f_5.setPixelSize(20);
    f_5.setWeight(QFont::Bold);
    f_5.setFamily("微软雅黑");

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
    titleText->setFont(f_2);
    titleText->setStyleSheet("background-color: #EEF0F5;");
    titleIcon = new QLabel(this);
    titleIcon->setGeometry(21,17,23,17);
    titleIcon->setStyleSheet("border-image: url(:/new/mainwindow/pictures/system_icon.png);"
                             "background-color: #EEF0F5;");
    titleText->setText(QStringLiteral("加密等级"));
    titleLabel->setStyleSheet("QLabel{font-family :微软雅黑;font-size : 5em;color : rgb(255,255,255);background-color: #EEF0F5;}");
    titleLabel->setFont(f_2);

    closeBtn = new QPushButton(this);
    closeBtn->setGeometry(423, 12, 13, 13);
    closeBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/delete_button.png);background-color: #EEF0F5;}QPushButton:hover{border-image:url(:/new/mainwindow/pictures/delete_button_hover.png);background-color: #EEF0F5;}");
    closeBtn->setCursor(QCursor(Qt::PointingHandCursor));

    weakBtn = new QPushButton(this);
    weakBtn->setGeometry(81,80,94,12);
    weakBtn->setCursor(QCursor(Qt::PointingHandCursor));
    weakBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/level_grey.png)}");

    midBtn = new QPushButton(this);
    midBtn->setGeometry(177,80,94,12);
    midBtn->setCursor(QCursor(Qt::PointingHandCursor));
    midBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/level_grey.png)}");

    hardBtn = new QPushButton(this);
    hardBtn->setGeometry(273,80,94,12);
    hardBtn->setCursor(QCursor(Qt::PointingHandCursor));
    hardBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/level_grey.png)}");

    weakLabel = new QLabel(this);
    weakLabel->setGeometry(81,50,94,25);
    weakLabel->setFont(f_4);
    weakLabel->setText("弱");

    midLabel = new QLabel(this);
    midLabel->setGeometry(177,50,94,25);
    midLabel->setAlignment(Qt::AlignCenter);
    midLabel->setFont(f_4);
    midLabel->setText("中");

    hardLabel = new QLabel(this);
    hardLabel->setGeometry(273,50,94,25);
    hardLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    hardLabel->setFont(f_4);
    hardLabel->setText("强");

    askLabel = new QLabel(this);
    askLabel->setGeometry(28,134,390,50);
    askLabel->setStyleSheet("color:grey");
    askLabel->setText("加密等级越强则被加密数据更具有安全保障，但文件较大时耗时可能增加；\n加密等级越弱则速度会更快，同样具有安全保障。");
    askLabel->setFont(f_3);

    deleteBtn = new QPushButton(this);
    deleteBtn->setText(QStringLiteral("删除条目及文件"));
    deleteBtn->setGeometry(73,133,119,27);
    deleteBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: #3A8CFF;color:white;}QPushButton:pressed{background-color: rgb(139,159,185);}");
    deleteBtn->setCursor(QCursor(Qt::PointingHandCursor));
    deleteBtn->setFont(f_1);
    deleteBtn->hide();

    okBtn=new QPushButton(this);
    okBtn->setGeometry(331,222,88,26);
    okBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/levelBtn_disable.png);}");
    okBtn->setCursor(QCursor(Qt::PointingHandCursor));
    okBtn->setFont(f_1);
    okBtn->setEnabled(false);

    cancleBtn=new QPushButton(this);
    cancleBtn->setGeometry(217,222,88,26);
    cancleBtn->setText(QStringLiteral("取消"));
    cancleBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);color:grey;}QPushButton:hover{background-color: #3A8CFF;color:white;}QPushButton:pressed{background-color: rgb(139,159,185);color:grey}");
    cancleBtn->show();
    cancleBtn->setCursor(QCursor(Qt::PointingHandCursor));
    cancleBtn->setFont(f_1);

    connect(okBtn,SIGNAL(clicked()),this,SLOT(okBtn_press()));
    connect(deleteBtn,SIGNAL(clicked()),this,SLOT(deletBtn_press()));
    connect(closeBtn,SIGNAL(clicked()),this,SLOT(closeBtn_press()));
    connect(cancleBtn,SIGNAL(clicked()),this,SLOT(cancleBtn_press()));
    connect(weakBtn,SIGNAL(clicked()),this,SLOT(weakBtn_press()));
    connect(midBtn,SIGNAL(clicked()),this,SLOT(midBtn_press()));
    connect(hardBtn,SIGNAL(clicked()),this,SLOT(hardBtn_press()));
}
encryptionLevel::~encryptionLevel(void)
{
}



void encryptionLevel::mousePressEvent(QMouseEvent *qevent)
{
    if(qevent->button() == Qt::LeftButton)
    {
        mouse_press = true;
        //鼠标相对于窗体的位置（或者使用event->globalPos() - this->pos()）
        move_point = qevent->pos();;
    }
}
void encryptionLevel::mouseMoveEvent(QMouseEvent *qevent)
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
void encryptionLevel::mouseReleaseEvent(QMouseEvent *qevent)
{
    //设置鼠标为未被按下
    mouse_press = false;
}

void encryptionLevel::okBtn_press()
{
    //qDebug()<<1;
    this->accept();
}
void encryptionLevel::cancleBtn_press()
{
    this->reject();
}
void encryptionLevel::closeBtn_press()
{
    close();
}
void encryptionLevel::deletBtn_press(){
    this->done(5);
}
void encryptionLevel::weakBtn_press()
{

    weakBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/level_red.png)}");
    midBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/level_grey.png)}");
    hardBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/level_grey.png)}");
    okBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/levelBtn.png);}QPushButton:hover{border-image:url(:/new/mainwindow/pictures/levelBtn_hover.png)}");
    okBtn->setEnabled(true);
    weakLabel->setFont(f_5);
    midLabel->setFont(f_4);
    hardLabel->setFont(f_4);
    extraction_rate = 100;
    qDebug()<<extraction_rate;
}
void encryptionLevel::midBtn_press()
{
    weakBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/level_orange.png)}");
    midBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/level_orange.png)}");
    hardBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/level_grey.png)}");
    okBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/levelBtn.png);}QPushButton:hover{border-image:url(:/new/mainwindow/pictures/levelBtn_hover.png)}");
    okBtn->setEnabled(true);
    weakLabel->setFont(f_4);
    midLabel->setFont(f_5);
    hardLabel->setFont(f_4);
    extraction_rate = 30;
    qDebug()<<extraction_rate;
}
void encryptionLevel::hardBtn_press()
{
    weakBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/level_green.png)}");
    midBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/level_green.png)}");
    hardBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/level_green.png)}");
    okBtn->setStyleSheet("QPushButton{border-image:url(:/new/mainwindow/pictures/levelBtn.png);}QPushButton:hover{border-image:url(:/new/mainwindow/pictures/levelBtn_hover.png)}");
    okBtn->setEnabled(true);
    weakLabel->setFont(f_4);
    midLabel->setFont(f_4);
    hardLabel->setFont(f_5);
    extraction_rate = 20;
    qDebug()<<extraction_rate;
}



void encryptionLevel::paintEvent(QPaintEvent *event)
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
