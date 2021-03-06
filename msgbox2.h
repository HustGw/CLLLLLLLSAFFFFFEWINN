#ifndef MSGBOX2_H
#define MSGBOX2_H
#include <QLabel>
#include <QPushButton>
#include <QDialog>
#include <QLineEdit>
#include <QClipboard>
#include <QMessageBox>
#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QtMath>
#include <QFontDatabase>
#include <QFont>
#include <QCoreApplication>
#include <QStringList>
#include <QList>


class MsgBox2 : public QDialog
{
    Q_OBJECT
public:
    MsgBox2(int style,QString text,QWidget* parent= nullptr);
    ~MsgBox2(void);
public:
    QPushButton *okBtn;
    QPushButton *cancleBtn;
    QPushButton *closeBtn;
    QPushButton * msgBtn;
    QPushButton *deleteBtn;
    QLabel * titleLabel;
    QLabel *askLabel;
    QLabel * titleText;
    QLabel *titleIcon;
protected:
    QPoint move_point;                                    //移动的距离
    bool mouse_press;                                    //鼠标按下
    void mousePressEvent(QMouseEvent *qevent);            //鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *qevent);         //鼠标释放事件
    void mouseMoveEvent(QMouseEvent *qevent);             //鼠标移动事件
    void paintEvent(QPaintEvent *event);
public slots:
    void okBtn_press();
    void cancleBtn_press();
    void closeBtn_press();
    void deletBtn_press();
};
#endif // MSGBOX2_H
