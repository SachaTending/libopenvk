#include <libopenvk.h>
#include <string.h>

struct ovk_getProfileInfo openvk_account_getProfileInfo(openvk_data_t *data) {
    const char *resp_buf;
    int ret = openvk_call(data, "account.getProfileInfo", &resp_buf, true, 0);

    struct ovk_getProfileInfo out;
    memset(&out, 0, sizeof(struct ovk_getProfileInfo));

    json_t *resp_node;
    json_error_t err = openvk_get_resp(resp_buf, &resp_node);

    if (!resp_node) {
        return out;
    }

    //printf(resp_buf);

    #define set_var_str(key) out. key = strdup(JSON_STR(resp_node, #key))
    #define set_var_int(key) out. key = JSON_INT(resp_node, #key)

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

    return out;
}