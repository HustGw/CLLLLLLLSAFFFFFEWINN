#include "encryptionitem.h"

EncryptionItem::EncryptionItem(QWidget *parent) : QWidget(parent)
{
    fileName = new QLabel(tr("加密界面"));
    fileSize = new QLabel(tr("15KB"));
    fileDescription = new QLabel(tr("请调取本地文件进行加密"));
    downloadBtn = new QPushButton(tr("确认下载"));
    hbox1 = new QHBoxLayout();
    hbox1->addWidget(fileName);
    hbox1->addWidget(fileSize);
    hbox1->addWidget(fileDescription);
    hbox1->addWidget(downloadBtn);
    this->setLayout(hbox1);
}
