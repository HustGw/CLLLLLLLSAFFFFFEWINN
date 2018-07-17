#ifndef FINISHVIEWCONTROLLER_H
#define FINISHVIEWCONTROLLER_H



#include <QWidget>
#include <decryptionitem.h>
#include <QVBoxLayout>
#include <QScrollArea>

class FinishViewController : public QWidget
{
    Q_OBJECT
public:
    explicit FinishViewController(QWidget *parent = nullptr);
    QVBoxLayout *vbox;          //用于放置Item的Layout
    QVBoxLayout *scrollLayout; //用于放置scrollArea的layout
    QScrollArea *scrollArea; //滚动槽
    QWidget *ItemWidget;
signals:

public slots:
};


#endif // FINISHVIEWCONTROLLER_H
