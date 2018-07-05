#include "decryptionviewcontroller.h"

DecryptionViewController::DecryptionViewController(QWidget *parent) : QWidget(parent)
{
    DecryptionItem *item1 = new DecryptionItem();
    item1->fileName->setText(tr("春游1.jpg"));
    item1->fileSize->setText(tr("15KB"));
    item1->fileDescription->setText("主体文件指定分享");
    item1->downloadBtn->setText("确认下载");
    item1->fileIcon->setText(tr("图标"));
    DecryptionItem *item2 = new DecryptionItem();
    item2->fileName->setText(tr("春游2.jpg"));
    item2->fileSize->setText(tr("15KB"));
    item2->fileDescription->setText("主体文件指定分享");
    item2->downloadBtn->setText("确认下载");
    item2->fileIcon->setText(tr("图标"));
    DecryptionItem *item3 = new DecryptionItem();
    item3->fileName->setText(tr("春游3.jpg"));
    item3->fileSize->setText(tr("15KB"));
    item3->fileDescription->setText("主体文件指定分享");
    item3->downloadBtn->setText("确认下载");
    item3->fileIcon->setText(tr("图标"));
    DecryptionItem *item4 = new DecryptionItem();
    item4->fileName->setText(tr("春游4.jpg"));
    item4->fileSize->setText(tr("15KB"));
    item4->fileDescription->setText("主体文件指定分享");
    item4->downloadBtn->setText("确认下载");
    item4->fileIcon->setText(tr("图标"));
    //新建一个Widget用于存放vbox  vbox插入Item
    QWidget *ItemWidget = new QWidget(this);
    vbox = new QVBoxLayout();
    vbox->addWidget(item1);
    vbox->addWidget(item2);
    vbox->addWidget(item3);
    vbox->addWidget(item4);
    ItemWidget->setLayout(vbox);
    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(ItemWidget);
    scrollLayout = new QVBoxLayout(this);
    scrollLayout->addWidget(scrollArea);
    this->setLayout(scrollLayout);


}
