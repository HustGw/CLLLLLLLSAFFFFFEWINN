#ifndef DECRYPTIONBTNVIEW_H
#define DECRYPTIONBTNVIEW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
class DecryptionBtnView : public QWidget
{
    Q_OBJECT
public:
    explicit DecryptionBtnView(QWidget *parent = nullptr);

signals:

public slots:
private:
    QLabel *allSel;
    QPushButton *deleteBtn;
    QPushButton *shareBtn;
    QHBoxLayout *hbox;
};

#endif // DECRYPTIONBTNVIEW_H
