#ifndef ENCRYPTIONBTNVIEW_H
#define ENCRYPTIONBTNVIEW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QSize>
#include <QLine>
#include <QPainter>
#include "QProgressBar"
#include "enitemthread.h"
#include <QPushButton>
#include <qdatetime.h>

class EncryptionBtnView : public QWidget
{
    Q_OBJECT
public:
    explicit EncryptionBtnView(QWidget *parent = nullptr);

signals:

public slots:
public:
    QLabel *allSel;
    QPushButton *openDic;
    QPushButton *selectAllBtn_ept;
    QHBoxLayout *hbox;

};

#endif // ENCRYPTIONBTNVIEW_H
