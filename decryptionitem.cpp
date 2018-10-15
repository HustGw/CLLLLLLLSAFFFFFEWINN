#include "decryptionitem.h"
int flag = 0;
DecryptionItem::DecryptionItem(QWidget *parent) : QWidget(parent)
{
    fileName = new QLabel(this);
    fileSize = new QLabel(this);
    fileDescription = new QLabel(this);


    label = new QLabel(this);
    label->setGeometry(0,0,34,16);
    label->setStyleSheet("QLabel{ border-image:url(:/new/mainwindow/pictures/finen_label.png); }");
    label->hide();

    progressBar = new QProgressBar(this);


    elseLabel = new QLabel(this);
    elseLabel->setGeometry(49,25,30,16);
    elseLabel->setStyleSheet("QLabel{background-color:#91A7B9 ; color:white;}");
    elseLabel->setAlignment(Qt::AlignCenter);


    fileName->setFont(QFont("冬青黑体简体",12,QFont::Bold));
    fileSize->setFont(QFont("冬青黑体简体",9));
    fileDescription->setFont(QFont("冬青黑体简体",10));
    fileSize->setStyleSheet("QLabel{color:#9999AB;background:transparent}");
    fileDescription->setStyleSheet("QLabel{color:#9999AB;background:transparent}");

    downloadBtn = new QPushButton(this);
    fileIcon = new QLabel(this);
    checkBox = new QCheckBox(this);
    checkBox->setCursor(QCursor(Qt::PointingHandCursor));
    timeLabel = new QLabel(this);
    //设置fileName、fileName fileDescription checkBox fileIcon 的位置

    downloadBtn->setStyleSheet("background:transparent");
    timeLabel->setStyleSheet("background:transparent");

    fileName->setGeometry(97,10,300,16);
    fileSize->setGeometry(97,28,300,12);
    fileDescription->setGeometry(97,42,400,14);

    downloadBtn->setGeometry(545,22,90,24);
    downloadBtn->setCursor(QCursor(Qt::PointingHandCursor));

    checkBox->setGeometry(15,25,13,13);
    checkBox->setStyleSheet("QCheckBox::indicator {width: 13px;height: 13px;}");

    fileIcon->setGeometry(45,10,38,46);

    timeLabel->setGeometry(294,31,230,10);
    timeLabel->setFont(QFont("Timers",8));
    timeLabel->setStyleSheet("color:#9999AB");
    progressBar->setGeometry(0,0,695,this->height()-1);
    QString strQSS = "QProgressBar { \
            text-align: center; \
            border: 1px ; \
            background: transparent; \
        } \
        QProgressBar::chunk { \
            background-color: rgba(230, 237, 255,0.8); \
        }";
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    progressBar->setStyleSheet(strQSS);
    fileName->setStyleSheet("background:transparent");
    downloadBtn->setStyleSheet("background:transparent");

}
//每一个Item绘制边界框
void DecryptionItem::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setPen(QColor(139,139,139));
    painter.drawLine(0,this->height()-1,this->width()-1,this->height()-1);
}

void DecryptionItem::changeCheckBox(){
    if(flag==0){
        checkBox->setCheckState(Qt::Checked);
        flag = 1;
        qDebug()<<"1";
    }
    else if(flag ==1){
        checkBox->setCheckState(Qt::Unchecked);
        flag = 0;
        qDebug()<<"2";
    }


}

