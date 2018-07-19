#ifndef INFORMATIONITEM_H
#define INFORMATIONITEM_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QSize>
#include <QLine>
#include <QPainter>

class InformationItem : public QWidget
{
    Q_OBJECT
public:
    explicit InformationItem(QWidget *parent = nullptr);
    QSize sizeHint() const{
        return QSize(525,120);
    }
    QPushButton *allowBtn;
    QPushButton *ignoreBtn;
    QLabel *titleLabel;
signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // INFORMATIONITEM_H
