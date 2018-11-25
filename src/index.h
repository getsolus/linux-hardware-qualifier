/*
 * Copyright 2018 Solus Project <copyright@getsol.us>
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

#ifndef __LINUX_HARDWARE_QUALIFIER_INDEX_H__
#define __LINUX_HARDWARE_QUALIFIER_INDEX_H__

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "lhq_list.h"

void lhq_index_entry_print(uint8_t **entry, FILE *out) {
    fprintf(out, "0x%016lx\n", (uint64_t)*entry);
}

LKDDB_LIST_DECLARE(index,uint8_t*)

typedef struct {
    uint8_t *raw;
    size_t rawLength;
    LKDDB_LIST **lists;
    unsigned int length;
} LHQ_INDEX;

LHQ_INDEX* lhq_index_new(unsigned int length, char * filepath) {
    FILE *f = fopen(filepath, "r");
    if(f == NULL) {
        fprintf(stderr, "Failed to open '%s'. Exiting.\n", filepath);
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    size_t fLength = (size_t)ftell(f);
    rewind(f);
    LHQ_INDEX *index = (LHQ_INDEX*)calloc(1,sizeof(LHQ_INDEX));
    index->raw = (uint8_t*)calloc(fLength+1, sizeof(uint8_t));
    index->raw[fLength] = '\0';
    if( index->raw == NULL ) {
        fprintf(stderr, "Failed to alloc space to read '%s'. Exiting.\n", filepath);
        return NULL;
    }
    index->rawLength = fLength;
    if( fread(index->raw, sizeof(uint8_t), fLength, f) != fLength ) {
        free(index->raw);
        free(index);
        fprintf(stderr, "Failed to read all of '%s'. Exiting.\n", filepath);
        return NULL;
    }
    fclose(f);
    index->lists = (LKDDB_LIST**)calloc(length, sizeof(LKDDB_LIST*));
    index->length = length;
    for(unsigned int i = 0; i < index->length; i++) {
        index->lists[i] = lhq_index_list_new();
    }
    return index;
}

void lhq_index_populate(LHQ_INDEX* index, int listID, char *prefix, unsigned long pLength) {
    char *curr = (char*)index->raw;
    if( strncmp(curr,prefix, pLength) == 0 ) {
        printf("Found '%s' at index '%d'\n", prefix, 0);
    }
    for( unsigned int i = 1; i < index->rawLength; i++) {
        if( *(curr-1) == '\n') {
            if( strncmp(curr,prefix, pLength) == 0) {
                printf("Found '%s' at index '%d'\n", prefix, i+1);
            }
        }
        curr++;
    }
}

void lhq_index_free(LHQ_INDEX* index) {
    for(unsigned int i = 0; i < index->length; i++) {
        lhq_index_list_free(index->lists[i]);
    }
    free(index->raw);
    free(index->lists);
    free(index);
}

#endif
