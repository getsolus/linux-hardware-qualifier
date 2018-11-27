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

#ifndef __LINUX_HARDWARE_QUALIFIER_PCI_IDS_H__
#define __LINUX_HARDWARE_QUALIFIER_PCI_IDS_H__

#include <stdio.h>
#include <string.h>
#include "index.h"
#include "lhq_types.h"

/* Representation of a LKDDB PCI ID

   @field vendor    - the 4 chracter hexadecimal ID of the device Vendor
   @field device    - the 4 chracter hexadecimal ID of the device
   @field subVendor - the 4 chracter hexadecimal ID of the subdevice Vendor
   @field subDevice - the 4 chracter hexadecimal ID of the subdevice
   @field name      - the name of the device
*/
typedef struct {
    char  *vendor;
    char  *device;

    char  *subVendor;
    char  *subDevice;

    char  *name;
} LKDDB_PCI_ID;

/* Create a new LKDDB_PCI_ID

   @param contents - pointer to the actual string contents
   @param length   - length of the string (excluding \0 terminator)

   @returns pointer to ther new LKDDB_PCI_ID
*/
LKDDB_PCI_ID* lhq_pci_id_new() {
    LKDDB_PCI_ID *result = (LKDDB_PCI_ID*)calloc(1,sizeof(LKDDB_PCI_ID));
    return result;
}

/* Parse the next PCI ID from a file

   @param entry - the entry to parse into
   @param file  - the file to parse
   @returns 0 if no more to parse, 1 if more to parse
*/
int lhq_pci_id_entry_parse(LKDDB_PCI_ID *entry, char ** file) {
    /* vendor */
    *file = strchr(*file, ' ') + 1;
    entry->vendor           = *file;
    /* device */
    *file = strchr(*file, ' ') + 1;
    (*file)[-1] = '\0';
    entry->device          = *file;
    /* subvendor */
    *file = strchr(*file, ' ') + 1;
    (*file)[-1] = '\0';
    entry->subVendor       = *file;
    /* subdevice */
    *file = strchr(*file, ' ') + 1;
    (*file)[-1] = '\0';
    entry->subDevice    = *file;
    /* name */
    *file = strchr(*file, ' ') + 1;
    (*file)[-1] = '\0';
    entry->name         = *file;
    *file = strchr(*file, '\n');
    /* check for more */
    if( *file != NULL ){
        (*file)++;
        (*file)[-1] = '\0';
        if( strncmp(*file, "pci_ids", 7) != 0 ){
            return 0;
        }
    }
    return 1;
}

/* Print a summary of this PCI ID

   @param entry - the entry to print
   @param out   - the file to write to
*/
void lhq_pci_id_entry_print(LKDDB_PCI_ID *entry, FILE *out) {
    fprintf(out, "PCI ID:\n");
    fprintf(out, "\tVendor: %s:%s\n", entry->vendor, entry->subVendor);
    fprintf(out, "\tDevice: %s:%s\n", entry->device, entry->subDevice);
    fprintf(out, "\tName: %s\n", entry->name);
}

/* declare list type */
LKDDB_LIST_DECLARE(pci_id,LKDDB_PCI_ID)

/* Parse all of the PCI IDs in the provided Index

   @param index - the index to read from
*/
void lhq_pci_ids(LHQ_INDEX *index) {
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


#endif
