#ifndef SHAREDIALOG_H
#define SHAREDIALOG_H

#include <QDialog>
#include <QClipboard>
#include <QMessageBox>
#include <QPainter>
#include <QWidget>
#include <QFileDialog>
#include  <QScreen>
#include <QMouseEvent>
#include <QtMath>
#include <QScrollBar>

namespace Ui {
class shareDialog;
}
extern QString file_id;
class shareDialog : public QDialog
{
    Q_OBJECT

public:
    explicit shareDialog(QWidget *parent = nullptr);
    ~shareDialog();
    void paintQR(QPainter &painter,const QSize sz, const QString &data, QColor fg);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_copy_clicked();

    void on_pushButton_sav_clicked();

    void on_pushButtonclos_clicked();

    void on_pushButtonclos2_clicked();
protected:
    QPoint move_point;                                    //移动的距离
    bool mouse_press;                                    //鼠标按下
    void mousePressEvent(QMouseEvent *qevent);            //鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *qevent);         //鼠标释放事件
    void mouseMoveEvent(QMouseEvent *qevent);             //鼠标移动事件
    void paintEvent(QPaintEvent *event);
private:
    Ui::shareDialog *ui;
};

#endif // SHAREDIALOG_H
