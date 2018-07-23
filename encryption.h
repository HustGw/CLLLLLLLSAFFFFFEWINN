#ifndef ENCRYPTION_H
#define ENCRYPTION_H


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

class encryption
{

public:
    QString originalFileName;
    QString originalFilePath;
    double originalFileSize;

    //用户唯一标识
    QString userID;
    //文件信息
    QFileInfo fInfo;
    EncryptionViewController *encryptionViewController ;

public:
    encryption();
    void connect();
    void encrypt();
    void drawItem();
};

#endif // ENCRYPTION_H
