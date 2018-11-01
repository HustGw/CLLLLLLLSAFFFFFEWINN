#ifndef RESETDIALOG_H
#define RESETDIALOG_H

#include <QDialog>
#include <QtNetwork>
#include <QMessageBox>
#include <QNetworkReply>
#include <QMouseEvent>
#include <QtMath>
#include <QPainter>
#include <QTimer>
#include <QFontDatabase>
#include <QFont>
#include <QCoreApplication>
#include <QStringList>

namespace Ui {
class resetDialog;
}

class resetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit resetDialog(QWidget *parent = 0);
    ~resetDialog();
    int flag;  //步骤
    int a;

private slots:
    void on_codeBtn_clicked();
    void on_confirmBtn_clicked();
    void on_passwordLineEdit_textChanged();
    void on_passwordLineEdit_editingFinished();
    void on_passwordLineEdit_2_textChanged();
    void on_passwordLineEdit_2_editingFinished();
    void on_closeBtn_clicked();
    void on_userLineEdit_textChanged();
    void on_userLineEdit_editingFinished();
    void finishedSlot(QNetworkReply *reply);
    void showTimelimit();
protected:
    QPoint move_point;                                    //移动的距离
    bool mouse_press;                                    //鼠标按下
    void mousePressEvent(QMouseEvent *qevent);            //鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *qevent);         //鼠标释放事件
    void mouseMoveEvent(QMouseEvent *qevent);             //鼠标移动事件

private:
    Ui::resetDialog *ui;
};

#endif // RESETDIALOG_H
