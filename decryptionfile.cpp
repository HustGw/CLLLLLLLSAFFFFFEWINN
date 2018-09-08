#include "decryptionfile.h"
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
extern errno_t err1, err2, err3, err4;

extern FILE *origin_file, *key_file, *decryption_file, *ciphertext_file;
//extern QFile *origin_file_t,*key_file_t, *decryption_file_t, *ciphertext_file_t;
extern int file_num;
extern int single_key;
extern double encryptPercent;
extern size_t file_bytes;
extern size_t file_length;
extern size_t file_length_2;
extern size_t key_length;


extern char file_buffer[BUFFER_SIZE];
extern char key_buffer[BUFFER_SIZE];

DecryptionFile::DecryptionFile()
{

}

bool de_UTF8ToUnicode(const char * UTF8, wchar_t * strUnicode){
     DWORD dwUnicodeLen;    //转换后Unicode的长度
     TCHAR *pwText;      //保存Unicode的指针
    // wchar_t* strUnicode;    //返回值
     //获得转换后的长度，并分配内存
     dwUnicodeLen = MultiByteToWideChar(CP_UTF8,0,UTF8,-1,NULL,0);
     pwText = new TCHAR[dwUnicodeLen];
     if(!pwText){
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


int DecryptionFile::decryptFile(QString ykeyAbPath, QString yzipAbPath, QString abPath) {


        char de_originalFileLocalPath[MAX_FILE_ADDRESS_LENGTH] = {};
        char de_keyLocalPath[MAX_FILE_ADDRESS_LENGTH] = {};
        char de_ciphertextPath[MAX_FILE_ADDRESS_LENGTH] = {};

        int extractionRate = 100;

        abPath = abPath.toUtf8();
        ykeyAbPath = ykeyAbPath.toUtf8();
        yzipAbPath = yzipAbPath.toUtf8();

        strcpy_s(de_originalFileLocalPath, abPath.toStdString().c_str());
        strcpy_s(de_keyLocalPath, ykeyAbPath.toStdString().c_str());
        strcpy_s(de_ciphertextPath, yzipAbPath.toStdString().c_str());

        wchar_t strUnicode1[260];
        de_UTF8ToUnicode(de_originalFileLocalPath, strUnicode1);
        origin_file = _wfopen(strUnicode1, L"wb");

//        err1 = fopen_s(&origin_file, de_originalFileLocalPath, "wb+");
//        if (err1 != 0)
//        {
//            qDebug()<<("new origin file create error");
//            return FAIL_NEW_ORIGIN;
//        }
//        key_file_t = new QFile(ykeyAbPath);
//        bool openflag2 = key_file_t->open(QIODevice::ReadWrite);
//        qDebug()<<openflag2;
        wchar_t strUnicode2[260];
        de_UTF8ToUnicode(de_keyLocalPath, strUnicode2);
        key_file = _wfopen(strUnicode2, L"rb");
//        err2 = fopen_s(&key_file, de_keyLocalPath, "rb+");
//        if (err2 != 0)
//        {
//            qDebug()<<("key file open error");
//            return FAIL_OPEN_DOWNLOAD_KEY_FILE;
//        }
//        decryption_file_t = new QFile(yzipAbPath);
//        bool openflag3 = decryption_file_t->open(QIODevice::ReadWrite);
//        qDebug()<<openflag3;
        wchar_t strUnicode3[260];
        de_UTF8ToUnicode(de_ciphertextPath, strUnicode3);
        decryption_file = _wfopen(strUnicode3, L"rb");
//        err3 = fopen_s(&decryption_file, de_ciphertextPath, "rb+");
//        if (err3 != 0)
//        {
//            qDebug()<<("origin file open error");
//            return FAIL_OPEN_DOWNLOAD_CIPHERTEXT;
//        }
//        while ((file_length_2 = decryption_file_t->read(file_buffer,BUFFER_SIZE)) >0
//               &&(key_length = key_file_t->read(key_buffer,BUFFER_SIZE / extractionRate)) >0) {
        while (((file_length_2 = fread(file_buffer, 1, BUFFER_SIZE, decryption_file)) > 0)
            && ((key_length = fread(key_buffer, 1, BUFFER_SIZE / extractionRate, key_file)) > 0)) {
            qDebug()<<'1';
            for (int i = 1; i <= key_length; i++) {
                file_buffer[i * extractionRate] = key_buffer[i - 1];
            }
            fwrite(file_buffer, sizeof(char), file_length_2, origin_file);
            //origin_file_t->write(file_buffer,file_length_2);
            memset(file_buffer, 0, BUFFER_SIZE);
        }

//        origin_file_t->close();
//        decryption_file_t->close();
//        key_file_t->close();
        fclose(origin_file);
        fclose(decryption_file);
        fclose(key_file);

        return DECRYPTION_SUCCESS;
}



