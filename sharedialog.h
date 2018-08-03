#ifndef SHAREDIALOG_H
#define SHAREDIALOG_H

#include <QDialog>
#include <QClipboard>
#include <QMessageBox>
#include <QPainter>
#include <QWidget>
#include <QFileDialog>
#include  <QScreen>
namespace Ui {
class shareDialog;
}
extern QString file_id;
class shareDialog : public QDialog
{
    Q_OBJECT

public:
    explicit shareDialog(QWidget *parent = 0);
    ~shareDialog();
    void paintEvent(QPaintEvent *event);
    void paintQR(QPainter &painter,const QSize sz, const QString &data, QColor fg);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::shareDialog *ui;
};

#endif // SHAREDIALOG_H
