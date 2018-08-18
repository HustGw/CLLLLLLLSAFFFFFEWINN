#include "finishviewcontroller.h"

FinishViewController::FinishViewController(QWidget *parent) : QWidget(parent)
{
    //新建一个Widget用于存放vbox  vbox插入Item
    ItemWidget = new QWidget(this);
    vbox = new QVBoxLayout();

    ItemWidget->setLayout(vbox);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(ItemWidget);
    scrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
    scrollLayout = new QVBoxLayout(this);
    scrollLayout->addWidget(scrollArea);
    this->setLayout(scrollLayout);

}
