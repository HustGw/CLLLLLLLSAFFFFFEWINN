#include "encryptfile.h"
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <QDebug>

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

encryptfile::encryptfile()
{

}

bool UTF8ToUnicode(const char * UTF8, wchar_t * strUnicode)
{
 DWORD dwUnicodeLen;    //转换后Unicode的长度
 TCHAR *pwText;      //保存Unicode的指针
// wchar_t* strUnicode;    //返回值
 //获得转换后的长度，并分配内存
 dwUnicodeLen = MultiByteToWideChar(CP_UTF8,0,UTF8,-1,NULL,0);
 pwText = new TCHAR[dwUnicodeLen];
 if(!pwText)
 {
 return false;
 }
 //转为Unicode
 MultiByteToWideChar(CP_UTF8,0,UTF8,-1,pwText,dwUnicodeLen);
 //转为CString
 wcscpy(strUnicode, pwText);
 //清除内存
 delete[]pwText;
 return true;
}

int encryptfile::encryptFile(QString fileAbPath, QString ykeyAbPath, QString yzipAbPath, int percent, QString file_id, QString user_identify){

    qDebug()<<'1';

    origin_file = nullptr;
    key_file = nullptr;
    decryption_file = nullptr;
    ciphertext_file = nullptr;
    file_num = 0;
    single_key = 0;
    encryptPercent = 0;
    file_bytes = 0;
    file_length = 0;
    file_length_2 = 0;
//    key_length = 0;

    char originalFileLocalPath[MAX_FILE_ADDRESS_LENGTH] = {};
    char keyLocalPath[MAX_FILE_ADDRESS_LENGTH] = {};
    char ciphertextPath[MAX_FILE_ADDRESS_LENGTH] = {};
    encryptPercent = percent / 100;
    int extractionRate = 100;
    qDebug()<<'2';
    ykeyAbPath = ykeyAbPath.toUtf8();
    fileAbPath = fileAbPath.toUtf8();
    yzipAbPath = yzipAbPath.toUtf8();
    strcpy_s(originalFileLocalPath, fileAbPath.toStdString().c_str());
    strcpy_s(keyLocalPath, ykeyAbPath.toStdString().c_str());
    strcpy_s(ciphertextPath, yzipAbPath.toStdString().c_str());

    wchar_t strUnicode1[260];
    UTF8ToUnicode(originalFileLocalPath, strUnicode1);
    origin_file = _wfopen(strUnicode1, L"rb");

    //err1 = fopen_s(&origin_file, originalFileLocalPath, "rb+");
//    if (err1 != 0)
//    {
//        qDebug()<<"cannot open original file";
//        return FAIL_OPEN_ORIGIN;
//    }

    wchar_t strUnicode2[260];
    UTF8ToUnicode(keyLocalPath, strUnicode2);
    key_file = _wfopen(strUnicode2, L"wb");

//    err2 = fopen_s(&key_file, keyLocalPath, "wb+");
//    if (err2 != 0)
//    {
//        qDebug()<<"cannot create key file";
//        return FAIL_OPEN_KEY_FILE;
//    }

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
    wchar_t strUnicode3[260];
    UTF8ToUnicode(ciphertextPath, strUnicode3);
    ciphertext_file = _wfopen(strUnicode3, L"wb");
//    err4 = fopen_s(&ciphertext_file, ciphertextPath, "wb+");
//    if (err4 != 0)
//    {
//        qDebug()<<"cipher file open error";
//        return FAIL_NEW_CIPHERTEXT;
//    }
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
