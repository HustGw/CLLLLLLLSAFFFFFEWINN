#ifndef ENCRYPTIONVIEWCONTROLLER_H
#define ENCRYPTIONVIEWCONTROLLER_H

#include <QWidget>
#include <encryptionitem.h>
#include <QVBoxLayout>
#include <QScrollArea>

class EncryptionViewController : public QWidget
{
    Q_OBJECT
public:
    explicit EncryptionViewController(QWidget *parent = nullptr);
    QVBoxLayout *vbox;          //用于放置Item的Layout
    QVBoxLayout *scrollLayout; //用于放置scrollArea的layout
    QScrollArea *scrollArea; //滚动槽
signals:

public slots:
};

#endif // ENCRYPTIONVIEWCONTROLLER_H
