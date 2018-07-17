#include "finishviewcontroller2.h"

FinishViewController2::FinishViewController2(QWidget *parent) : QWidget(parent)
{
    //新建一个Widget用于存放vbox  vbox插入Item
    ItemWidget = new QWidget(this);
    vbox = new QVBoxLayout();

    ItemWidget->setLayout(vbox);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(ItemWidget);

    scrollLayout = new QVBoxLayout(this);
    scrollLayout->addWidget(scrollArea);
    this->setLayout(scrollLayout);

}
