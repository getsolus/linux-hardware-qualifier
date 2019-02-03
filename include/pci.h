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

#ifndef __LINUX_HARDWARE_QUALIFIER_PCI_H__
#define __LINUX_HARDWARE_QUALIFIER_PCI_H__

#include "lhq_list.h"

#include "pci_class_ids.h"
#include "pci_ids.h"

#include <string.h>

/* Representation of a LKDDB PCI Entry

   @field id         - the id of this PCI device
   @field class      - the class of this PCI device
   @field configOpts - the configuration options needed to enable this device
   @field filename   - the source file where this device is declared
*/
typedef struct {
    LKDDB_PCI_ID id;
    LKDDB_PCI_CLASS_ID class;

    char *configOpts;
    char *filename;
} LKDDB_PCI_ENTRY;

/* Parse the next available PCI Entry

   @param entry - the entry to parse into
   @param file  - the file to parse from
   @returns 0 if no more to read, 1 if more to read
*/
int lhq_pci_entry_parse(LKDDB_PCI_ENTRY *entry, char **file) {
    entry->id.name    = NULL;
    entry->class.name = NULL;
    /* id */
    *file               = strchr(*file, ' ') + 1;
    entry->id.vendor    = *file;
    *file               = strchr(*file, ' ') + 1;
    (*file)[-1]         = '\0';
    entry->id.device    = *file;
    *file               = strchr(*file, ' ') + 1;
    (*file)[-1]         = '\0';
    entry->id.subVendor = *file;
    *file               = strchr(*file, ' ') + 1;
    (*file)[-1]         = '\0';
    entry->id.subDevice = *file;
    /* class */
    *file                  = strchr(*file, ' ') + 1;
    (*file)[-1]            = '\0';
    entry->class.classMask = *file;
    /* configOpts */
    *file             = strchr(*file, ':') + 2;
    (*file)[-3]       = '\0';
    entry->configOpts = *file;
    /* filename */
    *file           = strchr(*file, ':') + 2;
    (*file)[-3]     = '\0';
    entry->filename = *file;
    *file           = strstr(*file, "\n");
    /* check for more */
    if(*file != NULL) {
        (*file)++;
        (*file)[-1] = '\0';
        if(strncmp(*file, "pci", 3) != 0) {
            return 0;
        }
    }
    return 1;
}

/* Check if entry is the same as other, copy pointers from other if so

   @param entry - the entry to copy to
   @param other - the entry to compare against and copy from
   @returns 0 if equal otherwise < 0 or > 0
*/
int lhq_pci_compare_and_copy(LKDDB_PCI_ENTRY *entry, LKDDB_PCI_ENTRY *other) {
    int compare = lhq_pci_id_compare_and_copy(&(entry->id), &(other->id));
    if(compare == 0) {
        entry->configOpts = other->configOpts;
        entry->filename   = other->filename;
    }
    return compare;
}

/* define the lhq_pci_list functions */
LHQ_LIST_DECLARE(pci, LKDDB_PCI_ENTRY)

#endif
