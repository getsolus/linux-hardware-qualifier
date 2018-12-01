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

#ifndef __LINUX_HARDWARE_QUALIFIER_TYPES_INDEX_H__
#define __LINUX_HARDWARE_QUALIFIER_TYPES_INDEX_H__

#include "acpi.h"
#include "lhq_list.h"
#include "lhq_types.h"
#include "pci.h"
#include "usb.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

/* Declare the LHQ_TYPES_INDEX type

   @field raw       - the contents of the file for this index
   @field cursor    - pointer to the current location in raw
   @field rawLength - the nubmer of bytes in raw
   @field lists     - a list of lists for records in raw
*/
typedef struct {
    uint8_t *raw;
    char *cursor;
    size_t rawLength;
    LHQ_LIST *lists[LHQ_TYPE_COUNT];
} LHQ_TYPES_INDEX;

/* Create a Types Index from a file

   @param f - the file to read from
   @returns NULL on failure, pointer to Index on success
*/
LHQ_TYPES_INDEX *lhq_types_index_new(FILE *f) {
    fseek(f, 0, SEEK_END);
    size_t length = (size_t)ftell(f);
    rewind(f);
    LHQ_TYPES_INDEX *index = (LHQ_TYPES_INDEX *)calloc(1, sizeof(LHQ_TYPES_INDEX));
    index->raw             = (uint8_t *)calloc(length + 1, sizeof(uint8_t));
    index->cursor          = (char *)index->raw;
    index->raw[length]     = '\0';
    if(index->raw == NULL) {
        fprintf(stderr, "Failed to alloc space to read file. Exiting.\n");
        return NULL;
    }
    index->rawLength = length;
    if(fread(index->raw, sizeof(uint8_t), length, f) != length) {
        free(index->raw);
        free(index);
        fprintf(stderr, "Failed to read all of file. Exiting.\n");
        return NULL;
    }
    index->lists[LHQ_TYPE_ACPI] = lhq_acpi_list_new();
    index->lists[LHQ_TYPE_PCI]  = lhq_pci_list_new();
    index->lists[LHQ_TYPE_USB]  = lhq_usb_list_new();
    return index;
}

/* Print a summary of the index

   @param index - the index to summarize
*/
void lhq_types_index_summary(LHQ_TYPES_INDEX *index) {
    for(unsigned int i = 0; i < LHQ_TYPE_COUNT; i++) {
        fprintf(stderr, "List[%d]: %d %d\n", i, index->lists[i]->length, index->lists[i]->capacity);
    }
}

/* Parse ACPI entries from an Index

   @param index - the index to read from and parse into
*/
void lhq_types_parse_acpi(LHQ_TYPES_INDEX *index) {
    LKDDB_ACPI_ENTRY entry;
    LHQ_LIST *list = index->lists[LHQ_TYPE_ACPI];
    /* go to start of this section */
    index->cursor = strstr(index->cursor, "acpi");
    /* read entries */
    while(lhq_acpi_entry_parse(&entry, &(index->cursor))) {
        lhq_list_append(list, (void *)&entry);
    }
    /* append last entry */
    lhq_list_append(list, (void *)&entry);
    /* shrink list to save RAM */
    lhq_list_compact(list);

#ifdef LHQ_DEBUG
#if LHQ_DEBUG > 0
    fprintf(stderr, "List: acpi, Length: %d, Capacity: %d\n", list->length, list->capacity);
    lhq_acpi_list_print(list, stderr);
#endif
#endif
}

/* Parse PCI entries from an Index

   @param index - the index to read from and parse into
*/
void lhq_types_parse_pci(LHQ_TYPES_INDEX *index) {
    LKDDB_PCI_ENTRY entry;
    LHQ_LIST *list = index->lists[LHQ_TYPE_PCI];
    index->cursor  = strstr(index->cursor, "\npci");
    while(lhq_pci_entry_parse(&entry, &(index->cursor))) {
        lhq_list_append(list, (void *)&entry);
    }
    lhq_list_append(list, (void *)&entry);
    lhq_list_compact(list);
#ifdef LHQ_DEBUG
#if LHQ_DEBUG > 0
    fprintf(stderr, "Length: %d, Capacity: %d\n", list->length, list->capacity);
    lhq_pci_list_print(list, stderr);
#endif
#endif
}

/* Parse USB entries from an Index

   @param index - the index to read from and parse into
*/
void lhq_types_parse_usb(LHQ_TYPES_INDEX *index) {
    LKDDB_USB_ENTRY entry;
    LHQ_LIST *list = index->lists[LHQ_TYPE_USB];
    index->cursor  = strstr(index->cursor, "\nusb");
    while(lhq_usb_entry_parse(&entry, &(index->cursor))) {
        lhq_list_append(list, (void *)&entry);
    }
    lhq_list_append(list, (void *)&entry);
    lhq_list_compact(list);
#ifdef LHQ_DEBUG
#if LHQ_DEBUG > 0
    fprintf(stderr, "Length: %d, Capacity: %d\n", list->length, list->capacity);
    lhq_usb_list_print(list, stderr);
#endif
#endif
}

/* Parse all supported types into the index

   @param index - the index to fill
*/
void lhq_types_index_populate(LHQ_TYPES_INDEX *index) {
    lhq_types_parse_acpi(index);
    lhq_types_parse_pci(index);
    lhq_types_parse_usb(index);
}

/* Search for a matching entry, copy pointers from it if found

   @param index - the index to search in
   @param entry - the entry to copy to
   @param start - index to start from
   @returns index of the match or the length of the list if not found
*/
unsigned int lhq_acpi_search_and_copy(LHQ_TYPES_INDEX *index, LKDDB_ACPI_ENTRY *entry, unsigned int start) {
    LHQ_LIST *list            = index->lists[LHQ_TYPE_ACPI];
    LKDDB_ACPI_ENTRY *entries = (LKDDB_ACPI_ENTRY *)list->data;
    unsigned int i            = start;
    for(; i < list->length; i++) {
        if(lhq_acpi_compare_and_copy(entry, &entries[i]) == 0) {
            break;
        }
    }
    return i;
}

/* Search for a matching entry, copy pointers from it if found

   @param index - the index to search in
   @param entry - the entry to copy to
   @param start - index to start from
   @returns index of the match or the length of the list if not found
*/
unsigned int lhq_pci_search_and_copy(LHQ_TYPES_INDEX *index, LKDDB_PCI_ENTRY *entry, unsigned int start) {
    LHQ_LIST *list           = index->lists[LHQ_TYPE_PCI];
    LKDDB_PCI_ENTRY *entries = (LKDDB_PCI_ENTRY *)list->data;
    unsigned int i           = start;
    for(; i < list->length; i++) {
        if(lhq_pci_compare_and_copy(entry, &entries[i]) == 0) {
            break;
        }
    }
    return i;
}

/* Search for a matching entry, copy pointers from it if found

   @param index - the index to search in
   @param entry - the entry to copy to
   @param start - index to start from
   @returns index of the match or the length of the list if not found
*/
unsigned int lhq_usb_search_and_copy(LHQ_TYPES_INDEX *index, LKDDB_USB_ENTRY *entry, unsigned int start) {
    LHQ_LIST *list           = index->lists[LHQ_TYPE_USB];
    LKDDB_USB_ENTRY *entries = (LKDDB_USB_ENTRY *)list->data;
    unsigned int i           = start;
    for(; i < list->length; i++) {
        if(lhq_usb_compare_and_copy(entry, &entries[i]) == 0) {
            break;
        }
    }
    return i;
}

/* Destroy an index

   @param index - the index to destroy
*/
void lhq_types_index_free(LHQ_TYPES_INDEX *index) {
    free(index->raw);
    for(unsigned int i = 0; i < LHQ_TYPE_COUNT; i++) {
        lhq_list_free(index->lists[i]);
    }
    free(index);
}

#endif
