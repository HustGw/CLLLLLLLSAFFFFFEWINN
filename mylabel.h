#ifndef MYLABEL_H
#define MYLABEL_H
#include <QWidget>
#include <QLabel>
class Mylabel:public QLabel
{
    Q_OBJECT
public:
    explicit Mylabel(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *event);
private slots:
signals:
    void LabelClicked();
};

#endif // MYLABEL_H
