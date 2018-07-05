#ifndef ENCRYPTIONITEM_H
#define ENCRYPTIONITEM_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QHBoxLayout>

class EncryptionItem : public QWidget
{
    Q_OBJECT
public:
    explicit EncryptionItem(QWidget *parent = nullptr);
    QSize sizeHint() const{
        return QSize(200,50);
    }
signals:

public slots:
private:
    QLabel *fileName;
    QLabel *fileSize;
    QLabel *fileDescription;
    QLabel *fileIcon;
    QCheckBox *checkBox;
    QPushButton *downloadBtn;
    QHBoxLayout *hbox1;
};

#endif // ENCRYPTIONITEM_H
