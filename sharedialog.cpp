#include "sharedialog.h"
#include "ui_sharedialog.h"
#include "finishencryptionitem.h"

extern QString file_id;

shareDialog::shareDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::shareDialog)
{
    ui->setupUi(this);
    ui->textEdit->setPlainText(file_id);
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
