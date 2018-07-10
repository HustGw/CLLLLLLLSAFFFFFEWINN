#ifndef DECRYPTIONITEM_H
#define DECRYPTIONITEM_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QSize>
#include <QLine>
#include <QPainter>
class DecryptionItem : public QWidget
{
    Q_OBJECT
public:
    explicit DecryptionItem(QWidget *parent = nullptr);
    QSize sizeHint() const{
        return QSize(660,120);
    }

    QLabel *fileName;
    QLabel *fileSize;
    QLabel *fileDescription;
    QLabel *fileIcon;
    QCheckBox *checkBox;
    QPushButton *downloadBtn;
    QHBoxLayout *hbox1;

signals:

public slots:
    void changeCheckBox();
private:
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // DECRYPTIONITEM_H
