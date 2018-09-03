#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QtNetwork>
#include <QMessageBox>
#include <QNetworkReply>
#include <QMouseEvent>
#include <QtMath>
#include <QPainter>

#include "msgbox.h"



namespace Ui {
class registerDialog;
}

class registerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit registerDialog(QWidget *parent = 0);
    ~registerDialog();
    int flag;  //步骤
    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;

private slots:
    void on_closeBtn_clicked();

//    void on_userLineEdit_textChanged();

//    void on_userLineEdit_editingFinished();

//    void on_passwardLineEdit_textChanged();

//    void on_passwardLineEdit_editingFinished();

//    void on_passwardLineEdit_2_textChanged();

//    void on_passwardLineEdit_2_editingFinished();

//    void on_nichengLineEdit_textChanged();

//    void on_nichengLineEdit_editingFinished();

//    void on_email_textChanged();

//    void on_email_editingFinished();

//    void on_code_textChanged();

//    void on_code_editingFinished();


    void on_signBtn_clicked();

    void on_codeBtn_clicked();
    void finishedSlot(QNetworkReply *reply);
protected:
    QPoint move_point;                                    //移动的距离
    bool mouse_press;                                    //鼠标按下
    void mousePressEvent(QMouseEvent *qevent);            //鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *qevent);         //鼠标释放事件
    void mouseMoveEvent(QMouseEvent *qevent);             //鼠标移动事件
    void paintEvent(QPaintEvent *event);
private:
    Ui::registerDialog *ui;
};

#endif // REGISTERDIALOG_H
