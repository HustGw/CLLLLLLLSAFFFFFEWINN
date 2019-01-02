#ifndef ENCRYPTFILE_H
#define ENCRYPTFILE_H
#include <QTextStream>
#include <QTextCodec>
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <QDebug>

class encryptfile
{
public:
    encryptfile();

    int encryptFile(QString fileAbPath, QString ykeyAbPath, QString yzipAbPath, int percent, QString version,QString file_id, QString user_identify, int encryptRate);

    //string qstr2str(const QString qstr);
};

#endif // ENCRYPTFILE_H
