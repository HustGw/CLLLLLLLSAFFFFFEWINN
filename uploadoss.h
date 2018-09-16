#ifndef UPLOADOSS_H
#define UPLOADOSS_H
#include "aos_log.h"
#include "aos_util.h"
#include "aos_string.h"
#include "aos_status.h"
#include "oss_auth.h"
#include "oss_util.h"
#include "oss_api.h"
#include "QDebug"
#include "QFile"

class uploadoss
{
public:
    const char *OSS_ENDPOINT = "oss-cn-shenzhen.aliyuncs.com";
    const char *ACCESS_KEY_ID = "LTAIxMHXx6Zck9A6";
    const char *ACCESS_KEY_SECRET = "xkv1rWSPYLqlI0JGxZaoRQcqIDTtLS";
    const char *BUCKET_NAME ;
    const char *OBJECT_NAME ;
    char *filepath;
    uploadoss();
    void init_sample_request_options(oss_request_options_t *options, int is_cname);
    int put_object_from_file();
};

#endif // UPLOADOSS_H
