#include "decryptionfile.h"
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

extern int file_num;
extern int single_key;
extern double encryptPercent;
extern size_t file_bytes;
extern size_t file_length;
extern size_t file_length_2;
extern size_t key_length;


extern char file_buffer[BUFFER_SIZE];
extern char key_buffer[BUFFER_SIZE];


int DecryptionFile::decryptFile(QString ykeyAbPath, QString yzipAbPath, QString abPath) {


        char de_originalFileLocalPath[MAX_FILE_ADDRESS_LENGTH] = {};
        char de_keyLocalPath[MAX_FILE_ADDRESS_LENGTH] = {};
        char de_ciphertextPath[MAX_FILE_ADDRESS_LENGTH] = {};

        int extractionRate = 100;

        strcpy_s(de_originalFileLocalPath, abPath.toStdString().c_str());
        strcpy_s(de_keyLocalPath, ykeyAbPath.toStdString().c_str());
        strcpy_s(de_ciphertextPath, yzipAbPath.toStdString().c_str());

        err1 = fopen_s(&origin_file, de_originalFileLocalPath, "wb+");
        if (err1 != 0)
        {
            qDebug()<<("new origin file create error");
            return FAIL_NEW_ORIGIN;
        }
        err2 = fopen_s(&key_file, de_keyLocalPath, "rb+");
        if (err2 != 0)
        {
            qDebug()<<("key file open error");
            return FAIL_OPEN_DOWNLOAD_KEY_FILE;
        }
        err3 = fopen_s(&decryption_file, de_ciphertextPath, "rb+");
        if (err3 != 0)
        {
            qDebug()<<("origin file open error");
            return FAIL_OPEN_DOWNLOAD_CIPHERTEXT;
        }

        while (((file_length_2 = fread(file_buffer, 1, BUFFER_SIZE, decryption_file)) > 0)
            && ((key_length = fread(key_buffer, 1, BUFFER_SIZE / extractionRate, key_file)) > 0)) {
            qDebug()<<'1';
            for (int i = 1; i <= key_length; i++) {
                file_buffer[i * extractionRate] = key_buffer[i - 1];
            }
            fwrite(file_buffer, sizeof(char), file_length_2, origin_file);
            memset(file_buffer, 0, BUFFER_SIZE);
        }

        fclose(origin_file);
        fclose(decryption_file);
        fclose(key_file);




        return DECRYPTION_SUCCESS;
}

DecryptionFile::DecryptionFile()
{

}
