#include "encryptfile.h"
#include <QFile>

#define BUFFER_SIZE 52428800
#define MAX_FILE_ADDRESS_LENGTH 512

#define FAIL_OPEN_ORIGIN 41           //源文件打开失败
#define FAIL_OPEN_KEY_FILE 42         //新建密钥文件失败
#define FAIL_NEW_CIPHERTEXT 43        //新建密文文件失败
#define ENCRYPTION_SUCCESS 44         //加密成功

#define FAIL_NEW_ORIGIN 51			      //新建完整文件失败
#define FAIL_OPEN_DOWNLOAD_KEY_FILE 52    //打开下载的密钥文件失败
#define FAIL_OPEN_DOWNLOAD_CIPHERTEXT 53  //打开下载的密文文件失败
#define DECRYPTION_SUCCESS 54			  //解密成功

using namespace std;


errno_t err1, err2, err3, err4;

FILE *origin_file, *key_file, *decryption_file, *ciphertext_file;

int file_num;
int single_key;
double encryptPercent;
size_t file_bytes;
size_t file_length;
size_t file_length_2;
size_t key_length;
int random_num1 = NULL;
int file_count = 0;
char file_buffer[BUFFER_SIZE];
char key_buffer[BUFFER_SIZE];

//QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
encryptfile::encryptfile()
{
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
}

QByteArray qstr2str(QString qstr)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QByteArray cdata = qstr.toUtf8();
    //qstr.toLocal8Bit();
    //.toUtf8()
    //cdata.toStdString().c_str();
     const char *s = cdata.toStdString().data();
    return cdata;
}

int encryptfile::encryptFile(QString fileAbPath, QString ykeyAbPath, QString yzipAbPath, int percent, QString file_id, QString user_identify){

//    qDebug()<<'1';


    char originalFileLocalPath[MAX_FILE_ADDRESS_LENGTH] = {};
    char keyLocalPath[MAX_FILE_ADDRESS_LENGTH] = {};
    char ciphertextPath[MAX_FILE_ADDRESS_LENGTH] = {};
    encryptPercent = percent / 100;
    int extractionRate = 100;
    qDebug()<<'2';


    QString SaveFilePath = fileAbPath.toUtf8();
    const char * finalFilePath = SaveFilePath.toStdString().c_str();
    char* cSaveFilePath = new char[strlen(finalFilePath)+1];
    memset(cSaveFilePath,0,sizeof(cSaveFilePath));

    strcpy(cSaveFilePath,finalFilePath);

    QByteArray ypath;
    //ypath = fileAbPath.toStdString().c_str();

    strcpy_s(originalFileLocalPath, fileAbPath.toUtf8());
    strcpy_s(keyLocalPath, ykeyAbPath.toStdString().c_str());
    strcpy_s(ciphertextPath, yzipAbPath.toStdString().c_str());
    QTextCodec *codec = QTextCodec::codecForName("utf8");
    QString filePath;
    filePath = codec->fromUnicode(QString(originalFileLocalPath)).data();

    ypath = qstr2str(fileAbPath);
    const char *s = ypath.data();
    //char *temp = filePath.toLatin1().data();
    int i ;
//    for (i=0;i<filePath.length();i++)
//       originalFileLocalPath[i] = path[i];
//    originalFileLocalPath[i] = '\0';
    //originalFileLocalPath = filePath.toLatin1().data();
    err1 = fopen_s(&origin_file, originalFileLocalPath, "rb+");
    if (err1 != 0)
    {
        qDebug()<<"cannot open original file";
        return FAIL_OPEN_ORIGIN;
    }

    err2 = fopen_s(&key_file, keyLocalPath, "wb+");
    if (err2 != 0)
    {
        qDebug()<<"cannot create key file";
        return FAIL_OPEN_KEY_FILE;
    }

    qDebug()<<'3';
    while ((file_length = fread(file_buffer, 1, BUFFER_SIZE, origin_file)) > 0) {
        file_num++;
        while (file_count < (file_length / extractionRate)) {
            file_count++;
            single_key = file_buffer[file_count * extractionRate];
            fputc(single_key, key_file);
    //        qDebug()<<file_count;

        }
        file_count = 0;
        memset(file_buffer, 0, BUFFER_SIZE);
   //     qDebug()<<file_num;
    }
 //   qDebug()<<'4';

    fclose(key_file);

    fseek(origin_file, 0, SEEK_SET);

    file_num = 0;
    err4 = fopen_s(&ciphertext_file, ciphertextPath, "wb+");
    if (err4 != 0)
    {
        qDebug()<<"cipher file open error";
        return FAIL_NEW_CIPHERTEXT;
    }
    while ((file_length = fread(file_buffer, 1, BUFFER_SIZE, origin_file)) > 0) {
        for (int i = 1; i < (file_length / extractionRate); i++) {
            file_buffer[i * extractionRate] = random_num1;
        }
        file_bytes = file_bytes + file_length;
        fseek(origin_file, file_bytes, SEEK_SET);
        qDebug()<<"write";
        file_num++;
        fwrite(file_buffer, sizeof(char), file_length, ciphertext_file);
        qDebug()<<file_length;
        memset(file_buffer, 0, BUFFER_SIZE);

        qDebug()<<file_bytes;
    }
    qDebug()<<'5';
    fclose(ciphertext_file);
    fclose(origin_file);
    return ENCRYPTION_SUCCESS;
}
