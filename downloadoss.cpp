#include "downloadoss.h"

downloadoss::downloadoss()
{

}
void downloadoss::init_sample_request_options(oss_request_options_t *options, int is_cname)
{
    options->config = oss_config_create(options->pool);
    aos_str_set(&options->config->endpoint, OSS_ENDPOINT);
    aos_str_set(&options->config->access_key_id, ACCESS_KEY_ID);
    aos_str_set(&options->config->access_key_secret, ACCESS_KEY_SECRET);
    options->config->is_cname = is_cname;

    options->ctl = aos_http_controller_create(options->pool, 0);
}
//void downloadoss::get_object_to_file()
//{
//    apr_file_t *output = NULL;
//    aos_pool_t *pool = NULL;
//    apr_status_t ret;

//    // initialize http io system, call it olny once
//    if (aos_http_io_initialize(NULL, 0) != AOSE_OK) {
//        exit(1);
//    }
//    aos_pool_create(&pool, NULL);

//    aos_pool_t *p = NULL;
//    aos_string_t bucket;
//    char *download_filename = "SouthIsland_NewZealand_New.jpg";
//    aos_string_t object;
//    int is_cname = 0;
//    oss_request_options_t *options = NULL;
//    aos_table_t *headers = NULL;
//    aos_table_t *params = NULL;
//    aos_table_t *resp_headers = NULL;
//    aos_status_t *s = NULL;
//    aos_string_t file;

//    aos_pool_create(&p, NULL);
//    options = oss_request_options_create(p);
//    init_sample_request_options(options, is_cname);
//    aos_str_set(&bucket, BUCKET_NAME);
//    aos_str_set(&object, OBJECT_NAME);
//    headers = aos_table_make(p, 0);
//    aos_str_set(&file, download_filePath);

//    s = oss_get_object_to_file(options, &bucket, &object, headers,
//                               params, &file, &resp_headers);

//    if (aos_status_is_ok(s)) {
//       qDebug()<< "get object to local file succeeded\n";
//    } else {
//       qDebug()<<"get object to local file failed\n";
//    }

//    aos_pool_destroy(p);
//    //apr_file_close(output);

//    aos_pool_destroy(pool);

//    // deinitialize http io system, call it olny once
//    aos_http_io_deinitialize();
//}
void percentage(int64_t consumed_bytes, int64_t total_bytes)
{
    assert(total_bytes >= consumed_bytes);
    //printf("%%%" APR_INT64_T_FMT "\n", consumed_bytes * 100 / total_bytes);
    qDebug()<<consumed_bytes * 100 / total_bytes;
    double value = consumed_bytes * 100 / total_bytes;
    //emit downloadoss::progressBarValue(value);

}

void downloadoss::get_object_to_file()
{
    aos_pool_t *p = NULL;
    aos_status_t *s = NULL;
    int is_cname = 0;
    aos_string_t bucket;
    aos_string_t object;
    aos_string_t filename;
    oss_request_options_t *options = NULL;
    aos_table_t *resp_headers = NULL;
    aos_list_t resp_body;
    char *download_filename = "get_object_to_local_file.txt";
    /* init test*/
    if (aos_http_io_initialize(NULL, 0) != AOSE_OK) {
            exit(1);
        }
    aos_pool_create(&p, NULL);
    options = oss_request_options_create(p);
    init_sample_request_options(options, is_cname);

    aos_str_set(&bucket, BUCKET_NAME);
    aos_str_set(&object, OBJECT_NAME);
    aos_str_set(&filename, __FILE__);
    aos_list_init(&resp_body);
//    /* put object */
//    s = oss_do_put_object_from_file(options, &bucket, &object, &filename, NULL, NULL, percentage, &resp_headers, &resp_body);
//    if (aos_status_is_ok(s)) {
//        printf("put object from file succeeded\n");
//    } else {
//        printf("put object from file failed\n");
//        aos_pool_destroy(p);
//        return;
//    }
//    aos_pool_destroy(p);
    /* get object */
    aos_pool_create(&p, NULL);
    options = oss_request_options_create(p);
    init_sample_request_options(options, is_cname);
    aos_str_set(&filename, download_filename);
    s = oss_do_get_object_to_file(options, &bucket, &object, NULL, NULL, &filename, percentage, NULL);
    if (aos_status_is_ok(s)) {
        qDebug()<< "get object to local file succeeded\n";
    } else {
        qDebug()<<"get object to local file failed\n";
    }
    aos_pool_destroy(p);

}
