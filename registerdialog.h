#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

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
#include <QList>

//#include "msgbox.h"



namespace Ui {
class registerDialog;
}

extern QStringList m_fontList_;

class registerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit registerDialog(QWidget *parent = 0);
    ~registerDialog();
    int flag;  //步骤
    int a;
    int b;
    QTimer *timer1;


private slots:
    void on_closeBtn_clicked();
    void on_passwardLineEdit_textChanged();
    void on_passwardLineEdit_editingFinished();
    void on_passwardLineEdit_2_textChanged();
    void on_passwardLineEdit_2_editingFinished();
    void on_signBtn_clicked();
    void on_codeBtn_clicked();
    void finishedSlot(QNetworkReply *reply);
    void showTimelimit();
//    void on_nichengLineEdit_editingFinished();
    void on_nichengLineEdit_textChanged();
protected:
    QPoint move_point;                                    //移动的距离
    bool mouse_press;                                    //鼠标按下
    void mousePressEvent(QMouseEvent *qevent);            //鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *qevent);         //鼠标释放事件
    void mouseMoveEvent(QMouseEvent *qevent);             //鼠标移动事件
private:
    Ui::registerDialog *ui;
};

#endif // REGISTERDIALOG_H
