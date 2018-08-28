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
#include <QProgressBar>
class DecryptionItem : public QWidget
{
    Q_OBJECT
public:
    explicit DecryptionItem(QWidget *parent = nullptr);
    QSize sizeHint() const{
        return QSize(644,71);
    }
    QLabel *label;
    QLabel *elseLabel;

    QLabel *fileName;
    QLabel *fileSize;
    QLabel *fileDescription;
    QLabel *fileIcon;
    QCheckBox *checkBox;
    QPushButton *downloadBtn;
    QHBoxLayout *hbox1;
    QLabel *timeLabel;
    QProgressBar *progressBar;

signals:

public slots:
    void changeCheckBox();
private:
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // DECRYPTIONITEM_H
