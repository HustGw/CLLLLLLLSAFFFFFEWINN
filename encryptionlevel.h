#ifndef ENCRYPTIONLEVEL_H
#define ENCRYPTIONLEVEL_H
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
#include <QDebug>
class encryptionLevel : public QDialog
{
    Q_OBJECT
public:
    encryptionLevel(QWidget* parent= nullptr);
    ~encryptionLevel(void);
public:
    QPushButton *okBtn;
    QPushButton *cancleBtn;
    QPushButton *closeBtn;
    QPushButton *deleteBtn;
    QPushButton *weakBtn;
    QPushButton *midBtn;
    QPushButton *hardBtn;
    QLabel *weakLabel;
    QLabel *midLabel;
    QLabel *hardLabel;
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
    void weakBtn_press();
    void midBtn_press();
    void hardBtn_press();
};


#endif // ENCRYPTIONLEVEL_H
