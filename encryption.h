#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include<QObject>
#include <connectsql.h>
#include <encryptfile.h>
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QtSql/QSqlError>
#include <QUuid>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include "uploadoss.h"
#include "QString"
#include "QDir"
#include <time.h>
#include <stdlib.h>
#include <qdatetime.h>
#include "encryptionviewcontroller.h"
#include "encryptionitem.h"
#include <QTextStream>
#include <QTextCodec>
#include "connectionpool.h"
#include "QMessageBox"
#include "QtWidgets"
#include <msgbox.h>
#include <QTime>
#include "QThread"
#include "startencryptthread.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include "msgbox.h"
#include "msgbox2.h"

class encryption: public QObject
{
    Q_OBJECT

public:
    explicit encryption(QObject* parent = 0);
    QString originalFileName;
    QString originalFilePath;
    double originalFileSize;

    //用户唯一标识
    QString userID;
    //文件信息
    QFileInfo fInfo;
    EncryptionViewController *encryptionViewController ;
    QSqlDatabase e_conn ;
    int oss_PutKey_Flag;
    int oss_PutFile_Flag;
    double debugTime,uploadTime;
    int encptRate;
    QNetworkAccessManager *m_accessManager;


public:
    encryption();
    //void connect();
    int encrypt();
    void drawItem();

private slots:
    void finishedSlot(QNetworkReply *reply);
};

#endif // ENCRYPTION_H
