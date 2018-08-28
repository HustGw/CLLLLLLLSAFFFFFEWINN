#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QtNetwork>
#include <QMessageBox>
#include <QNetworkReply>
#include "msgbox.h"



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
    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;

private slots:
    void on_closeBtn_clicked();

//    void on_userLineEdit_textChanged();

//    void on_userLineEdit_editingFinished();

//    void on_passwardLineEdit_textChanged();

//    void on_passwardLineEdit_editingFinished();

//    void on_passwardLineEdit_2_textChanged();

//    void on_passwardLineEdit_2_editingFinished();

//    void on_nichengLineEdit_textChanged();

//    void on_nichengLineEdit_editingFinished();

//    void on_email_textChanged();

//    void on_email_editingFinished();

//    void on_code_textChanged();

//    void on_code_editingFinished();

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void on_signBtn_clicked();

    void on_codeBtn_clicked();
    void finishedSlot(QNetworkReply *reply);

private:
    Ui::registerDialog *ui;
};

#endif // REGISTERDIALOG_H
