/*
 * Copyright (c) 2014, 2016, 2017 Nicira, Inc.
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

#ifndef MC_WRAP_H
#define MC_WRAP_H 1

#include "compiler.h"
#include "jsonrpc.h"
#include "mc.h"
#include "ovs-thread.h"
#include "ovsdb-error.h"
#include "ovsdb/log.h"
#include "seq.h"
#include "unixctl.h"

/* Wrappers for library calls to allow the model checker to drive
 * the execution for testing
 */

struct ovsdb_error * mc_wrap_ovsdb_log_open(const char *name,
					    const char *magic,
					    enum ovsdb_log_open_mode open_mode,
					    int locking, struct ovsdb_log **filep,
					    struct jsonrpc *mc_conn,
					    int tid, const char *where)
    OVS_WARN_UNUSED_RESULT;

struct ovsdb_error * mc_wrap_ovsdb_log_read(struct ovsdb_log *file,
					    struct json **jsonp,
					    struct jsonrpc *mc_conn,
					    int tid, const char *where)
    OVS_WARN_UNUSED_RESULT;

struct ovsdb_error * mc_wrap_ovsdb_log_write(struct ovsdb_log *file,
					     const struct json *json,
					     struct jsonrpc *mc_conn,
					     int tid, const char *where)
    OVS_WARN_UNUSED_RESULT;

struct ovsdb_error * mc_wrap_ovsdb_log_commit(struct ovsdb_log *file,
					      struct jsonrpc *mc_conn,
					      int tid, const char *where)
    OVS_WARN_UNUSED_RESULT;

struct ovsdb_error * mc_wrap_ovsdb_log_replace_start(struct ovsdb_log *old,
						     struct ovsdb_log **newp,
						     struct jsonrpc *mc_conn,
						     int tid, const char *where)
    OVS_WARN_UNUSED_RESULT;


struct ovsdb_error * mc_wrap_ovsdb_log_replace_commit(struct ovsdb_log *old,
						      struct ovsdb_log *new,
						      struct jsonrpc *mc_conn,
						      int tid, const char *where)
    OVS_WARN_UNUSED_RESULT;

int mc_wrap_unixctl_client_create(const char *path, struct jsonrpc **client,
				  struct jsonrpc *mc_conn, int tid,
				  const char *where)
    OVS_WARN_UNUSED_RESULT;

int mc_wrap_unixctl_client_transact(struct jsonrpc *client,
				    const char *command,
				    int argc, char *argv[],
				    char **result, char **error,
				    struct jsonrpc *mc_conn, int tid,
				    const char *where)
    OVS_WARN_UNUSED_RESULT;

int mc_wrap_unixctl_server_create(const char *path,
				  struct unixctl_server **serverp,
				  struct jsonrpc *mc_conn, int tid,
				  const char *where)
    OVS_WARN_UNUSED_RESULT;

void mc_wrap_noexecute_server_transact(struct jsonrpc *mc_conn, int tid,
				       const char *where);

struct jsonrpc * mc_wrap_connect(char *mc_addr);

void mc_wrap_send_hello_or_bye(struct jsonrpc *mc_conn, enum mc_rpc_type type,
			       int tid, const char *where);

int mc_wrap_jsonrpc_session_send(struct jsonrpc_session *s,
				 struct jsonrpc_msg *msg,
				 struct jsonrpc *mc_conn,
				 int tid, const char *where);

struct jsonrpc_msg * mc_wrap_jsonrpc_session_recv(struct jsonrpc_session *s,
						  struct jsonrpc *mc_conn,
						  int tid, const char *where);

void mc_wrap_ovs_mutex_lock(const struct ovs_mutex *mutex,
			    struct jsonrpc *mc_conn,
			    int tid, const char *where);

void mc_wrap_ovs_mutex_unlock(const struct ovs_mutex *mutex,
			      struct jsonrpc *mc_conn,
			      int tid, const char *where);

void mc_wrap_poll_block(struct jsonrpc *mc_conn);
void mc_wrap_seq_wait(const struct seq *seq, uint64_t value,
		      struct jsonrpc *mc_conn, int tid, const char *where);
void mc_wrap_seq_change(struct seq *seq, struct jsonrpc *mc_conn, int tid,
			const char *where);
pthread_t mc_wrap_ovs_thread_create(const char *name, void *(*start)(void *),
				    void *arg, struct jsonrpc *mc_conn,
				    int tid, const char *where);
#endif /* tests/mc/mc_wrap.h */
