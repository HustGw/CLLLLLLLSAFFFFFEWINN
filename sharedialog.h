#ifndef SHAREDIALOG_H
#define SHAREDIALOG_H

#include <QDialog>
#include <QClipboard>
#include <QMessageBox>

namespace Ui {
class shareDialog;
}
extern QString file_id;
class shareDialog : public QDialog
{
    Q_OBJECT

public:
    explicit shareDialog(QWidget *parent = 0);
    ~shareDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::shareDialog *ui;
};

#endif // SHAREDIALOG_H
