#include <libopenvk.h>
#include <string.h>
#include <stdio.h>

int openvk_audio_get(openvk_data_t *data, int owner_id, int album_id, bool audio_ids, unsigned int offset, unsigned int count, bool uploaded_only, struct ovk_audio_get *out) {
    char params[4096];
    #define add_param(...) snprintf(params+strlen(params), sizeof(params)-strlen(params), __VA_ARGS__);
    if (owner_id != -1) {
        //add_param("owner_id=%d&", owner_id);
        snprintf(params, 4096, "owner_id=%d&", owner_id);
    }
    if (album_id != -1) {
        add_param("album_id=%d&", album_id);
    } if (audio_ids) {
        add_param("audio_ids=1&");
    } if (offset) {
        add_param("offset=%u&", offset);
    } if (count) {
        add_param("count=%u&", count);
    } if (uploaded_only) {
        add_param("uploaded_only=1&");
    }

    *(strrchr(params, '&')) = 0;

    //printf(params);

    const char *resp_buf;
    openvk_call(data, "audio.get", &resp_buf, 1, params);
    printf(resp_buf);

    json_t *resp_node;
    int err = openvk_get_resp(resp_buf, &resp_node);
    if (!resp_node) {
        return OVK_API_ERROR;
    }

    int item_count = JSON_INT(resp_node, "count");
    struct ovk_audio_item *items = malloc(sizeof(struct ovk_audio_item)*item_count);
    memset(items, 0, sizeof(struct ovk_audio_item)*item_count);
    #define item_str(d, k) k = strdup(JSON_STR(d, #k))
    #define item_int(d, k) k = JSON_INT(d, #k)
    for (int i=0;i<item_count;i++) {
        json_t *audio = json_array_get(json_object_get(resp_node, "items"), i);
        items[i].item_str(audio, unique_id);
        items[i].item_int(audio, aid);
        items[i].item_int(audio, id);
        items[i].item_str(audio, artist);
        items[i].item_str(audio, title);
        items[i].item_int(audio, duration);
        items[i].item_str(audio, url);
        items[i].item_int(audio, genre);
        items[i].item_str(audio, genre_str);
        items[i].item_int(audio, owner_id);
        items[i].item_int(audio, lyrics_id);
        items[i].item_int(audio, editable);
        items[i].item_int(audio, ready);
    }

    #undef item_str
    #undef item_int

    out->count = item_count;
    out->audios = items;
    json_decref(resp_node);
    return OVK_API_OK;
}