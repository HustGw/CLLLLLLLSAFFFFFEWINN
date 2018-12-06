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
#include <QPropertyAnimation>
#include "enitemthread.h"
#include <QPushButton>
#include <qdatetime.h>
#include "QDir"

class EncryptionItem : public QWidget
{
    Q_OBJECT
public:
    explicit EncryptionItem(QWidget *parent = nullptr);
    QSize sizeHint() const{
        return QSize(695,71);
    }

    QLabel *fileName;
    QLabel *fileSize;
    QLabel *fileDescription;
    QLabel *fileIcon;
    QLabel *timeInfo;
    QLabel *line;
    //QCheckBox *checkBox;
    //QPushButton *downloadBtn;
    QHBoxLayout *hbox1;
    QProgressBar *progressBar = new QProgressBar(this);
    QPropertyAnimation *m_proAnimation;

    QPushButton *encryptStaBtn;
    QPushButton *starEncptBtn;
    QLabel *initPage;

    QFileInfo fInfo;
signals:
    //void starEcptItem(QString itemName);

public slots:
    //void on_encryptStaBtn_clicked();
//    // 更新进度
//        void handleResults(int value);
//        // 开启线程
//        void startThread();
private:
       // enItemThread enItem_Thread;
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // ENCRYPTIONITEM_H
