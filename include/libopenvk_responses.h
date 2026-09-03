#pragma once
#include <stdbool.h>

struct ovk_user {
    int id;
    const char *first_name;
    const char *last_name;
    bool is_closed;
    bool can_access_closed;
    bool online;
};

struct ovk_link {
    const char *name;
    const char *link;
};

struct ovk_instanceInfo {
    struct {
        int users_count;
        int online_users_count;
        int active_users_count;
        int groups_count;
        int wall_posts_count;
    } stats;

    struct {
        int count;
        struct ovk_user *users;
    } admins;

    struct {

    } popular_groups; // TODO

    struct {
        int count;
        struct ovk_link *links;
    } links;
};