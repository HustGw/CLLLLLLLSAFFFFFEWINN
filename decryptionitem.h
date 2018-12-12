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
#include <QDebug>
class DecryptionItem : public QWidget
{
    Q_OBJECT
public:
    explicit DecryptionItem(QWidget *parent = nullptr);
    QSize sizeHint() const{
        return QSize(687,71);
    }
    QLabel *label;
    QLabel *elseLabel;
    QLabel *background;
    QLabel *fileName;
    QLabel *fileSize;
    QLabel *fileDescription;
    QLabel *fileIcon;
    QLabel *fileLevel;
    QCheckBox *checkBox;
    QPushButton *downloadBtn;
    QHBoxLayout *hbox1;
    QLabel *timeLabel;
    int flag;
    QProgressBar *progressBar;
    double down_time;
    double dec_time;


signals:

public slots:
    void changeCheckBox();
    void onStateChanged(int);
private:
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // DECRYPTIONITEM_H
