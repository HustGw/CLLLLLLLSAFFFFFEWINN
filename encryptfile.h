#ifndef ENCRYPTFILE_H
#define ENCRYPTFILE_H
#include <QTextStream>

class encryptfile
{
public:
    encryptfile();
    int encryptFile(QString fileAbPath, QString ykeyAbPath, QString yzipAbPath, int percent, QString file_id, QString user_identify);
};

#endif // ENCRYPTFILE_H
