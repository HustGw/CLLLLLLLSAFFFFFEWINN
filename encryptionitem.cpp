#include "encryptionitem.h"

EncryptionItem::EncryptionItem(QWidget *parent) : QWidget(parent)
{
    fileName = new QLabel(this);
    fileSize = new QLabel(this);
    fileDescription = new QLabel(this);
    //downloadBtn = new QPushButton(this);
    fileIcon = new QLabel(this);
    checkBox = new QCheckBox(this);
    //设置fileName、fileName fileDescription checkBox fileIcon 的位置
    fileName->setGeometry(300,10,300,30);
    fileSize->setGeometry(300,40,100,30);
    fileDescription->setGeometry(300,80,200,30);
    //downloadBtn->setGeometry(500,50,100,30);
    checkBox->setGeometry(10,70,20,20);
    fileIcon->setGeometry(30,50,300,30);
    progressBar->setMinimum(0);
    progressBar->setMaximum(0);
    progressBar->setValue(20);
    progressBar->setGeometry(450,85,100,20);
//    double dProgress = (progressBar->value() - progressBar->minimum()) * 100.0
//                    / (progressBar->maximum() - progressBar->minimum());
//    progressBar->setFormat(QString::fromLocal8Bit(u8"%1%").arg(QString::number(dProgress, 'f', 1)));
    progressBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);  // 对齐方式
}
//每一个Item绘制边界框
void EncryptionItem::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setPen(QColor(139,139,139));
    painter.drawLine(31,0,this->width()-1,0);
    painter.drawLine(31,0,31,this->height()-1);
    painter.drawLine(this->width()-1,0,this->width()-1,this->height()-1);
    painter.drawLine(31,this->height()-1,this->width()-1,this->height()-1);


}
