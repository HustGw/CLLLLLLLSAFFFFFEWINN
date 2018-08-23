#ifndef RESETDIALOG_H
#define RESETDIALOG_H

#include <QDialog>
#include <QtNetwork>
#include <QMessageBox>
#include <QNetworkReply>

namespace Ui {
class resetDialog;
}

class resetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit resetDialog(QWidget *parent = 0);
    ~resetDialog();
    int flag;  //步骤

private slots:
    void on_codeBtn_clicked();
    void on_confirmBtn_clicked();

    void finishedSlot(QNetworkReply *reply);

private:
    Ui::resetDialog *ui;
};

#endif // RESETDIALOG_H
