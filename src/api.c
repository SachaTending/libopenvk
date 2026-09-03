#include <curl/curl.h>
#include <stdlib.h>
#include <libopenvk.h>
#include <string.h>
#include <jansson.h>

struct resp_buf {
    char *buf;
    size_t size;
};

static size_t write_cb(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct resp_buf *mem = (struct resp_buf *)userp;
    mem->buf = realloc(mem->buf, mem->size + realsize + 1);
    memcpy(&(mem->buf[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->buf[mem->size] = 0;
    return realsize;
}

int openvk_init(openvk_data_t **data) {
    CURLcode result = curl_global_init(CURL_GLOBAL_ALL);
    if(result != CURLE_OK)
        return (int)result;
    
    openvk_data_t *priv;
    priv = (openvk_data_t *)malloc(sizeof(openvk_data_t));
    memset(priv, 0, sizeof(openvk_data_t));
    
    priv->user_agent = strdup("libopenvk/1.0");

    *data = priv;
    return 0;
}

int openvk_set_instance(openvk_data_t *data, const char *api_base_url) {
    if (data->instance) {
        free((void *)data->instance);
    }
    data->instance = strdup(api_base_url);
    return 0;
}

void openvk_free(openvk_data_t *data) {
    #define FREE_IF_EXISTS(ptr) if (ptr) free((void *)ptr)

    FREE_IF_EXISTS(data->instance);
    FREE_IF_EXISTS(data->user_agent);
    free((void *)data);

    #undef FREE_IF_EXISTS
}

int openvk_call(openvk_data_t *data, const char *method, const char **resp_buf) {
    char url_buf[512];
    snprintf(url_buf, sizeof(url_buf), "%s/method/%s", data->instance, method);

    struct resp_buf buf;
    buf.buf = malloc(1);
    buf.size = 0;

    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, data->user_agent);
    curl_easy_setopt(curl, CURLOPT_URL, url_buf);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);

    CURLcode ret = curl_easy_perform(curl);

    if (ret != CURLE_OK) {
        return ret;
    }

    *resp_buf = buf.buf;

    return 0;
}

json_error_t openvk_get_resp(const char *resp, json_t **resp_node) {
    json_error_t err;
    json_t *root = json_loads(resp, 0, &err);
    if (!root) {
        return err;
    }
    json_t *r_node = json_object_get(root, "response");
    *resp_node = r_node;
    return err;
}