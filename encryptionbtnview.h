#ifndef ENCRYPTIONBTNVIEW_H
#define ENCRYPTIONBTNVIEW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

class EncryptionBtnView : public QWidget
{
    Q_OBJECT
public:
    explicit EncryptionBtnView(QWidget *parent = nullptr);

signals:

public slots:
private:
    QLabel *allSel;
    QPushButton *openDic;
    QHBoxLayout *hbox;

};

#endif // ENCRYPTIONBTNVIEW_H
