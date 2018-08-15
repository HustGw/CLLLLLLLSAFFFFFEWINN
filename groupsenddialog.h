#ifndef GROUPSENDDIALOG_H
#define GROUPSENDDIALOG_H

#include <QDialog>
#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLabel>
#include <connectionpool.h>
#include <QCloseEvent>
#include <QCheckBox>

extern QString file_id_list;
extern QString user_id;
namespace Ui {
class groupSendDialog;
}

class groupSendDialog : public QDialog
{
    Q_OBJECT

public:
    explicit groupSendDialog(QWidget *parent = 0);
    ~groupSendDialog();
    QSqlDatabase db1;
    void getCheckedItems();

private slots:
    void on_pushButton_clicked();
    void onStageChanged(int stage);
protected:
    void closeEvent(QCloseEvent *);
private:
    Ui::groupSendDialog *ui;
};

#endif // GROUPSENDDIALOG_H
