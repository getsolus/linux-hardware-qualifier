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
#include "lhq_string.h"
#include "lhq_list.h"

/* PCI ID Format string for fscanf */
const char * LKDDB_PCI_ID_FORMAT = "pci_ids %s %s %s %s %[^\n]\n";

/* Representation of a LKDDB PCI ID */
typedef struct {
    char  vendor[5];
    char  device[5];

    char  subVendor[5];
    char  subDevice[5];

    LHQ_STRING name;
} LKDDB_PCI_ID;

/* Create a new LKDDB_PCI_ID

   @param contents - pointer to the actual string contents
   @param length   - length of the string (excluding \0 terminator)

   @returns pointer to ther new LHQ_STRING
*/
LKDDB_PCI_ID* lhq_pci_id_new() {
    LKDDB_PCI_ID *result = (LKDDB_PCI_ID*)calloc(1,sizeof(LKDDB_PCI_ID));
    return result;
}

int lhq_pci_id_entry_parse(LKDDB_PCI_ID *entry, FILE * file) {
    return fscanf(file, LKDDB_PCI_ID_FORMAT,
                  entry->vendor,     entry->device,
                  entry->subVendor,  entry->subDevice,
                  entry->name
    ) == 5;
}

void lhq_pci_id_entry_print(LKDDB_PCI_ID *entry, FILE *out) {
    fprintf(out, "PCI ID:\n");
    fprintf(out, "\tVendor: %s:%s\n", entry->vendor, entry->subVendor);
    fprintf(out, "\tDevice: %s:%s\n", entry->device, entry->subDevice);
    fprintf(out, "\tName: %s\n", entry->name);
}

LKDDB_LIST_DECLARE(pci_id,LKDDB_PCI_ID)

void lhq_pci_ids(FILE * lkddb_ids) {
    rewind(lkddb_ids);
    LKDDB_PCI_ID entry;
    LKDDB_LIST *list = lhq_pci_id_list_new();
    while(!feof(lkddb_ids) ){
        if( lhq_pci_id_entry_parse(&entry, lkddb_ids) ){
            lhq_pci_id_list_append(list, &entry);
        } else {
            while(!feof(lkddb_ids) && getc(lkddb_ids) != '\n');
        }
    }
    lhq_list_compact(list);
    fprintf(stderr, "Length: %d, Capacity: %d\n", list->length, list->capacity);
    //lhq_pci_id_list_print(list,stderr);
    lhq_pci_id_list_free(list);
}

#endif
