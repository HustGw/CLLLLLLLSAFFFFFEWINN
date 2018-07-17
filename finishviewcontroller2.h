#ifndef FINISHVIEWCONTROLLER2_H
#define FINISHVIEWCONTROLLER2_H

#include <QWidget>
#include <decryptionitem.h>
#include <QVBoxLayout>
#include <QScrollArea>

class FinishViewController2 : public QWidget
{
    Q_OBJECT
public:
    explicit FinishViewController2(QWidget *parent = nullptr);
    QVBoxLayout *vbox;          //用于放置Item的Layout
    QVBoxLayout *scrollLayout; //用于放置scrollArea的layout
    QScrollArea *scrollArea; //滚动槽
    QWidget *ItemWidget;
signals:

public slots:
};

#endif // FINISHVIEWCONTROLLER2_H
