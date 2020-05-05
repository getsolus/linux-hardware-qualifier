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

#include "lhq_list.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/* Get a pointer to the next free entry at the end of the list

   @param list  - the list to get the entry from
   @returns pointer to the next entry
*/
void *lhq_list_next(LHQ_LIST *list) { return (void *)(((uint8_t *)list->data) + (list->length * list->elementSize)); }

/* Mark the next entry as added

   @param list  - the list to modify
*/
void lhq_list_inc(LHQ_LIST *list) {
    list->length++;
    if(list->length == list->capacity) {
        list->capacity <<= 1;
        list->data = realloc(list->data, list->capacity * list->elementSize);
    }
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
