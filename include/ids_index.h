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

#ifndef __LINUX_HARDWARE_QUALIFIER_IDS_INDEX_H__
#define __LINUX_HARDWARE_QUALIFIER_IDS_INDEX_H__

#include "lhq_list.h"
#include "lhq_types.h"

#include "pci_class_ids.h"
#include "pci_ids.h"
#include "usb_class_ids.h"
#include "usb_ids.h"

#include <inttypes.h>
#include <stdio.h>

/* Declare the LHQ_IDS_INDEX type

   @field raw       - the contents of the file for this index
   @field cursor    - pointer to the current location in raw
   @field rawLength - the nubmer of bytes in raw
   @field lists     - a list of lists for records in raw
*/
typedef struct {
    uint8_t *raw;
    char *cursor;
    size_t rawLength;
    LHQ_LIST *lists[LHQ_ID_COUNT];
} LHQ_IDS_INDEX;

/* Create a IDs Index from a file

   @param f - the file to read from
   @returns NULL on failure, pointer to Index on success
*/
LHQ_IDS_INDEX *lhq_ids_index_new(FILE *f);

/* Print a summary of the index

   @param index - the index to summarize
*/
void lhq_ids_index_summary(LHQ_IDS_INDEX *index);

/* Parse all supported IDs into the index

   @param index - the index to fill
*/
void lhq_ids_index_populate(LHQ_IDS_INDEX *index);

/* Search for a matching entry, copy pointers from it if found

   @param index - the index to search in
   @param entry - the entry to copy to
   @param start - index to start from
   @returns index of the match or the length of the list if not found
*/
unsigned int lhq_pci_id_search_and_copy(LHQ_IDS_INDEX *index, LKDDB_PCI_ID *entry, unsigned int start);

/* Search for a matching entry, copy pointers from it if found

   @param index - the index to search in
   @param entry - the entry to copy to
   @param start - index to start from
   @returns index of the match or the length of the list if not found
*/
unsigned int lhq_pci_class_id_search_and_copy(LHQ_IDS_INDEX *index, LKDDB_PCI_CLASS_ID *entry, unsigned int start);

/* Search for a matching entry, copy pointers from it if found

   @param index - the index to search in
   @param entry - the entry to copy to
   @param start - index to start from
   @returns index of the match or the length of the list if not found
*/
unsigned int lhq_usb_id_search_and_copy(LHQ_IDS_INDEX *index, LKDDB_USB_ID *entry, unsigned int start);

/* Search for a matching entry, copy pointers from it if found

   @param index - the index to search in
   @param entry - the entry to copy to
   @param start - index to start from
   @returns index of the match or the length of the list if not found
*/
unsigned int lhq_usb_class_id_search_and_copy(LHQ_IDS_INDEX *index, LKDDB_USB_CLASS_ID *entry, unsigned int start);

/* Destroy an index

   @param index - the index to destroy
*/
void lhq_ids_index_free(LHQ_IDS_INDEX *index);

#endif
