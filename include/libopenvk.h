#pragma once
#include <curl/curl.h>
#include <jansson.h>
#include "libopenvk_responses.h"

// some helper macros

#define JSON_STR(dict, key) json_string_value(json_object_get(dict, key))
#define JSON_INT(dict, key) json_integer_value(json_object_get(dict, key))

typedef struct openvk_data {
    const char *instance;
    const char *user_agent;
    const char *token;
} openvk_data_t;

enum LIBOVK_ERROR {
    OVK_API_OK = 0,
    OVK_API_ERROR,
    OVK_API_NET_ERROR,
    OVK_API_JSON_PARSE_ERROR
};

// From api.c

/**
 * openvk_init: Allocate and initialize openvk_data_t struct
 * @param data: Pointer to data pointer
 * @returns OVK_API_OK, other value on error
 */
int openvk_init(openvk_data_t **data);

/**
 * openvk_free: Free openvk_data_t struct
 * @param data: openvk_data_t struct
 */
void openvk_free(openvk_data_t *data);

/**
 * openvk_set_instance: Sets instance api base url(without /method) to strdupped version
 * @param data: data struct
 * @param api_base_url: Instance api base url
 * @returns OVK_API_OK on success
 */
int openvk_set_instance(openvk_data_t *data, const char *api_base_url);

/**
 * openvk_set_token: Sets auth token to use to strdupped version
 * @param data: data struct
 * @param token: auth token
 * @returns OVK_API_OK on success
 */
int openvk_set_token(openvk_data_t *data, const char *token);

/**
 * openvk_call: Calls openvk method
 * @param data: data struct
 * @param method: method to call
 * @param resp_buf: pointer to resp_buf(gets allocated by function)
 * @param use_token: use token in request
 * @param params: request paramets(can be set to 0)
 * @returns OVK_API_OK on success
 */
int openvk_call(openvk_data_t *data, const char *method, const char **resp_buf, bool use_token, const char *params);

/**
 * openvk_auth: Perform authentication and set token to use
 * @param data: data struct
 * @param user: Username
 * @param password: Password
 * @param two_fac_code: (Optional)2FA code
 * @param is_roamin: https://openvk.github.io/docs/openvk_engine/api/authorization/#roaming
 * @returns OVK_API_OK  on success
 */
int openvk_auth(openvk_data_t *data, const char *user, const char *password, const char *two_fac_code, bool is_roaming);

/**
 * openvk_get_resp: Parses json and returns response node in resp_node
 * @param resp: response buffer from openvk_call
 * @param resp_node: pointer to resp_node
 * @returns OVK_API_OK on success
 */
int openvk_get_resp(const char *resp, json_t **resp_node);

// From classes/account.c

/**
 * openvk_account_getProfileInfo: calls api method account.getProfileInfo, parses response and returns profile info in struct ovk_getProfileInfo
 * @param data: data struct
 * @param out: pointer to struct to store out
 * @returns OVK_API_OK and profile info in out on sucess
 */
int openvk_account_getProfileInfo(openvk_data_t *data, struct ovk_getProfileInfo *out);

/**
 * openvk_account_setOnline: calls api method account.setOnline
 * @param data: data struct
 * @return OVK_API_OK on success
 */
int openvk_account_setOnline(openvk_data_t *data);

/**
 * opemvk_account_getCounters: calls api method account.getCounters
 * @param data: data struct
 * @param out: pointer to struct to store results
 * @returns OVK_API_OK on success
 */
int openvk_account_getCounters(openvk_data_t *data, struct ovk_acc_getCounters *out);

// From classes/audio.c

/**
 * openvk_audio_get: calls api method audio.get with specified parameters and returs parsed data in ovk_audio_get. You can set one parameter to 0 to use default value
 * @param data: data struct
 * (you can check descriptions of parameters in https://openvk.github.io/docs/openvk_engine/api/methods/audio/get/)
 * @param out: pointer to struct to store data
 * @returns OVK_API_OK on success
 */
int openvk_audio_get(openvk_data_t *data, 
                                    int owner_id, 
                                    int album_id, 
                                    bool audio_ids, 
                                    unsigned int offset, 
                                    unsigned int count, 
                                    bool uploaded_only,
                                    struct ovk_audio_get *out);


// From classes/ovk.c

/**
 * openvk_ovk_aboutInstance: calls api method ovk.aboutInstance, parses response and returns it in struct ovk_instanceInfo
 * @param data: data struct
 * @param out: pointer to struct to store data
 * @returns OVK_API_OK on success
 */
int openvk_ovk_aboutInstance(openvk_data_t *data, struct ovk_instanceInfo *out);

/**
 * openvk_ovk_version: calls api method ovk.version and returns instance version
 * @param data: data struct
 * @returns instance version
 */
const char *openvk_ovk_version(openvk_data_t *data);