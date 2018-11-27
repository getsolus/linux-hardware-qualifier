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

/* Print a summary of an index entry

   @param entry - the entry to print
   @param out   - the file to write to
*/
void lhq_index_entry_print(uint8_t **entry, FILE *out) {
    fprintf(out, "0x%016lx\n", (uint64_t)*entry);
}

/* declare the index list type */
LKDDB_LIST_DECLARE(index,uint8_t*)

/* Declare the LHQ_INDEX type

   @field raw       - the contents of the file for this index
   @field cursor    - pointer to the current location in raw
   @field rawLength - the nubmer of bytes in raw
   @field lists     - a list of lists for records in raw
   @field length    - the number of lists in lists
*/
typedef struct {
    uint8_t *raw;
    char *cursor;
    size_t rawLength;
    LKDDB_LIST **lists;
    unsigned int length;
} LHQ_INDEX;

LHQ_INDEX* lhq_index_new(unsigned int length, FILE *f) {
    fseek(f, 0, SEEK_END);
    size_t fLength = (size_t)ftell(f);
    rewind(f);
    LHQ_INDEX *index = (LHQ_INDEX*)calloc(1,sizeof(LHQ_INDEX));
    index->raw = (uint8_t*)calloc(fLength+1, sizeof(uint8_t));
    index->cursor = (char*)index->raw;
    index->raw[fLength] = '\0';
    if( index->raw == NULL ) {
        fprintf(stderr, "Failed to alloc space to read file. Exiting.\n");
        return NULL;
    }
    index->rawLength = fLength;
    if( fread(index->raw, sizeof(uint8_t), fLength, f) != fLength ) {
        free(index->raw);
        free(index);
        fprintf(stderr, "Failed to read all of file. Exiting.\n");
        return NULL;
    }
    index->lists = (LKDDB_LIST**)calloc(length, sizeof(LKDDB_LIST*));
    index->length = length;
    for( unsigned int i = 0; i < length; i++ ) {
        index->lists[i] = lhq_index_list_new();
    }
    return index;
}

void lhq_index_summary(LHQ_INDEX* index) {
    for( unsigned int i = 0; i < index->length; i++) {
        fprintf(stderr, "List[%d]: %d %d\n", i, index->lists[i]->length, index->lists[i]->capacity);
    }
}

void lhq_index_free(LHQ_INDEX* index) {
    free(index->raw);
    for( unsigned int i = 0; i < index->length; i++ ) {
        lhq_list_free(index->lists[i]);
    }
    free(index->lists);
    free(index);
}

#endif
