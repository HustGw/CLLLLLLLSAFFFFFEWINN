#ifndef DELINKDIALOG_H
#define DELINKDIALOG_H
#include <QLabel>
#include <QPushButton>
#include <QDialog>
#include <QLineEdit>
#include <QClipboard>
#include <QMessageBox>
#include <QApplication>
#include <qt_windows.h>
#include <QMouseEvent>
#include <QPainter>
#include <QtMath>
extern int LinkInsertFlag;
class DelinkDialog : public QDialog
{
    Q_OBJECT
public:
    DelinkDialog(QWidget* parent= nullptr);
    QSize sizeHint() const{
        return QSize(300,150);
    }
    QLabel * titleLabel;
    QLabel *askLabel;
    QLabel * titleText;
    QLabel *titleIcon;
    QLineEdit *inputLineEdit;
    QPushButton *okBtn;
    QPushButton *cancleBtn;
    QPushButton *closeBtn;
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent * event);
private slots:
    void cancel();
    void sendLink();
    void closeBtn_press();
signals:
    void sendLinkToMain(QString link);
private:
    bool mouse_press;
    QPoint dragPosition;
};

#endif // DELINKDIALOG_H
