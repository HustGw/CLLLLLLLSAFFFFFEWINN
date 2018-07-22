#ifndef ENCRYPTIONITEM_H
#define ENCRYPTIONITEM_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QSize>
#include <QLine>
#include <QPainter>
#include "QProgressBar"

class EncryptionItem : public QWidget
{
    Q_OBJECT
public:
    explicit EncryptionItem(QWidget *parent = nullptr);
    QSize sizeHint() const{
        return QSize(660,120);
    }
    QLabel *fileName;
    QLabel *fileSize;
    QLabel *fileDescription;
    QLabel *fileIcon;
    QCheckBox *checkBox;
    //QPushButton *downloadBtn;
    QHBoxLayout *hbox1;
    QProgressBar *progressBar = new QProgressBar(this);
signals:

public slots:
private:
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // ENCRYPTIONITEM_H
