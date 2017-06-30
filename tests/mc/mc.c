/*
 * Copyright (c) 2016, 2017 Nicira, Inc.
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

#include <config.h>
#include "jsonrpc.h"
#include "mc.h"
#include "openvswitch/dynamic-string.h"
#include "openvswitch/json.h"
#include "openvswitch/list.h"
#include "openvswitch/vlog.h"
#include "openvswitch/util.h"
#include "process.h"
#include "util.h"

VLOG_DEFINE_THIS_MODULE(mc);

struct mc_process {
    char* name;
    struct jsonrpc_session *js;
    struct ovs_list list_node;
    struct process *proc_ptr;
    struct uuid sid;
    bool failure_inject;
};

static struct ovs_list mc_processes = OVS_LIST_INITIALIZER(&mc_processes);

static void
start_processes(struct json *config) {

    ovs_assert(config->type == JSON_OBJECT);
    
    struct json *exec_conf = shash_find_data(config->u.object,
					     "model_check_execute");
    
    if (exec_conf == NULL) {
	ovs_fatal(0, "Cannot find the execute config");
    }

    ovs_assert(exec_conf->type == JSON_ARRAY);

    struct mc_process *new_proc;
    for (int i = 0; i < exec_conf->u.array.n; i++) {
	struct shash_node *exe =
	    shash_first(exec_conf->u.array.elems[i]->u.object);
	new_proc = xmalloc(sizeof(struct mc_process));
	new_proc->name = xmalloc(strlen(exe->name));
	strcpy(new_proc->name, exe->name);

	struct json *exe_data = exe->data;
	exe_data = shash_find_data(exe_data->u.object, "command");

	if (exe_data == NULL) {
	    ovs_fatal(0, "Did not find command for %s\n", exe->name);
	}
	
	char **args = xmalloc(sizeof(char*) * (exe_data->u.array.n + 1));
	int j = 0;
	for (; j < exe_data->u.array.n; j++) {
	    args[j] = xmalloc(strlen(exe_data->u.array.elems[j]->u.string));
	    strcpy(args[j], exe_data->u.array.elems[j]->u.string);
	    printf("%s\n", args[j]);
	}
	args[j] = NULL;

	int errno = process_start(args, &(new_proc->proc_ptr));
	if (errno != 0) {
	    ovs_fatal(errno, "Cannot start process %s", new_proc->name);
	}

	exe_data = exe->data;
	exe_data = shash_find_data(exe_data->u.object, "failure_inject");
	if (exe_data == NULL ||
	    !(exe_data->type == JSON_TRUE || exe_data->type == JSON_FALSE)) {

	    ovs_fatal(0,
		      "Did not find failure injection information for %s\n",
		      exe->name);
	} else if (exe_data->type == JSON_TRUE) {
	    new_proc->failure_inject = true;
	} else {
	    new_proc->failure_inject = false;
	}
    }
}
    
int
main(int argc, char *argv[])
{
    if (argc < 2) {
	ovs_fatal(0, "Usage is ./mc <configfile>. Not enough arguments provided");
    }

    struct json *config = json_from_file(argv[1]);

    if (config->type == JSON_STRING) {
	ovs_fatal(0, "Cannot read the json config in %s\n%s", argv[1], config->u.string);
    }

    start_processes(config);
    
    return 0;
}
