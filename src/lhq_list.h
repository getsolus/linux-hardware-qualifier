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

#ifndef __LINUX_HARDWARE_QUALIFIER_LIST_H__
#define __LINUX_HARDWARE_QUALIFIER_LIST_H__

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#define LKDDB_LIST_START 1000
#define LKDDB_LIST_INCREMENT 1000

typedef struct {
    void *data;
    size_t elementSize;
    unsigned int length;
    unsigned int capacity;
} LKDDB_LIST;

LKDDB_LIST* lhq_list_new(size_t elementSize) {
    LKDDB_LIST *list = (LKDDB_LIST*)calloc(1,sizeof(LKDDB_LIST));
    list->data     = calloc(LKDDB_LIST_START,elementSize);
    list->elementSize = elementSize;
    list->length   = 0;
    list->capacity = LKDDB_LIST_START;
    return list;
}

void lhq_list_append(LKDDB_LIST* list, void *entry) {
    if(list->length == list->capacity) {
        list->capacity += LKDDB_LIST_INCREMENT;
        list->data = realloc(list->data, list->capacity*list->elementSize );
    }
    memcpy(((uint8_t*)list->data) + (list->length * list->elementSize), entry, list->elementSize);
    list->length++;
}

void lhq_list_free(LKDDB_LIST* list) {
    free(list->data);
    free(list);
}

#define LKDDB_LIST_DECLARE(type,entryType)                               \
                                                                         \
LKDDB_LIST* lhq_ ## type ## _list_new() {                                \
    return lhq_list_new(sizeof(entryType));                              \
}                                                                        \
                                                                         \
void lhq_ ## type ## _list_append(LKDDB_LIST* list, entryType *entry) {  \
    lhq_list_append(list, (void*)entry);                                 \
}                                                                        \
                                                                         \
void lhq_ ## type ## _list_print(LKDDB_LIST *list, FILE *out) {          \
    for(unsigned int i = 0; i < list->length; i++ ){                     \
        lhq_ ## type ## _entry_print(&((entryType*)list->data)[i], out); \
    }                                                                    \
}                                                                        \
                                                                         \
void lhq_ ## type ## _list_free(LKDDB_LIST* list) {                      \
    lhq_list_free(list);                                                 \
}                                                                        \

#endif
