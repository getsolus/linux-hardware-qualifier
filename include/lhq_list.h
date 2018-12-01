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

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* starting capacity of a list */
#define LHQ_LIST_START 128

/* List type for this project

   @field data        - the actual array of elements
   @field elementSize - the size of an element
   @field length      - the number of elements
   @field capacity    - the number of allocated elements
*/
typedef struct {
    void *data;
    size_t elementSize;
    unsigned int length;
    unsigned int capacity;
} LHQ_LIST;

/* Create a new list

   @param elementSize - the size of each eleement in the list
   @returns a pointer to the new list
*/
LHQ_LIST *lhq_list_new(size_t elementSize) {
    LHQ_LIST *list    = (LHQ_LIST *)calloc(1, sizeof(LHQ_LIST));
    list->data        = calloc(LHQ_LIST_START, elementSize);
    list->elementSize = elementSize;
    list->length      = 0;
    list->capacity    = LHQ_LIST_START;
    return list;
}

/* Append a new item to the end of the list

   @param list  - the list to append to
   @param entry - the item to add
*/
void lhq_list_append(LHQ_LIST *list, void *entry) {
    if(list->length == list->capacity) {
        list->capacity <<= 1;
        list->data = realloc(list->data, list->capacity * list->elementSize);
    }
    memcpy(((uint8_t *)list->data) + (list->length * list->elementSize), entry, list->elementSize);
    list->length++;
}

/* Shrink the list to only use enough memory for current elements

   @param list - the list to shrink
*/
void lhq_list_compact(LHQ_LIST *list) {
    list->data     = realloc(list->data, list->length * list->elementSize);
    list->capacity = list->length;
}

/* Destroy the list

   @param list - the list to free
*/
void lhq_list_free(LHQ_LIST *list) {
    free(list->data);
    free(list);
}

/* Macro to declare convenience functions for lists of different types */
#define LHQ_LIST_DECLARE(type, entryType)                                         \
                                                                                  \
    LHQ_LIST *lhq_##type##_list_new() { return lhq_list_new(sizeof(entryType)); } \
                                                                                  \
    void lhq_##type##_list_print(LHQ_LIST *list, FILE *out) {                     \
        for(unsigned int i = 0; i < list->length; i++) {                          \
            lhq_##type##_entry_print(&((entryType *)list->data)[i], out);         \
        }                                                                         \
    }

#endif
