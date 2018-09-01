#include "encryptionitem.h"
extern bool initPageFlag;

EncryptionItem::EncryptionItem(QWidget *parent) : QWidget(parent)
{
    if (initPageFlag){
        initPage = new QLabel(this);
        initPage->setGeometry(241,162,215,30);
        initPage->setText("请调取本地文件进行加密！");
        //checkBox->hide();
    }
    else{

        fileName = new QLabel(this);
        fileSize = new QLabel(this);
        fileDescription = new QLabel(this);
        timeInfo = new QLabel(this);

        fileName->setFont(QFont("Timers",11,QFont::Bold));
        fileSize->setFont(QFont("Timers",8));
        fileDescription->setFont(QFont("Timers",8));
        fileSize->setStyleSheet("color:#9999AB");
        fileDescription->setStyleSheet("color:#9999AB");
        timeInfo->setFont(QFont("Timers",8));;
        timeInfo->setStyleSheet("color:#9999AB");

        fileDescription->setStyleSheet("background:transparent");
        fileName->setStyleSheet("background:transparent");
        fileSize->setStyleSheet("background:transparent");
        timeInfo->setStyleSheet("background:transparent");

        //downloadBtn = new QPushButton(this);
        fileIcon = new QLabel(this);

        encryptStaBtn = new QPushButton(this);


        //设置fileName、fileName fileDescription checkBox fileIcon 的位置
        fileName->setGeometry(95,12,150,16);
        timeInfo->setGeometry(300,31,200,30 );
        fileSize->setGeometry(95,31,100,11);
        fileDescription->setGeometry(95,47,100,12);
        //downloadBtn->setGeometry(500,50,100,30);

        //checkBox->setGeometry(10,70,20,20);
        fileIcon->setGeometry(50,13,38,46);
        encryptStaBtn->setGeometry(95,47,100,12);
        encryptStaBtn->setFont(QFont("Timers",8));
        encryptStaBtn->setStyleSheet("color:#9999AB");
        encryptStaBtn->setStyleSheet("background:transparent");

        QDateTime time = QDateTime::currentDateTime();
        QString time_str = time.toString("yyyy-MM-dd hh:mm:ss");
        timeInfo->setText(time_str);
        encryptStaBtn->hide();

    }

    progressBar->setGeometry(10,0,650,65);
    QString strQSS = "QProgressBar { \
            text-align: center; \
            border: 1px ; \
            background: transparent; \
        } \
        QProgressBar::chunk { \
            background-color: rgba(235, 241, 255,180); \
        }";
    //progressBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);  // 对齐方式
    progressBar->setOrientation(Qt::Horizontal);
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    progressBar->setStyleSheet(strQSS);
    //progressBar->setValue(20);
   // progressBar->setGeometry(450,85,100,20);
//    double dProgress = (progressBar->value() - progressBar->minimum()) * 100.0
//                    / (progressBar->maximum() - progressBar->minimum());
//    progressBar->setFormat(QString::fromLocal8Bit(u8"%1%").arg(QString::number(dProgress, 'f', 1)));
    //progressBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);  // 对齐方式
}
//每一个Item绘制边界框
void EncryptionItem::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setPen(QColor(139,139,139));
    painter.drawLine(0,this->height()-1,this->width()-1,this->height()-1);
//    painter.drawLine(31,0,31,this->height()-1);
//    painter.drawLine(this->width()-1,0,this->width()-1,this->height()-1);
//    painter.drawLine(31,this->height()-1,this->width()-1,this->height()-1);


}
