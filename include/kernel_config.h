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

#ifndef __LINUX_HARDWARE_QUALIFIER_KERNEL_CONFIG_H__
#define __LINUX_HARDWARE_QUALIFIER_KERNEL_CONFIG_H__

#include "kernel_flag.h"

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

/* Declare the LHQ_KERNEL_CONFIG type

   @field raw       - the contents of the file for this index
   @field cursor    - pointer to the current location in raw
   @field rawLength - the nubmer of bytes in raw
   @field flags     - a list of pointers to flags in raw
*/
typedef struct {
    uint8_t *raw;
    char *cursor;
    size_t rawLength;
    LHQ_LIST *flags;
} LHQ_KERNEL_CONFIG;

/* Create a Kernel Config from a file

   @param f - the file to read from
   @returns NULL on failure, pointer to flag on success
*/
LHQ_KERNEL_CONFIG *lhq_kernel_config_new(FILE *f) {
    fseek(f, 0, SEEK_END);
    size_t length = (size_t)ftell(f);
    rewind(f);
    LHQ_KERNEL_CONFIG *config = (LHQ_KERNEL_CONFIG *)calloc(1, sizeof(LHQ_KERNEL_CONFIG));
    config->raw               = (uint8_t *)calloc(length + 1, sizeof(uint8_t));
    config->cursor            = (char *)config->raw;
    config->raw[length]       = '\0';
    if(config->raw == NULL) {
        fprintf(stderr, "Failed to alloc space to read file. Exiting.\n");
        return NULL;
    }
    config->rawLength = length;
    if(fread(config->raw, sizeof(uint8_t), length, f) != length) {
        free(config->raw);
        free(config);
        fprintf(stderr, "Failed to read all of file. Exiting.\n");
        return NULL;
    }
    config->flags = lhq_kernel_flag_list_new();
    return config;
}

/* Print a summary of the kernel config

   @param config - the kernel config to summarize
*/
void lhq_kernel_config_summary(LHQ_KERNEL_CONFIG *config) {
    fprintf(stderr, "List: %d %d\n", config->flags->length, config->flags->capacity);
}

/* Parse flags from the raw config

   @param config - the config to parse
*/
static void lhq_kernel_flags_parse(LHQ_KERNEL_CONFIG *config) {
    LHQ_LIST *list         = config->flags;
    LHQ_KERNEL_FLAG *entry = (LHQ_KERNEL_FLAG *)lhq_list_next(list);
    while(lhq_kernel_flag_parse(entry, &(config->cursor)) == 1) {
        lhq_list_inc(list);
        entry = (LHQ_KERNEL_FLAG *)lhq_list_next(list);
    }
    if(*config->cursor != '\0') {
        lhq_list_inc(list);
    }
    lhq_list_compact(list);
#if LHQ_DEBUG > 0
    fprintf(stderr, "Length: %d, Capacity: %d\n", list->length, list->capacity);
    lhq_kernel_flag_list_print(list, stderr);
#endif
}

/* Parse all supported flags into the kernel config

   @param config - the config to fill
*/
void lhq_kernel_config_populate(LHQ_KERNEL_CONFIG *config) { lhq_kernel_flags_parse(config); }

/* Search for a matching entry, copy pointers from it if found

   @param config - the config to search in
   @param entry - the entry to copy to
   @param start - index to start from
   @returns index of the match or the length of the list if not found
*/
unsigned int lhq_kernel_flag_search_and_copy(LHQ_KERNEL_CONFIG *config, LHQ_KERNEL_FLAG *entry, unsigned int start) {
    LHQ_LIST *list       = config->flags;
    LHQ_KERNEL_FLAG *ids = (LHQ_KERNEL_FLAG *)list->data;
    unsigned int i       = start;
    for(; i < list->length; i++) {
        if(lhq_kernel_flag_compare_and_copy(entry, &ids[i]) == 0) {
            break;
        }
    }
    return i;
}

/* Destroy a kernel config

   @param config - the config to destroy
*/
void lhq_kernel_config_free(LHQ_KERNEL_CONFIG *config) {
    free(config->raw);
    lhq_list_free(config->flags);
    free(config);
}

#endif
