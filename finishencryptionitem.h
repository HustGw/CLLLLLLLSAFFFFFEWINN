#ifndef FINISHENCRYPTIONVIEW_H
#define FINISHENCRYPTIONVIEW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QSize>
#include <QLine>
#include <QPainter>
#include <QDesktopServices>
#include <QUrl>
#include <qdebug.h>
#include "connectionpool.h"
#include <QMessageBox>

class FinishEncryptionItem : public QWidget
{
    Q_OBJECT
public:
    explicit FinishEncryptionItem(QWidget *parent = nullptr);
    QSize sizeHint() const{
        return QSize(660,120);
    }

    QLabel *fileName;
    QLabel *fileSize;
    QLabel *fileDescription;
    QLabel *fileIcon;
    QCheckBox *checkBox;
    QPushButton *transprotBtn;
    QPushButton *shareBtn;
    QPushButton *pathOpenBtn;
    QPushButton *deleteBtn;
    QHBoxLayout *hbox1;
signals:

public slots:
    void on_transprotBtn_clicked();
    void on_shareBtn_clicked();
    void on_pathOpenBtn_clicked();
   // void on_deleteBtn_clicked();
private:
protected:
    void paintEvent(QPaintEvent *event);
};


#endif // FINISHENCRYPTIONVIEW_H
