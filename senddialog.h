#ifndef SENDDIALOG_H
#define SENDDIALOG_H

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
#include <QWidget>

namespace Ui {
class sendDialog;
}
extern QString file_id;
extern QString user_id;
class sendDialog : public QDialog
{
    Q_OBJECT

public:
    explicit sendDialog(QWidget *parent = 0);
    ~sendDialog();
    QSqlDatabase db1;
    void getCheckedItems();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void reciveData(QString Data);
    void reciveUserId(QString data);
protected:
    void closeEvent(QCloseEvent *);
private:
    Ui::sendDialog *ui;
};

#endif // SENDDIALOG_H
