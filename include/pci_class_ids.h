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

#ifndef __LINUX_HARDWARE_QUALIFIER_PCI_CLASS_IDS_H__
#define __LINUX_HARDWARE_QUALIFIER_PCI_CLASS_IDS_H__

#include <stdio.h>
#include <string.h>
#include "lhq_types.h"

/* Representation of a LKDDB PCI Class ID

   @field classMask - the six character hexadecimal mask for a PCI Class
   @field name      - the name of the PCI Class
*/
typedef struct {
    char *classMask;
    char *name;
} LKDDB_PCI_CLASS_ID;

/* Create a new LKDDB_PCI_CLASS_ID

   @returns pointer to ther new LKDDB_PCI_CLASS_ID
*/
LKDDB_PCI_CLASS_ID* lhq_pci_class_id_new() {
    LKDDB_PCI_CLASS_ID *result = (LKDDB_PCI_CLASS_ID*)calloc(1,sizeof(LKDDB_PCI_CLASS_ID));
    return result;
}

/* Check if entry is the same as other, copy pointers from other if so

   @param entry - the entry to copy to
   @param other - the entry to compare against and copy from
   @returns 0 if equal otherwise < 0 or > 0
*/
int lhq_pci_class_id_compare_and_copy(LKDDB_PCI_CLASS_ID *entry, LKDDB_PCI_CLASS_ID *other) {
    /* don't try to compare if entry has already been filled */
    if( entry->name != NULL ) {
        return 0;
    }
    int compare = strcmp(entry->classMask, other->classMask);
    if( compare == 0 ){
        entry->name = other->name;
    }
    return compare;
}


/* Parse a PCI Class ID from a file

   @param entry - the entry to write into
   @param file  - the file to read from

   @returns 0 if done, 1 if more to parse
*/
int lhq_pci_class_id_entry_parse(LKDDB_PCI_CLASS_ID *entry, char ** file) {
    /* classmask */
    *file = strchr(*file, ' ') + 1;
    entry->classMask = *file;
    entry->classMask[2] = entry->classMask[3];
    entry->classMask[3] = entry->classMask[4];
    entry->classMask[4] = entry->classMask[6];
    entry->classMask[5] = entry->classMask[7];
    entry->classMask[7] = '\0';
    *file += 8;
    /* name */
    *file = strchr(*file, ' ') + 1;
    entry->name         = *file;
    *file = strstr(*file, "\n");
    /* check for more */
    if( *file != NULL ){
        (*file)++;
        (*file)[-1] = '\0';
        if( strncmp(*file, "pci_class_ids", 13) != 0 ){
            return 0;
        }
    }
    return 1;
}

/* Print out a summary of a PCI Class ID

    @param entry - the entry to print
    @param out   - the file to print to
*/
void lhq_pci_class_id_entry_print(LKDDB_PCI_CLASS_ID *entry, FILE *out) {
    fprintf(out, "PCI Class ID:\n");
    fprintf(out, "\tMask: %s\n", entry->classMask);
    fprintf(out, "\tName: %s\n", entry->name);
}

/* define the lhq_class_id_list functions */
LHQ_LIST_DECLARE(pci_class_id,LKDDB_PCI_CLASS_ID)

#endif
