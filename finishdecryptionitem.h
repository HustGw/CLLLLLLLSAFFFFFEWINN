#ifndef FINISHDECRYPTION_H
#define FINISHDECRYPTION_H

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
#include"msgbox.h"

extern QString User_qqNum;
class FinishDecryptionItem : public QWidget
{
    Q_OBJECT
public:
    explicit FinishDecryptionItem(QWidget *parent = nullptr);
    QSize sizeHint() const{
        return QSize(695,71);
    }
    QLabel *background;
    QLabel *label;
    QLabel *elseLabel;
    QLabel *fileName;
    QLabel *fileSize;
    QLabel *fileDescription;
    QLabel *fileIcon;
    QLabel *fileLevel;
    QCheckBox *checkBox;
    QSqlDatabase db;
    QPushButton *openBtn;
    QPushButton *pathOpenBtn;
    QPushButton *deleteBtn;
    QLabel *timeLabel;
    QHBoxLayout *hbox1;
signals:

public slots:
    void on_openBtn_clicked();
    void on_pathOpenBtn_clicked();
    void onStateChanged(int);
    void changeBackground();

private:
protected:
    void paintEvent(QPaintEvent *event);
};


#endif // FINISHDECRYPTION_H
