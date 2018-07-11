#include "decryptionitem.h"

DecryptionItem::DecryptionItem(QWidget *parent) : QWidget(parent)
{
    fileName = new QLabel(this);
    fileSize = new QLabel(this);
    fileDescription = new QLabel(this);
    downloadBtn = new QPushButton(this);
    fileIcon = new QLabel(this);
    checkBox = new QCheckBox(this);
    //设置fileName、fileName fileDescription checkBox fileIcon 的位置
    fileName->setGeometry(200,10,100,30);
    fileSize->setGeometry(200,40,100,30);
    fileDescription->setGeometry(200,80,250,30);
    downloadBtn->setGeometry(500,50,100,30);
    checkBox->setGeometry(10,70,20,20);
    fileIcon->setGeometry(30,50,100,30);

}
//每一个Item绘制边界框
void DecryptionItem::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setPen(QColor(139,139,139));
    painter.drawLine(31,0,this->width()-1,0);
    painter.drawLine(31,0,31,this->height()-1);
    painter.drawLine(this->width()-1,0,this->width()-1,this->height()-1);
    painter.drawLine(31,this->height()-1,this->width()-1,this->height()-1);
}

void DecryptionItem::changeCheckBox(){
    checkBox->setCheckState(Qt::Checked);
}

