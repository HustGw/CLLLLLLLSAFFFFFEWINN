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

    int VerifyFile(QString yzipAbPath, char v_dest[], char f_dest[], char u_dest[]);

    int decryptFile(QString ykeyAbPath, QString yzipAbPath, QString abPath, int decryptRate);
};

#endif // DECRYPTIONFILE_H
