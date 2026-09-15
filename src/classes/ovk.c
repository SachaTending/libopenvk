#include <libopenvk.h>
#include <string.h>

int openvk_ovk_aboutInstance(openvk_data_t *data, struct ovk_instanceInfo *out) {
    const char *resp_buf;
    int ret2 = openvk_call(data, "ovk.aboutInstance", &resp_buf, false, 0);
    if (ret2 != OVK_API_OK) {
        return ret2;
    }
    json_t *resp_node;
    int err = openvk_get_resp(resp_buf, &resp_node);
    if (!resp_node) {
        return OVK_API_ERROR;
    }

    json_t *stats_node = json_object_get(resp_node, "statistics");
    out->stats.users_count = json_integer_value(json_object_get(stats_node, "users_count"));
    out->stats.online_users_count = json_integer_value(json_object_get(stats_node, "online_users_count"));
    out->stats.active_users_count = json_integer_value(json_object_get(stats_node, "active_users_count"));
    out->stats.groups_count = json_integer_value(json_object_get(stats_node, "groups_count"));
    out->stats.wall_posts_count = json_integer_value(json_object_get(stats_node, "wall_posts_count"));

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
    out->admins.count = adm_count;
    out->admins.users = admns;

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
    out->links.count = lnk_count;
    out->links.links = lnks;

    json_decref(resp_node);

    return OVK_API_OK;
}

const char *openvk_ovk_version(openvk_data_t *data) {
    const char *resp_buf;
    int ret = openvk_call(data, "ovk.version", &resp_buf, false, 0);
    if (ret) {
        return NULL;
    }

    json_t *resp;
    openvk_get_resp(resp_buf, &resp);
    if (!resp) return NULL;

    return strdup(json_string_value(resp));
}