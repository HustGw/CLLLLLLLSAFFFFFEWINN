#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QtNetwork>
#include <QMessageBox>
#include <QNetworkReply>



namespace Ui {
class registerDialog;
}

class registerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit registerDialog(QWidget *parent = 0);
    ~registerDialog();
    int flag;  //步骤

private slots:
    void on_signBtn_clicked();

    void on_codeBtn_clicked();
    void finishedSlot(QNetworkReply *reply);

private:
    Ui::registerDialog *ui;
};

#endif // REGISTERDIALOG_H
