#ifndef DOWNLOADOSS_H
#define DOWNLOADOSS_H
#include "aos_log.h"
#include "aos_util.h"
#include "aos_string.h"
#include "aos_status.h"
#include "oss_auth.h"
#include "oss_util.h"
#include "oss_api.h"
#include "QDebug"

class downloadoss
{
public:
    const char *OSS_ENDPOINT = "oss-cn-shenzhen.aliyuncs.com";
    const char *ACCESS_KEY_ID = "LTAIxMHXx6Zck9A6";
    const char *ACCESS_KEY_SECRET = "xkv1rWSPYLqlI0JGxZaoRQcqIDTtLS";
    const char *BUCKET_NAME ;//下载文件夹名称
    const char *OBJECT_NAME ;//下载文件名
    char *download_filePath;//文件下载本地保存地址,要包含文件名
    downloadoss();
    void init_sample_request_options(oss_request_options_t *options, int is_cname);
    void get_object_to_file();
    //void percentage(int64_t consumed_bytes, int64_t total_bytes);

signals:
    void progressBarValue(double value);
};

#endif // DOWNLOADOSS_H
