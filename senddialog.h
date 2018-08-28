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
#include <QMouseEvent>
#include <QPainter>
#include <QtMath>

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
    void onStageChanged(int stage);
    void reciveData(QString Data);
    void reciveUserId(QString data);
    void on_pushButton_trans_clicked();

    void on_pushButton_close2_clicked();

    void on_pushButton_close_clicked();

protected:
    QPoint move_point;                                    //移动的距离
    bool mouse_press;                                    //鼠标按下
    void mousePressEvent(QMouseEvent *qevent);            //鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *qevent);         //鼠标释放事件
    void mouseMoveEvent(QMouseEvent *qevent);             //鼠标移动事件
    void closeEvent(QCloseEvent *);
    void paintEvent(QPaintEvent *event);
private:
    Ui::sendDialog *ui;
};

#endif // SENDDIALOG_H
