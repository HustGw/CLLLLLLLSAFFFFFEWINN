#ifndef NEWDOWNLOADDIALOG_H
#define NEWDOWNLOADDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QClipboard>
#include <QMessageBox>
#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QtMath>
#include <QStringList>
#include <QListWidget>
#include <QListWidgetItem>
#include <connectionpool.h>
#include <QCloseEvent>
namespace Ui {
class newDownloadDialog;
}

class newDownloadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit newDownloadDialog(QWidget *parent = nullptr);
    ~newDownloadDialog();
    QSqlDatabase db1;
    bool isOpened = false;
    QPushButton *okBtn;
    QPushButton *cancleBtn;
    QPushButton *closeBtn;
protected:
    QPoint move_point;                                    //移动的距离
    bool mouse_press;                                    //鼠标按下
    void mousePressEvent(QMouseEvent *qevent);            //鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *qevent);         //鼠标释放事件
    void mouseMoveEvent(QMouseEvent *qevent);             //鼠标移动事件
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
private slots:
    void on_pushButtonconfirm_clicked();

    void on_pushButtoncancel_clicked();

    void on_pushButton_close_clicked();

    void change_list_view();
private:
    Ui::newDownloadDialog *ui;
    void okBtn_press();
    void cancleBtn_press();
    void closeBtn_press();
signals:
    void downloadOne(QString id);
};

#endif // NEWDOWNLOADDIALOG_H
