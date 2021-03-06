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

#include "pci_class_ids.h"

#include "lhq_list.h"

#include <string.h>
#include <stdlib.h>

/* Convert a full PCI Class ID to one suitable for finding just the PCI Class

   @param entry - the full PCI Class ID
   @param class - the extracted PCI Class
*/
void lhq_pci_class_id_class(LKDDB_PCI_CLASS_ID *entry, LKDDB_PCI_CLASS_ID *class) {
    class->classMask    = (char *)calloc(7, sizeof(char));
    class->classMask[0] = entry->classMask[0];
    class->classMask[1] = entry->classMask[1];
    class->classMask[2] = '.';
    class->classMask[3] = '.';
    class->classMask[4] = '.';
    class->classMask[5] = '.';
    class->classMask[6] = '\0';
}

/* Convert a full PCI Class ID to one suitable for finding just the PCI Subclass

   @param entry    - the full PCI Class ID
   @param subclass - the extracted PCI Subclass
*/
void lhq_pci_class_id_subclass(LKDDB_PCI_CLASS_ID *entry, LKDDB_PCI_CLASS_ID *subclass) {
    subclass->classMask    = (char *)calloc(7, sizeof(char));
    subclass->classMask[0] = entry->classMask[0];
    subclass->classMask[1] = entry->classMask[1];
    subclass->classMask[2] = entry->classMask[2];
    subclass->classMask[3] = entry->classMask[3];
    subclass->classMask[4] = '.';
    subclass->classMask[5] = '.';
    subclass->classMask[6] = '\0';
}

/* Check if entry is the same as other, copy pointers from other if so

   @param entry - the entry to copy to
   @param other - the entry to compare against and copy from
   @returns 0 if equal otherwise < 0 or > 0
*/
int lhq_pci_class_id_compare_and_copy(LKDDB_PCI_CLASS_ID *entry, LKDDB_PCI_CLASS_ID *other) {
    int compare = strncmp(entry->classMask, other->classMask, 6);
    if((entry->name == NULL) && (compare == 0)) {
        entry->name = other->name;
    }
    return compare;
}

/* Parse a PCI Class ID from a file

   @param entry - the entry to write into
   @param file  - the file to read from

   @returns 0 if done, 1 if more to parse
*/
int lhq_pci_class_id_entry_parse(LKDDB_PCI_CLASS_ID *entry, char **file) {
    /* classmask */
    *file               = strchr(*file, ' ') + 1;
    entry->classMask    = *file;
    entry->classMask[2] = entry->classMask[3];
    entry->classMask[3] = entry->classMask[4];
    entry->classMask[4] = entry->classMask[6];
    entry->classMask[5] = entry->classMask[7];
    entry->classMask[6] = '\0';
    *file += 7;
    /* name */
    *file       = strchr(*file, ' ') + 1;
    entry->name = *file;
    *file       = strchr(*file, '\n');
    /* check for more */
    if(*file != NULL) {
        (*file)++;
        (*file)[-1] = '\0';
        if(strncmp(*file, "pci_class_ids", 13) != 0) {
            return 0;
        }
    }
    return 1;
}

/* define the lhq_class_id_list functions */
LHQ_LIST_DEFINE(pci_class_id, LKDDB_PCI_CLASS_ID)
