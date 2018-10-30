#include "decryptionitem.h"
extern QStringList m_fontList;
int flag = 0;
QPalette pa;


DecryptionItem::DecryptionItem(QWidget *parent) : QWidget(parent)
{
    pa.setColor(QPalette::Background, QColor(0x00,0xff,0x00,0x00));


    QFont f_14;
     QFont f_12;
     QFont f_10;
     f_14.setFamily(m_fontList.at(0));
     f_12.setFamily(m_fontList.at(0));
     f_10.setFamily(m_fontList.at(0));
     f_14.setPixelSize(14);
     f_12.setPixelSize(12);
     f_10.setPixelSize(10);
     f_14.setWeight(QFont::Bold);
     f_12.setWeight(QFont::Normal);
     f_10.setWeight(QFont::Normal);

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


    fileName->setFont(f_14);
    fileSize->setFont(f_10);
    fileDescription->setFont(f_12);
    fileSize->setStyleSheet("QLabel{color:#9999AB;background:transparent}");
    fileDescription->setStyleSheet("QLabel{color:#9999AB;background:transparent}");

    downloadBtn = new QPushButton(this);
    fileIcon = new QLabel(this);
    checkBox = new QCheckBox(this);
    checkBox->setCursor(QCursor(Qt::PointingHandCursor));
    timeLabel = new QLabel(this);
    //设置fileName、fileName fileDescription checkBox fileIcon 的位置
    timeLabel->setPalette (pa);
    downloadBtn->setStyleSheet("background:transparent");
    //timeLabel->setStyleSheet("background:transparent");

    fileName->setGeometry(97,10,300,16);
    fileSize->setGeometry(97,28,300,12);
    fileDescription->setGeometry(97,42,400,14);

    downloadBtn->setGeometry(545,22,90,24);
    downloadBtn->setCursor(QCursor(Qt::PointingHandCursor));

    checkBox->setGeometry(15,25,13,13);
    checkBox->setStyleSheet("QCheckBox::indicator {width: 13px;height: 13px;}");

    fileIcon->setGeometry(45,10,38,46);

    timeLabel->setGeometry(294,31,230,10);
    timeLabel->setFont(f_10);
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
    f_14.setWeight(QFont::Normal);
    downloadBtn->setFont(f_14);
    f_14.setWeight(QFont::Bold);
}
//每一个Item绘制边界框
void DecryptionItem::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setPen(QColor(237,237,237));
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

