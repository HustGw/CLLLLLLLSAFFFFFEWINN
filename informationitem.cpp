#include "informationitem.h"

InformationItem::InformationItem(QWidget *parent) : QWidget(parent)
{
    allowBtn = new QPushButton(this);
    ignoreBtn = new QPushButton(this);
    titleLabel = new QLabel(this);
    InforKindsLabel = new QLabel(this);
    timeLabel =  new QLabel(this);
    allowBtn->setText(tr("允许"));
    ignoreBtn->setText(tr("忽略"));
    InforKindsLabel->setText("申请授权");
    timeLabel->setText("2018-8-2 14:33:59");
    allowBtn->setGeometry(300,60,80,40);
    ignoreBtn->setGeometry(420,60,80,40);
    titleLabel->setGeometry(10,30,400,45);
    QFont font("Microsoft YaHei",10,75);
    InforKindsLabel->setGeometry(10,5,80,20);
    InforKindsLabel->setFont(font);
    timeLabel->setGeometry(320,10,200,40);

}


void InformationItem::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setPen(QColor(139,139,139));
    painter.drawLine(5,0,this->width()-1,0);
    painter.drawLine(5,0,5,this->height()-1);
    painter.drawLine(this->width()-1,0,this->width()-1,this->height()-1);
    painter.drawLine(5,this->height()-1,this->width()-1,this->height()-1);
}
