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

struct ovk_getProfileInfo {
    const char *first_name;
    const char *last_name;
    const char *photo_200; // link to a pfp
    const char *nickname;
    bool is_service_account;
    long int id;
    bool is_verified;
    const char *verification_status;
    const char *status;
    const char *bdate;
    int bdate_visibility;
    int relation;
    const char *screen_name;
    int sex;
};

struct ovk_audio_item {
    const char *unique_id;
    int aid;
    int id;
    const char *artist;
    const char *title;
    int duration;
    const char *url;
    int genre;
    const char *genre_str;
    int owner_id;
    int lyrics_id;
    bool ready;
    bool editable;
    // TODO: Add more
};

struct ovk_audio_get {
    int count;
    struct ovk_audio_item *audios;
};