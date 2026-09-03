#include <libopenvk.h>
#include <string.h>

struct ovk_instanceInfo openvk_ovk_aboutInstance(openvk_data_t *data) {
    const char *resp_buf;
    struct ovk_instanceInfo ret;
    int ret2 = openvk_call(data, "ovk.aboutInstance", &resp_buf, false, 0);
    if (ret2) {
        return ret;
    }
    json_t *resp_node;
    json_error_t err = openvk_get_resp(resp_buf, &resp_node);
    if (!resp_node) {
        return ret;
    }

    json_t *stats_node = json_object_get(resp_node, "statistics");
    ret.stats.users_count = json_integer_value(json_object_get(stats_node, "users_count"));
    ret.stats.online_users_count = json_integer_value(json_object_get(stats_node, "online_users_count"));
    ret.stats.active_users_count = json_integer_value(json_object_get(stats_node, "active_users_count"));
    ret.stats.groups_count = json_integer_value(json_object_get(stats_node, "groups_count"));
    ret.stats.wall_posts_count = json_integer_value(json_object_get(stats_node, "wall_posts_count"));

    json_t *admins_node = json_object_get(resp_node, "administrators");
    int adm_count = json_integer_value(json_object_get(admins_node, "count"));
    struct ovk_user *admns = malloc(adm_count * sizeof(struct ovk_user));
    json_t *admins = json_object_get(admins_node, "items");
    for (int i=0;i<adm_count;i++) {
        json_t *adm = json_array_get(admins, i);
        admns[i].id = json_integer_value(json_object_get(adm, "id"));
        admns[i].first_name = strdup(json_string_value(json_object_get(adm, "first_name")));
        admns[i].last_name = strdup(json_string_value(json_object_get(adm, "last_name")));
        admns[i].is_closed = json_integer_value(json_object_get(adm, "is_closed"));
        admns[i].can_access_closed = json_integer_value(json_object_get(adm, "can_access_closed"));
        admns[i].online = json_integer_value(json_object_get(adm, "online"));
    }
    ret.admins.count = adm_count;
    ret.admins.users = admns;

    // TODO: Popular groups

    json_t *links_node = json_object_get(resp_node, "links");
    int lnk_count = json_integer_value(json_object_get(links_node, "count"));
    struct ovk_link *lnks = malloc(lnk_count * sizeof(struct ovk_link));
    json_t *links = json_object_get(links_node, "items");
    for (int i=0;i<lnk_count;i++) {
        json_t *lnk = json_array_get(links, i);
        lnks[i].name = strdup(json_string_value(json_object_get(lnk, "name")));
        lnks[i].link = strdup(json_string_value(json_object_get(lnk, "url")));
    }
    ret.links.count = lnk_count;
    ret.links.links = lnks;

    json_decref(resp_node);

    return ret;
}

const char *openvk_ovk_version(openvk_data_t *data) {
    const char *resp_buf;
    int ret = openvk_call(data, "ovk.version", &resp_buf, false, 0);
    if (ret) {
        return NULL;
    }

    json_t *resp;
    json_error_t err = openvk_get_resp(resp_buf, &resp);
    if (!resp) return NULL;

    return strdup(json_string_value(resp));
}