#ifndef DECRYPTIONVIEWCONTROLLER_H
#define DECRYPTIONVIEWCONTROLLER_H

#include <QWidget>
#include <decryptionitem.h>
#include <QVBoxLayout>
#include <QScrollArea>

class DecryptionViewController : public QWidget
{
    Q_OBJECT
public:
    explicit DecryptionViewController(QWidget *parent = nullptr);
    QVBoxLayout *vbox;          //用于放置Item的Layout
    QVBoxLayout *scrollLayout; //用于放置scrollArea的layout
    QScrollArea *scrollArea; //滚动槽
    QWidget *ItemWidget;
signals:

public slots:
};

#endif // DECRYPTIONVIEWCONTROLLER_H
