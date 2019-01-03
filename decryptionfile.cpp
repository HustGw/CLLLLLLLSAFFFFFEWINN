#include "decryptionfile.h"
#include <QFile>
#include "QDir"
#define BUFFER_SIZE 1000
#define MAX_FILE_ADDRESS_LENGTH 512

#define FAIL_ERRO_VERSION 30          //版本号不匹配
#define FAIL_NOTYFILE 31              //文件不是密文

#define FAIL_OPEN_ORIGIN 41           //源文件打开失败
#define FAIL_OPEN_KEY_FILE 42         //新建密钥文件失败
#define FAIL_NEW_CIPHERTEXT 43        //新建密文文件失败
#define ENCRYPTION_SUCCESS 44         //加密成功

#define FAIL_NEW_ORIGIN 51			      //新建完整文件失败
#define FAIL_OPEN_DOWNLOAD_KEY_FILE 52    //打开下载的密钥文件失败
#define FAIL_OPEN_DOWNLOAD_CIPHERTEXT 53  //打开下载的密文文件失败
#define DECRYPTION_SUCCESS 54			  //解密成功

using namespace std;
extern QString tempFilePath;
extern errno_t err1, err2, err3, err4;


//extern QFile *origin_file_t,*key_file_t, *decryption_file_t, *ciphertext_file_t;
 int d_file_num;
 int d_single_key;
 double d_encryptPercent;
 size_t d_file_bytes;
 size_t d_file_length;
 size_t d_file_length_2;
 size_t d_key_length;


 char d_file_buffer[BUFFER_SIZE];
 char d_key_buffer[BUFFER_SIZE];

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
//获取文件末尾信息
//v_dest 版本号
//f_dest file_id
//u_dest user_id
//版本号
//1
//0 前两位表示当前软件版本
//1 表示加密等级  1--100 2--20 3--30
//0 表示加密客户端类型 0 PC端  1 安卓端  2 iOS端
int DecryptionFile::VerifyFile(QString yzipAbPath,char v_dest[],char f_dest[],char u_dest[]){
    FILE *yzipAbPath_file;
    yzipAbPath_file = nullptr;
    char de_yzipAbPath[MAX_FILE_ADDRESS_LENGTH] = {};

    yzipAbPath = yzipAbPath.toUtf8();

    strcpy_s(de_yzipAbPath, yzipAbPath.toStdString().c_str());

    wchar_t strUnicode5[260];
    de_UTF8ToUnicode(de_yzipAbPath, strUnicode5);
    yzipAbPath_file = _wfopen(strUnicode5, L"rb");

    if (!yzipAbPath_file)
       {
           perror("Failed to open file.\n");
           return FAIL_OPEN_ORIGIN;
       }
    //读取源文件，判断版本是否满足解密
    int current_pos = 0;
    char ch = 0;
    long offset = -1;
    const char CR = '$';
    //v_dest[100] = {'0'};
    //char u_dest[100];
    //char f_dest[100];
     int v_length = 0;
     int u_length = 0;
     int f_length = 0;
     int verNum = 0;
    /*初始化dest数组*/
     memset(v_dest, 0, sizeof(v_dest));
     memset(v_dest, 0, sizeof(f_dest));
     memset(v_dest, 0, sizeof(u_dest));
    /*将文件指针移动到倒数第一个字符*/
    current_pos = fseek(yzipAbPath_file, offset, SEEK_END);
    /*从后往前寻找第一个$符号为止*/
    while ((ch = (char)fgetc(yzipAbPath_file)) != CR)
    {
        verNum ++;
        if (verNum >= 100){
            memset(v_dest,'\0',sizeof(u_dest));
            return FAIL_NOTYFILE;
        }else {
            //qDebug()<<ch;
            /*将找到的字符加入dest数组*/
            char src[2] = { ch, '\0' };
            strcat(v_dest, src);
            offset += -1;
            current_pos = fseek(yzipAbPath_file, offset, SEEK_END);
        }
    }
    v_length -= offset + 1;
    //offset += -1;
    current_pos = fseek(yzipAbPath_file, offset, SEEK_END);
    int num = 0;
    for(int i =0 ; i < 4 ; i ++){

        if((ch = (char)fgetc(yzipAbPath_file)) == CR){
            num ++;
            qDebug()<<ch;
        }else{
            //不是相连四个$，清空数组
            memset(v_dest,'\0',sizeof(v_dest));
        }
        if(num == 4){
            qDebug()<<"vision";
            /*因为dest中存入的数字是按文件从后往前顺序排列的，所以需要倒置顺序*/
            _strrev(v_dest);
            for (int j = 0 ; j < v_length; j++){
                qDebug()<<v_dest[j];
            }
        }
        offset += -1;
        current_pos = fseek(yzipAbPath_file, offset, SEEK_END);
    }

    //读取源文件，存储userid
    while ((ch = (char)fgetc(yzipAbPath_file)) != CR)
    {
        /*将找到的字符加入dest数组*/
        char src[2] = { ch, '\0' };
        strcat(u_dest, src);
        offset += -1;
        current_pos = fseek(yzipAbPath_file, offset, SEEK_END);
    }
    u_length -= offset + 1;
    current_pos = fseek(yzipAbPath_file, offset, SEEK_END);
    num = 0;
    for(int i =0 ; i < 4 ; i ++){

        if((ch = (char)fgetc(yzipAbPath_file)) == CR){
            num ++;
            qDebug()<<ch;
        }else{
            //不是相连四个$，清空数组
            memset(u_dest,'\0',sizeof(u_dest));
        }
        if(num == 4){
            qDebug()<<"userid";
            /*因为dest中存入的数字是按文件从后往前顺序排列的，所以需要倒置顺序*/
            _strrev(u_dest);
            for (int j = 0 ; j < (u_length - v_length - 4); j++){
                qDebug()<<u_dest[j];
            }
        }
        offset += -1;
        current_pos = fseek(yzipAbPath_file, offset, SEEK_END);
    }
    //读取源文件，存储fileid
    while ((ch = (char)fgetc(yzipAbPath_file)) != CR)
    {
        /*将找到的字符加入dest数组*/
        char src[2] = { ch, '\0' };
        strcat(f_dest, src);
        offset += -1;
        current_pos = fseek(yzipAbPath_file, offset, SEEK_END);
    }
    f_length -= offset + 1;
    current_pos = fseek(yzipAbPath_file, offset, SEEK_END);
    num = 0;
    for(int i =0 ; i < 4 ; i ++){

        if((ch = (char)fgetc(yzipAbPath_file)) == CR){
            num ++;
            qDebug()<<ch;
        }else{
            //不是相连四个$，清空数组
            memset(f_dest,'\0',sizeof(f_dest));
        }
        if(num == 4){
            qDebug()<<"fileid";
            qDebug()<<f_length;
            /*因为dest中存入的数字是按文件从后往前顺序排列的，所以需要倒置顺序*/
            _strrev(f_dest);
            for (int j = 0 ; j < (f_length - u_length - 4); j++){

                qDebug()<<f_dest[j];
            }
        }
        offset += -1;
        current_pos = fseek(yzipAbPath_file, offset, SEEK_END);
    }
    f_length = f_length - u_length - 4;
    u_length = u_length - v_length - 4;
    fseek(yzipAbPath_file, 0, SEEK_SET);
    fclose(yzipAbPath_file);
}

int DecryptionFile::decryptFile(QString ykeyAbPath, QString yzipAbPath, QString abPath, int decryptRate) {

    FILE *d_origin_file, *d_key_file, *d_decryption_file, *d_ciphertext_file;
    d_origin_file = nullptr;
    d_key_file = nullptr;
    d_decryption_file = nullptr;
    d_ciphertext_file = nullptr;
    d_file_num = 0;
    d_single_key = 0;
    d_encryptPercent = 0;
    d_file_bytes = 0;
    d_file_length = 0;
    d_file_length_2 = 0;

    char de_originalFileLocalPath[MAX_FILE_ADDRESS_LENGTH] = {};
    char de_keyLocalPath[MAX_FILE_ADDRESS_LENGTH] = {};
    char de_ciphertextPath[MAX_FILE_ADDRESS_LENGTH] = {};

    int extractionRate = decryptRate;

    QFileInfo or_fileInfo (abPath);
    QFileInfo yz_fileInfo (yzipAbPath);
    //QString tempFilePath = "C://CloundSafe//"+User_qqNum+"//temp//" + originalFileName;
    QString temp_filePath = tempFilePath +"tmp"+ yz_fileInfo.fileName();
    QString fileSuffix = or_fileInfo.suffix();
    qDebug()<<"后缀名："<<fileSuffix;
    //base64解码
    if (fileSuffix.compare(QString::fromLocal8Bit("txt") )==0 || fileSuffix.compare(QString::fromLocal8Bit("png") )==0){
        temp_filePath = temp_filePath.toUtf8();
        strcpy_s(de_originalFileLocalPath, temp_filePath.toStdString().c_str());
    }else{
        abPath = abPath.toUtf8();
        strcpy_s(de_originalFileLocalPath, abPath.toStdString().c_str());
    }
        ykeyAbPath = ykeyAbPath.toUtf8();
        yzipAbPath = yzipAbPath.toUtf8();

        strcpy_s(de_keyLocalPath, ykeyAbPath.toStdString().c_str());
        strcpy_s(de_ciphertextPath, yzipAbPath.toStdString().c_str());

        wchar_t strUnicode1[260];
        de_UTF8ToUnicode(de_originalFileLocalPath, strUnicode1);
        d_origin_file = _wfopen(strUnicode1, L"wb");

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
        d_key_file = _wfopen(strUnicode2, L"rb");
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
        d_decryption_file = _wfopen(strUnicode3, L"rb");
//        err3 = fopen_s(&decryption_file, de_ciphertextPath, "rb+");
//        if (err3 != 0)
//        {
//            qDebug()<<("origin file open error");
//            return FAIL_OPEN_DOWNLOAD_CIPHERTEXT;
//        }
//        while ((file_length_2 = decryption_file_t->read(file_buffer,BUFFER_SIZE)) >0
//               &&(key_length = key_file_t->read(key_buffer,BUFFER_SIZE / extractionRate)) >0) {
        while (((d_file_length_2 = fread(d_file_buffer, 1, BUFFER_SIZE, d_decryption_file)) > 0)
            && ((d_key_length = fread(d_key_buffer, 1, BUFFER_SIZE / extractionRate, d_key_file)) > 0)) {
            qDebug()<<'1';
            for (int i = 1; i <= d_key_length; i++) {
                d_file_buffer[i * extractionRate] = d_key_buffer[i - 1];
            }
            fwrite(d_file_buffer, sizeof(char), d_file_length_2, d_origin_file);
            //origin_file_t->write(file_buffer,file_length_2);
            memset(d_file_buffer, 0, BUFFER_SIZE);
        }

//        origin_file_t->close();
//        decryption_file_t->close();
//        key_file_t->close();
        fclose(d_origin_file);
        fclose(d_decryption_file);
        fclose(d_key_file);
        //QString fileSuffix = fileInfo.suffix();
        qDebug()<<"后缀名："<<fileSuffix;
        //base64解码
        if (fileSuffix.compare(QString::fromLocal8Bit("txt") )==0 || fileSuffix.compare(QString::fromLocal8Bit("png") )==0){
            QFile read_fileToBase64(temp_filePath);
            if(!read_fileToBase64.open(QIODevice::ReadOnly))
                return false;

            QByteArray decryption_Array = QByteArray::fromBase64(read_fileToBase64.readAll());

            QFile write_fileToBase64(abPath);
            if(!write_fileToBase64.open(QIODevice::WriteOnly))
                return false;

            write_fileToBase64.write(decryption_Array);
            read_fileToBase64.close();
            write_fileToBase64.close();
        }
//        QFile file_tem(temp_filePath);
//        if (file_tem.exists())
//        {
//            file_tem.remove();
//        }
//        file_tem.close();

            //yzipAbPath = temp_filePath;
        return DECRYPTION_SUCCESS;
}



