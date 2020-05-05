/*
 * Copyright 2018-2019 Solus Project <copyright@getsol.us>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http: *www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "acpi.h"

#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

/* Search for all of the system ACPI devices and look them un in the LKDDB

   @param results - the list of results to save to
*/
int lhq_acpi_find_devices(LHQ_LIST *results) {
    DIR *devices = opendir("/sys/bus/acpi/devices");
    if(devices == NULL) {
        return -1;
    }
    struct dirent *entry = readdir(devices);
    while(entry != NULL) {
        if((entry->d_type == DT_LNK) && (strncmp(entry->d_name, "device", 6) != 0)) {
            LHQ_ACPI_RESULT *result = lhq_list_next(results);
            char *end               = strchr(entry->d_name, ':');
            result->entry.id        = (char *)calloc((size_t)(end - entry->d_name + 1), sizeof(char));
            strncpy(result->entry.id, entry->d_name, (size_t)(end - entry->d_name));
            LHQ_ACPI_RESULT *prev = (LHQ_ACPI_RESULT *)results->data;
            int ok                = 1;
            for(unsigned int i = 0; (i < results->length) && (ok == 1); i++) {
                if(strcmp(prev[i].entry.id, result->entry.id) == 0) {
                    ok = 0;
                }
            }
            if(ok == 1) {
                lhq_list_inc(results);
            } else {
                free(result->entry.id);
                result->entry.id = NULL;
            }
        }
        entry = readdir(devices);
    }
    return closedir(devices);
}
