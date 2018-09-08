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
#include <QtNetwork>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

extern int AddFriendFlag;

class friendInputDlg : public QDialog
{
    Q_OBJECT
public:
    friendInputDlg(QWidget* parent= 0);
    QSize sizeHint() const{
        return QSize(512,412);
    }
    QLabel *titleLabel;
    QLineEdit *inputLineEdit;
    QPushButton *okBtn;
    QPushButton *cancelBtn;
    QSqlDatabase db;
    QLabel *userHead;
    QLabel *userName;
   // QLabel *userPhone;
    QLabel *userTitle;
    QLabel *userId;
    QPushButton *addFriendBtn;
    QWidget *topWidget;
    QWidget *bottomWidget;
    QLabel *titleIcon;
    QLabel * titleText;
    QPushButton *closeBtn;
private slots:
    void sendName();
    void cancel();
    void ShowDetail();
    void SearchFriend();
    void closeBtn_press();
    void finishedSlot(QNetworkReply *reply);
signals:
    void sendNameToMain(QString name);

private:
    QPoint mLastMousePosition;
    bool mMoving;
protected:
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
};

#endif // FRIENDINPUTDLG_H
