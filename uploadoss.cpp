#include "uploadoss.h"


uploadoss::uploadoss()
{


    // open log file
    //ret = apr_file_open(&output, "oss-c-sdk-sample.log", APR_FOPEN_CREATE | APR_FOPEN_WRITE | APR_FOPEN_APPEND, APR_OS_DEFAULT, pool);
//    if (APR_SUCCESS != ret) {
//        exit(1);
//    }
    // set log file
    //aos_log_set_output(output);
    // set log level
    //aos_log_set_level(AOS_LOG_INFO);

    //aos_info_log("start");

    // run samples
    //put_object_from_buffer();
    //put_object_from_file();
// close log file

}

int uploadoss::put_object_from_file()
{
    apr_file_t *output = NULL;
    aos_pool_t *pool = NULL;
    apr_status_t ret;

    // initialize http io system, call it olny once
    if (aos_http_io_initialize(NULL, 0) != AOSE_OK) {
        exit(1);
    }
    aos_pool_create(&pool, NULL);

    aos_pool_t *p = NULL;
        aos_string_t bucket;
        aos_string_t object;
        int is_cname = 0;
        aos_table_t *headers = NULL;
        aos_table_t *resp_headers = NULL;
        oss_request_options_t *options = NULL;
        char *filename = filepath;
        aos_status_t *s = NULL;
        aos_string_t file;

        aos_pool_create(&p, NULL);
        options = oss_request_options_create(p);
        init_sample_request_options(options, is_cname);
        headers = aos_table_make(options->pool, 1);
        apr_table_set(headers, OSS_CONTENT_TYPE, "image/jpeg");
        aos_str_set(&bucket, BUCKET_NAME);
        aos_str_set(&object, OBJECT_NAME);
        aos_str_set(&file, filename);

        QFile ofile(filepath);
        if(!ofile.open(QIODevice::ReadWrite )) {
           return 3;
        }
        s = oss_put_object_from_file(options, &bucket, &object, &file,headers, &resp_headers);

        if (aos_status_is_ok(s)) {
            qDebug()<< OBJECT_NAME;
            qDebug()<< "put object from file succeeded";
            return 1;
        } else {
             qDebug()<<"put object from file failed"<<s->code<<s->error_code<<s->error_msg<<s->req_id;
                       //code:%d, error_code:%s, error_msg:%s, request_id:%s\n",
                //s->code, s->error_code, s->error_msg, s->req_id;
             return 0;
        }

        aos_pool_destroy(p);

        //apr_file_close(output);

        aos_pool_destroy(pool);

        // deinitialize http io system, call it olny once
        aos_http_io_deinitialize();
}

void uploadoss::init_sample_request_options(oss_request_options_t *options, int is_cname)
{
    options->config = oss_config_create(options->pool);
    aos_str_set(&options->config->endpoint, OSS_ENDPOINT);
    aos_str_set(&options->config->access_key_id, ACCESS_KEY_ID);
    aos_str_set(&options->config->access_key_secret, ACCESS_KEY_SECRET);
    options->config->is_cname = is_cname;

    options->ctl = aos_http_controller_create(options->pool, 0);
}
