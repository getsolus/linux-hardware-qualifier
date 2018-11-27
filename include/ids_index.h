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

#ifndef __LINUX_HARDWARE_QUALIFIER_IDS_INDEX_H__
#define __LINUX_HARDWARE_QUALIFIER_IDS_INDEX_H__

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "lhq_list.h"
#include "lhq_types.h"
#include "pci_class_ids.h"
#include "pci_ids.h"
#include "usb_class_ids.h"
#include "usb_ids.h"

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
    LKDDB_LIST *lists[LHQ_ID_COUNT];
} LHQ_IDS_INDEX;

/* Create a IDs Index from a file

   @param f - the file to read from
   @returns NULL on failure, pointer to Index on success
*/
LHQ_IDS_INDEX* lhq_ids_index_new(FILE *f) {
    fseek(f, 0, SEEK_END);
    size_t length = (size_t)ftell(f);
    rewind(f);
    LHQ_IDS_INDEX *index = (LHQ_IDS_INDEX*)calloc(1,sizeof(LHQ_IDS_INDEX));
    index->raw = (uint8_t*)calloc(length+1, sizeof(uint8_t));
    index->cursor = (char*)index->raw;
    index->raw[length] = '\0';
    if( index->raw == NULL ) {
        fprintf(stderr, "Failed to alloc space to read file. Exiting.\n");
        return NULL;
    }
    index->rawLength = length;
    if( fread(index->raw, sizeof(uint8_t), length, f) != length ) {
        free(index->raw);
        free(index);
        fprintf(stderr, "Failed to read all of file. Exiting.\n");
        return NULL;
    }
    index->lists[LHQ_ID_PCI_CLASS]  = lhq_pci_class_id_list_new();
    index->lists[LHQ_ID_PCI]        = lhq_pci_id_list_new();
    index->lists[LHQ_ID_USB_CLASS]  = lhq_usb_class_id_list_new();
    index->lists[LHQ_ID_USB]        = lhq_usb_class_id_list_new();
    return index;
}

/* Print a summary of the index

   @param index - the index to summarize
*/
void lhq_ids_index_summary(LHQ_IDS_INDEX* index) {
    for( unsigned int i = 0; i < LHQ_ID_COUNT; i++) {
        fprintf(stderr, "List[%d]: %d %d\n", i, index->lists[i]->length, index->lists[i]->capacity);
    }
}

/* Parse of the PCI Class IDs in the provided Index

   @param index - the index to read from
*/
void lhq_pci_class_ids_parse(LHQ_IDS_INDEX *index) {
    LKDDB_PCI_CLASS_ID entry;
    LKDDB_LIST *list = index->lists[LHQ_ID_PCI_CLASS];
    index->cursor = strstr(index->cursor, "\npci_class_ids");
    while( lhq_pci_class_id_entry_parse(&entry, &(index->cursor)) ) {
        lhq_list_append(list, (void*)&entry);
    }
    lhq_list_append(list, (void*)&entry);
    lhq_list_compact(list);
#ifdef LHQ_DEBUG
#if LHQ_DEBUG > 0
    fprintf(stderr, "Length: %d, Capacity: %d\n", list->length, list->capacity);
    lhq_pci_class_id_list_print(list,stderr);
#endif
#endif
}

/* Parse all of the PCI IDs in the provided Index

   @param index - the index to read from
*/
void lhq_pci_ids_parse(LHQ_IDS_INDEX *index) {
    LKDDB_PCI_ID entry;
    LKDDB_LIST *list = index->lists[LHQ_ID_PCI];
    index->cursor = strstr(index->cursor, "\npci_ids");
    while( lhq_pci_id_entry_parse(&entry, &(index->cursor)) ){
        lhq_list_append(list, (void*)&entry);
    }
    lhq_list_append(list, (void*)&entry);
    lhq_list_compact(list);
#ifdef LHQ_DEBUG
#if LHQ_DEBUG > 0
    fprintf(stderr, "Length: %d, Capacity: %d\n", list->length, list->capacity);
    lhq_pci_id_list_print(list,stderr);
#endif
#endif
}

/* Parse all USB Class IDS from the Index

   @param index - the index to populate
*/
void lhq_usb_class_ids_parse(LHQ_IDS_INDEX* index) {
    LKDDB_USB_CLASS_ID entry;
    LKDDB_LIST *list = index->lists[LHQ_ID_USB_CLASS];
    index->cursor = strstr(index->cursor, "\nusb_class_ids");
    while( lhq_usb_class_id_entry_parse(&entry, &(index->cursor)) ){
        lhq_list_append(list, (void*)&entry);
    }
    lhq_list_append(list, (void*)&entry);
    lhq_list_compact(list);
#ifdef LHQ_DEBUG
#if LHQ_DEBUG > 0
    fprintf(stderr, "Length: %d, Capacity: %d\n", list->length, list->capacity);
    lhq_usb_class_id_list_print(list,stderr);
#endif
#endif
}

/* Parse USB IDS from index

   @param index - the index to parse
*/
void lhq_usb_ids_parse(LHQ_IDS_INDEX *index) {
    LKDDB_USB_ID entry;
    LKDDB_LIST *list = index->lists[LHQ_ID_USB];
    index->cursor = strstr(index->cursor, "\nusb_ids");
    while( lhq_usb_id_entry_parse(&entry, &(index->cursor)) ) {
        lhq_list_append(list, (void*)&entry);
    }
    lhq_list_append(list, (void*)&entry);
    lhq_list_compact(list);
#ifdef LHQ_DEBUG
#if LHQ_DEBUG > 0
    fprintf(stderr, "Length: %d, Capacity: %d\n", list->length, list->capacity);
    lhq_usb_id_list_print(list,stderr);
#endif
#endif
}

/* Parse all supported IDs into the index

   @param index - the index to fill
*/
void lhq_ids_index_populate(LHQ_IDS_INDEX* index){
    lhq_pci_class_ids_parse(index);
    lhq_pci_ids_parse(index);
    lhq_usb_class_ids_parse(index);
    lhq_usb_ids_parse(index);
}

/* Destroy an index

   @param index - the index to destroy
*/
void lhq_ids_index_free(LHQ_IDS_INDEX* index) {
    free(index->raw);
    for( unsigned int i = 0; i < LHQ_ID_COUNT; i++) {
        lhq_list_free(index->lists[i]);
    }
    free(index);
}

#endif