#include "friendinputdlg.h"

friendInputDlg::friendInputDlg(QWidget *parent):QDialog(parent)
{
    setWindowTitle(tr("好友添加"));
    titleLabel = new QLabel();
    titleLabel->setText(tr("请输入对方名称"));
    inputLineEdit = new QLineEdit();
   titleLabel->setGeometry(10,10,100,40);
}
