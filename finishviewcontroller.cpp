#include "finishviewcontroller.h"

FinishViewController::FinishViewController(QWidget *parent) : QWidget(parent)
{
    //新建一个Widget用于存放vbox  vbox插入Item
    ItemWidget = new QWidget(this);
    ItemWidget->setContentsMargins(0,0,0,0);
    vbox = new QVBoxLayout();
    vbox->setMargin(0);
    vbox->setSpacing(0);
    vbox->setContentsMargins(0,0,0,0);
    ItemWidget->setLayout(vbox);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(ItemWidget);
    scrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
    scrollArea->setContentsMargins(0,0,0,0);
    scrollLayout = new QVBoxLayout(this);
    scrollLayout->setMargin(0);
    scrollLayout->setSpacing(0);
    scrollLayout->addWidget(scrollArea);
    this->setLayout(scrollLayout);

}
