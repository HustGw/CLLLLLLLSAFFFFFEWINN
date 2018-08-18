#include "encryptionviewcontroller.h"

EncryptionViewController::EncryptionViewController(QWidget *parent) : QWidget(parent)
{

    //新建一个Widget用于存放vbox  vbox插入Item
      QWidget *ItemWidget = new QWidget(this);
      vbox = new QVBoxLayout();
//    vbox->addWidget(item1);
    ItemWidget->setLayout(vbox);
    scrollArea = new QScrollArea(this);
    scrollArea->setStyleSheet("border:0;padding:0;spacing:0;");
    scrollArea->setWidget(ItemWidget);
    scrollLayout = new QVBoxLayout(this);
    scrollLayout->addWidget(scrollArea);
    this->setLayout(scrollLayout);

}
