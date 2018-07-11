#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <connectsql.h>
class encryption
{

public:
    QString originalFileName;
    QString originalFilePath;
    double originalFileSize;

    //用户唯一标识
    QString userID;

public:
    encryption();
    void connect();
    void encrypt();
};

#endif // ENCRYPTION_H
