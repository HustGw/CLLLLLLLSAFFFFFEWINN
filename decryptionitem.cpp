#include "decryptionitem.h"

DecryptionItem::DecryptionItem(QWidget *parent) : QWidget(parent)
{
    fileName = new QLabel(this);
    fileSize = new QLabel(this);
    fileDescription = new QLabel(this);



    fileName->setFont(QFont("Timers",11,QFont::Bold));
    fileSize->setFont(QFont("Timers",8));
    fileDescription->setFont(QFont("Timers",8));
    fileSize->setStyleSheet("color:#9999AB");
    fileDescription->setStyleSheet("color:#9999AB");

    downloadBtn = new QPushButton(this);
    fileIcon = new QLabel(this);
    checkBox = new QCheckBox(this);
    checkBox->setCursor(QCursor(Qt::PointingHandCursor));
    timeLabel = new QLabel(this);
    //设置fileName、fileName fileDescription checkBox fileIcon 的位置

    downloadBtn->setStyleSheet("background:transparent");
    timeLabel->setStyleSheet("background:transparent");

    fileName->setGeometry(97,10,300,16);
    fileSize->setGeometry(97,28,300,11);
    fileDescription->setGeometry(97,41,400,12);

    downloadBtn->setGeometry(545,22,90,24);
    downloadBtn->setCursor(QCursor(Qt::PointingHandCursor));

    checkBox->setGeometry(15,25,13,13);
    checkBox->setStyleSheet("QCheckBox::indicator {width: 13px;height: 13px;}");

    fileIcon->setGeometry(45,10,38,46);

    timeLabel->setGeometry(294,31,230,10);
    timeLabel->setFont(QFont("Timers",8));
    timeLabel->setStyleSheet("color:#9999AB");
    progressBar->setGeometry(20,0,this->width()-1,this->height()-1);
    QString strQSS = "QProgressBar { \
            text-align: center; \
            border: 1px ; \
            background: transparent; \
        } \
        QProgressBar::chunk { \
            background-color: rgba(235, 241, 255,180); \
        }";
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    progressBar->setStyleSheet(strQSS);
    fileName->setStyleSheet("background:transparent");
    fileDescription->setStyleSheet("background:transparent");
    fileSize->setStyleSheet("background:transparent");
    downloadBtn->setStyleSheet("background:transparent");
    timeLabel->setStyleSheet("background:transparent");
}
//每一个Item绘制边界框
void DecryptionItem::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setPen(QColor(139,139,139));
    painter.drawLine(0,this->height()-1,this->width()-1,this->height()-1);
}

void DecryptionItem::changeCheckBox(){
    checkBox->setCheckState(Qt::Checked);

}

