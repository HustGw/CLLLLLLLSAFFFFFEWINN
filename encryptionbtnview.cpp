#include "encryptionbtnview.h"

EncryptionBtnView::EncryptionBtnView(QWidget *parent) : QWidget(parent)
{
    allSel = new QLabel(tr("全选"));
    openDic = new QPushButton("调取本地");
    hbox = new QHBoxLayout();
    hbox->addWidget(allSel);
    hbox->addWidget(openDic);
    this->setLayout(hbox);
}
