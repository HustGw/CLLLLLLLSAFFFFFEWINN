#ifndef INFORMATIONDLG_H
#define INFORMATIONDLG_H

#include <QLabel>
#include <QPushButton>
#include <QDialog>
#include <QLineEdit>
#include "informationitem.h"
#include <QVBoxLayout>
#include <QScrollArea>
#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QtSql/QSqlError>
#include <synchapi.h>
#include <QtSql/QSqlRecord>
#include<QtSql/QSqlQuery>
#include <connectionpool.h>
#include <QString>
#include <QMessageBox>
extern QString infor_User_ID;
class informationDlg : public QDialog
{
    Q_OBJECT
public:
    informationDlg(QWidget* parent = 0);
    QSize sizeHint() const{
        return QSize(600,400);
    }
    QVBoxLayout *vbox;//用于放置Item的Layout
    QVBoxLayout *scrollLayout;//用于放置scrollArea的Layout
    QScrollArea *scrollArea;
    QWidget *ItemWidget;
    QLabel *titleLabel;
    QSqlDatabase db;
    void setItem();
private:

private slots:
    void setUserID(QString user_id);
    void recvReq();
    void ignoreReq();
    void newInformation();
signals:
};

#endif // INFORMATIONDLG_H
