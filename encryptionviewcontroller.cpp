#include "encryptionviewcontroller.h"

EncryptionViewController::EncryptionViewController(QWidget *parent) : QWidget(parent)
{
//    EncryptionItem *item1 = new EncryptionItem();
//    item1->fileName->setText(tr("春游1.jpg"));
//    item1->fileSize->setText(tr("15KB"));
//    item1->fileDescription->setText("主体文件指定分享");
//    item1->downloadBtn->setText("确认下载");
//    item1->fileIcon->setText(tr("图标"));
    //新建一个Widget用于存放vbox  vbox插入Item
      QWidget *ItemWidget = new QWidget(this);
      vbox = new QVBoxLayout();
//    vbox->addWidget(item1);

    ItemWidget->setLayout(vbox);
    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(ItemWidget);
    scrollLayout = new QVBoxLayout(this);
    scrollLayout->addWidget(scrollArea);
    this->setLayout(scrollLayout);

}
