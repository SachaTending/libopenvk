#pragma once
#include <curl/curl.h>
#include <jansson.h>
#include "libopenvk_responses.h"

typedef struct openvk_data {
    const char *instance;
    const char *user_agent;
} openvk_data_t;

// From api.c

/**
 * openvk_init: Allocate and initialize openvk_data_t struct
 * @param data: Pointer to data pointer
 * @returns 0 on success, other value on error
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
 * @returns 0 on success
 */
int openvk_set_instance(openvk_data_t *data, const char *api_base_url);

/**
 * openvk_call: Calls openvk method
 * @param data: data struct
 * @param method: method to call
 * @param resp_buf: pointer to resp_buf(gets allocated by function)
 * @returns 0 on success
 */
int openvk_call(openvk_data_t *data, const char *method, const char **resp_buf);

/**
 * openvk_get_resp: Parses json and returns response node in resp_node
 * @param resp: response buffer from openvk_call
 * @param resp_node: pointer to resp_node
 * @returns error info in json_error_t if something got wrong
 */
json_error_t openvk_get_resp(const char *resp, json_t **resp_node);

// From classes/ovk.c

/**
 * openvk_ovk_aboutInstance: calls api method ovk.aboutInstance, parses response and returns it in struct ovk_instanceInfo
 * @param data: data struct
 * @returns parsed data
 */
struct ovk_instanceInfo openvk_ovk_aboutInstance(openvk_data_t *data);

/**
 * openvk_ovk_version: calls api method ovk.version and returns instance version
 * @param data: data struct
 * @returns instance version
 */
const char *openvk_ovk_version(openvk_data_t *data);