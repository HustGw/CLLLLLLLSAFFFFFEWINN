#include "decryptionbtnview.h"

DecryptionBtnView::DecryptionBtnView(QWidget *parent) : QWidget(parent)
{
    allSel = new QLabel(tr("全选"));
    deleteBtn = new QPushButton(tr("批量删除"));
}
