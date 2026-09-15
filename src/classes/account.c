#include <libopenvk.h>
#include <string.h>

int openvk_account_getProfileInfo(openvk_data_t *data, struct ovk_getProfileInfo *out) {
    const char *resp_buf;
    int ret = openvk_call(data, "account.getProfileInfo", &resp_buf, true, 0);

    memset(out, 0, sizeof(struct ovk_getProfileInfo));

    json_t *resp_node;
    int err = openvk_get_resp(resp_buf, &resp_node);

    if (!resp_node) {
        return OVK_API_ERROR;
    }

    //printf(resp_buf);

    #define set_var_str(key) out->key = strdup(JSON_STR(resp_node, #key))
    #define set_var_int(key) out->key = JSON_INT(resp_node, #key)

    set_var_str(first_name);
    set_var_str(last_name);
    set_var_str(photo_200);
    set_var_str(nickname);

    set_var_int(is_service_account);
    set_var_int(id);
    set_var_int(is_verified);

    set_var_str(verification_status);
    set_var_str(bdate);
    set_var_str(status);

    set_var_int(bdate_visibility);
    set_var_int(relation);
    set_var_str(screen_name);
    set_var_int(sex);

    #undef set_var_int
    #undef set_var_str

    json_decref(resp_node);
    free(resp_buf);

    return OVK_API_OK;
}

int openvk_account_setOnline(openvk_data_t *data) {
    const char *resp_buf;
    int ret = openvk_call(data, "account.setOnline", &resp_buf, true, 0);

    if (ret != OVK_API_OK) {
        return ret;
    }
    if (resp_buf[0] != '1') return OVK_API_ERROR; // in ovk's api spec account.setOnline by default returns 1, so i guess if response 1, then something went wrong
    free(resp_buf);
    return OVK_API_OK;
}

int openvk_account_getCounters(openvk_data_t *data, struct ovk_acc_getCounters *out) {
    const char *resp_buf;
    int ret = openvk_call(data, "account.getCounters", &resp_buf, true, 0);
    if (ret != OVK_API_OK) {
        return ret;
    }

    json_t *resp_node;
    int err = openvk_get_resp(resp_buf, &resp_node);

    if (!resp_node) {
        return OVK_API_ERROR;
    }

    #define set_var_int(key) out->key = JSON_INT(resp_node, #key)
    set_var_int(friends);
    set_var_int(messages);
    set_var_int(notifications);
    #undef set_var_int

    return OVK_API_OK;
}