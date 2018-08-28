#ifndef FRIENDINPUTDLG_H
#define FRIENDINPUTDLG_H
#include <QLabel>
#include <QPushButton>
#include <QDialog>
#include <QLineEdit>
#include <QPainter>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include<QtSql/QSqlQuery>
#include <connectionpool.h>
#include <QCloseEvent>
#include <QMessageBox>
#include"msgbox.h"
class friendInputDlg : public QDialog
{
    Q_OBJECT
public:
    friendInputDlg(QWidget* parent= 0);
    QSize sizeHint() const{
        return QSize(500,400);
    }
    QLabel *titleLabel;
    QLineEdit *inputLineEdit;
    QPushButton *okBtn;
    QPushButton *cancelBtn;
    QSqlDatabase db;
    QLabel *userHead;
    QLabel *userName;
    QLabel *userPhone;
    QLabel *userTitle;
    QLabel *userId;
    QPushButton *addFriendBtn;
private slots:
    void sendName();
    void cancel();
    void ShowDetail();
    void SearchFriend();
signals:
    void sendNameToMain(QString name);

private:
protected:
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
};

#endif // FRIENDINPUTDLG_H
