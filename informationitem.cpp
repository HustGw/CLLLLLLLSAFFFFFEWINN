#include "informationitem.h"
InformationItem::InformationItem(QWidget *parent) : QWidget(parent)
{
    lineFlag = 0;
    allowBtn = new QPushButton(this);
    ignoreBtn = new QPushButton(this);
    titleLabel = new QLabel(this);
    InforKindsLabel = new QLabel(this);
    timeLabel =  new QLabel(this);
    allowBtn->setText(QStringLiteral("允许"));
    allowBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: #3A8CFF;color:white;}QPushButton:pressed{background-color: rgb(139,159,185);}");
    ignoreBtn->setText(QStringLiteral("忽略"));
    ignoreBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:4px;border-color: rgb(139,159,185);}QPushButton:hover{background-color: #3A8CFF;color:white;}QPushButton:pressed{background-color: rgb(139,159,185);}");
    InforKindsLabel->setText("申请授权");
    timeLabel->setText("2018-8-2 14:33:59");
    allowBtn->setGeometry(285,75,80,30);
    ignoreBtn->setGeometry(395,75,80,30);
    titleLabel->setGeometry(10,30,450,45);
    QFont font("Microsoft YaHei",10,75);
    InforKindsLabel->setGeometry(10,5,80,20);
    InforKindsLabel->setFont(font);
    timeLabel->setGeometry(275,5,200,40);

}


void InformationItem::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    if(lineFlag){
        painter.setPen("#C7C5C6");
    }else{
      painter.setPen(QColor(139,139,139));
    }
    painter.drawLine(5,0,this->width()-1,0);
    painter.drawLine(5,0,5,this->height()-1);
    painter.drawLine(this->width()-1,0,this->width()-1,this->height()-1);
    painter.drawLine(5,this->height()-1,this->width()-1,this->height()-1);


}

void InformationItem::resetColor(){
    this->setStyleSheet("color:#C7C5C6");
    lineFlag =1;
    this->update();
}
