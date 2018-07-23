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
void downloadoss::get_object_to_file()
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
    char *download_filename = "SouthIsland_NewZealand_New.jpg";
    aos_string_t object;
    int is_cname = 0;
    oss_request_options_t *options = NULL;
    aos_table_t *headers = NULL;
    aos_table_t *params = NULL;
    aos_table_t *resp_headers = NULL;
    aos_status_t *s = NULL;
    aos_string_t file;

    aos_pool_create(&p, NULL);
    options = oss_request_options_create(p);
    init_sample_request_options(options, is_cname);
    aos_str_set(&bucket, BUCKET_NAME);
    aos_str_set(&object, OBJECT_NAME);
    headers = aos_table_make(p, 0);
    aos_str_set(&file, download_filePath);

    s = oss_get_object_to_file(options, &bucket, &object, headers,
                               params, &file, &resp_headers);

    if (aos_status_is_ok(s)) {
       qDebug()<< "get object to local file succeeded\n";
    } else {
       qDebug()<<"get object to local file failed\n";
    }

    aos_pool_destroy(p);
    //apr_file_close(output);

    aos_pool_destroy(pool);

    // deinitialize http io system, call it olny once
    aos_http_io_deinitialize();
}
