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

#include <string.h>
#include "lhq_string.h"
#include "lhq_list.h"

/* PCI Class ID Format string for fscanf */
const char * LKDDB_PCI_CLASS_ID_FORMAT = "pci_class_ids %s %s %s %[^\n]\n";

/* Representation of a LKDDB PCI Class ID */
typedef struct {
    char  classMask[7];
    LHQ_STRING name;
} LKDDB_PCI_CLASS_ID;

/* Create a new LKDDB_PCI_CLASS_ID

   @param contents - pointer to the actual string contents
   @param length   - length of the string (excluding \0 terminator)

   @returns pointer to ther new LHQ_STRING
*/
LKDDB_PCI_CLASS_ID* lhq_pci_class_id_new() {
    LKDDB_PCI_CLASS_ID *result = (LKDDB_PCI_CLASS_ID*)calloc(1,sizeof(LKDDB_PCI_CLASS_ID));
    return result;
}

int lhq_pci_class_id_entry_parse(LKDDB_PCI_CLASS_ID *entry, FILE * file) {
    return fscanf(file, LKDDB_PCI_CLASS_ID_FORMAT,
                  &entry->classMask[0], &entry->classMask[2], &entry->classMask[4],
                  entry->name
    ) == 4;
}

void lhq_pci_class_id_entry_print(LKDDB_PCI_CLASS_ID *entry, FILE *out) {
    fprintf(out, "PCI Class ID:\n");
    fprintf(out, "\tMask: %s\n", entry->classMask);
    fprintf(out, "\tName: %s\n", entry->name);
}

LKDDB_LIST_DECLARE(pci_class_id,LKDDB_PCI_CLASS_ID)

#endif
