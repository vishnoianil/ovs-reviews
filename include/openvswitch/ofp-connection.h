/*
 * Copyright (c) 2008-2017 Nicira, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OPENVSWITCH_OFP_CONNECTION_H
#define OPENVSWITCH_OFP_CONNECTION_H 1

#include "openflow/openflow.h"
#include "openvswitch/ofp-protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Abstract ofp_role_request and reply. */
struct ofpconn_role_request {
    enum ofp12_controller_role role;
    bool have_generation_id;
    uint64_t generation_id;
};

struct ofpconn_role_status {
    enum ofp12_controller_role role;
    enum ofp14_controller_role_reason reason;
    uint64_t generation_id;
};

enum ofperr ofpconn_decode_role_message(const struct ofp_header *,
                                        struct ofpconn_role_request *);
struct ofpbuf *ofpconn_encode_role_reply(const struct ofp_header *,
                                         const struct ofpconn_role_request *);

struct ofpbuf *ofpconn_encode_role_status(const struct ofpconn_role_status *,
                                          enum ofputil_protocol);

enum ofperr ofpconn_decode_role_status(const struct ofp_header *,
                                       struct ofpconn_role_status *);

enum ofpconn_async_msg_type {
    /* Standard asynchronous messages. */
    OAM_PACKET_IN,              /* OFPT_PACKET_IN or NXT_PACKET_IN. */
    OAM_PORT_STATUS,            /* OFPT_PORT_STATUS. */
    OAM_FLOW_REMOVED,           /* OFPT_FLOW_REMOVED or NXT_FLOW_REMOVED. */
    OAM_ROLE_STATUS,            /* OFPT_ROLE_STATUS. */
    OAM_TABLE_STATUS,           /* OFPT_TABLE_STATUS. */
    OAM_REQUESTFORWARD,         /* OFPT_REQUESTFORWARD. */

    /* Extension asynchronous messages (none yet--coming soon!). */
#define OAM_EXTENSIONS 0        /* Bitmap of all extensions. */

    OAM_N_TYPES
};
const char *ofpconn_async_msg_type_to_string(enum ofpconn_async_msg_type);

struct ofpconn_async_cfg {
    uint32_t master[OAM_N_TYPES];
    uint32_t slave[OAM_N_TYPES];
};
#define OFPCONN_ASYNC_CFG_INIT (struct ofpconn_async_cfg) { .master[0] = 0 }

enum ofperr ofpconn_decode_set_async_config(const struct ofp_header *,
                                            bool loose,
                                            const struct ofpconn_async_cfg *,
                                            struct ofpconn_async_cfg *);

struct ofpbuf *ofpconn_encode_get_async_reply(
    const struct ofp_header *, const struct ofpconn_async_cfg *);
struct ofpbuf *ofpconn_encode_set_async_config(
    const struct ofpconn_async_cfg *, uint32_t oams, enum ofp_version);

struct ofpconn_async_cfg ofpconn_async_cfg_default(enum ofp_version);

#ifdef __cplusplus
}
#endif

#endif  /* ofp-connection.h */
