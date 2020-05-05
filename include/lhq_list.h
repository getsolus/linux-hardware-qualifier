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

#ifndef __LINUX_HARDWARE_QUALIFER_LIST_H__
#define __LINUX_HARDWARE_QUALIFER_LIST_H__

#include <stdlib.h>

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
LHQ_LIST *lhq_list_new(size_t elementSize);

/* Get a pointer to the next free entry at the end of the list

   @param list  - the list to get the entry from
   @returns pointer to the next entry
*/
void *lhq_list_next(LHQ_LIST *list);

/* Mark the next entry as added

   @param list  - the list to modify
*/
void lhq_list_inc(LHQ_LIST *list);

/* Shrink the list to only use enough memory for current elements

   @param list - the list to shrink
*/
void lhq_list_compact(LHQ_LIST *list);

/* Destroy the list

   @param list - the list to free
*/
void lhq_list_free(LHQ_LIST *list);

/* Macro to declare convenience functions for lists of different types */
#define LHQ_LIST_DECLARE(type, entryType)                                         \
                                                                                  \
    LHQ_LIST *lhq_##type##_list_new(void);                                        \

/* Macro to define convenience functions for lists of different types */
#define LHQ_LIST_DEFINE(type, entryType)                                          \
                                                                                  \
    LHQ_LIST *lhq_##type##_list_new() { return lhq_list_new(sizeof(entryType)); } \

#endif
