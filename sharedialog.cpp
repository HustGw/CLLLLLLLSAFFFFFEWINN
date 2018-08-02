#include "sharedialog.h"
#include "ui_sharedialog.h"
#include "finishencryptionitem.h"

extern QString file_id;

shareDialog::shareDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::shareDialog)
{
    ui->setupUi(this);
    QString fake_address = "https://youxinzhongwang.com/theRea1Fi1eId&&"+file_id;
    ui->textEdit->setPlainText(fake_address);
    ui->textEdit->setReadOnly(true);
}

shareDialog::~shareDialog()
{
    delete ui;
}

void shareDialog::on_pushButton_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->textEdit->toPlainText());
    QMessageBox::information(this,"提示","拷贝成功");
}
