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
#include "senddialog.h"
#include "sharedialog.h"
#include"msgbox.h"
extern QString file_id;
extern QString User_qqNum;
class FinishEncryptionItem : public QWidget
{
    Q_OBJECT
public:
    explicit FinishEncryptionItem(QWidget *parent = nullptr);
    QSize sizeHint() const{
        return QSize(695,71);
    }
    QLabel *label;
    QLabel *elseLabel;
    QLabel *fileName;
    QLabel *fileSize;
    QLabel *fileDescription;
    QLabel *fileIcon;
    QCheckBox *checkBox;
    QPushButton *transprotBtn;
    QSqlDatabase db;
    QPushButton *shareBtn;
    QPushButton *pathOpenBtn;
    QPushButton *deleteBtn;
    QHBoxLayout *hbox1;
    sendDialog *dlg;
    shareDialog *share;
    QLabel *timeLabel;
signals:
    void fileData(QString);
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
