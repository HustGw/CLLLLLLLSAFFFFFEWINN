#ifndef DECRYPTIONFILE_H
#define DECRYPTIONFILE_H
#include <QTextStream>
#include <QTextCodec>
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <QDebug>

class DecryptionFile
{
public:
    DecryptionFile();
    int decryptFile(QString ykeyAbPath, QString yzipAbPath, QString abPath);
};

#endif // DECRYPTIONFILE_H
