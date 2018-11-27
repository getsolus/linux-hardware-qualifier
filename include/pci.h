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

#ifndef __LINUX_HARDWARE_QUALIFIER_PCI_H__
#define __LINUX_HARDWARE_QUALIFIER_PCI_H__

#include <stdio.h>
#include <string.h>
#include "pci_class_ids.h"
#include "pci_ids.h"

/* Representation of a LKDDB PCI Entry

   @field id         - the id of this PCI device
   @field class      - the class of this PCI device
   @field configOpts - the configuration options needed to enable this device
   @field filename   - the source file where this device is declared
*/
typedef struct {
    LKDDB_PCI_ID          id;
    LKDDB_PCI_CLASS_ID class;

    char *configOpts;
    char *filename;
} LKDDB_PCI_ENTRY;

/* Create a new LKDDB_PCI_ENTRY

   @param contents - pointer to the actual string contents
   @param length   - length of the string (excluding \0 terminator)

   @retruns pointer to ther new LKDDB_PCI_ENTRY
*/
LKDDB_PCI_ENTRY* lhq_pci_entry_new() {
    LKDDB_PCI_ENTRY *result = (LKDDB_PCI_ENTRY*)calloc(1,sizeof(LKDDB_PCI_ENTRY));
    return result;
}

/* Parse the next available PCI Entry

   @param entry - the entry to parse into
   @param file  - the file to parse from
   @returns 0 if no more to read, 1 if more to read
*/
int lhq_pci_entry_parse(LKDDB_PCI_ENTRY *entry, char ** file) {
    /* id */
    *file = strchr(*file, ' ') + 1;
    entry->id.vendor           = *file;
    *file = strchr(*file, ' ') + 1;
    (*file)[-1] = '\0';
    entry->id.device          = *file;
    *file = strchr(*file, ' ') + 1;
    (*file)[-1] = '\0';
    entry->id.subVendor       = *file;
    *file = strchr(*file, ' ') + 1;
    (*file)[-1] = '\0';
    entry->id.subDevice    = *file;
    /* class */
    *file = strchr(*file, ' ') + 1;
    (*file)[-1] = '\0';
    entry->class.classMask    = *file;
    /* configOpts */
    *file = strchr(*file, ':') + 2;
    (*file)[-3] = '\0';
    entry->configOpts         = *file;
    /* filename */
    *file = strchr(*file, ':') + 2;
    (*file)[-3] = '\0';
    entry->filename           = *file;
    *file = strstr(*file, "\n");
    /* check for more */
    if( *file != NULL ){
        (*file)++;
        (*file)[-1] = '\0';
        if( strncmp(*file, "pci", 3) != 0 ){
            return 0;
        }
    }
    return 1;
}

/* Print a summary of a PCI Entry

   @param entry - the entry to print
   @param out   - the file to write to
*/
void lhq_pci_entry_print(LKDDB_PCI_ENTRY *entry, FILE *out) {
    fprintf(out, "PCI Entry:\n");
    lhq_pci_id_entry_print(&(entry->id),out);
    lhq_pci_class_id_entry_print(&(entry->class),out);
    fprintf(out, "\tConfig Options: %s\n", entry->configOpts);
    fprintf(out, "\tSource: %s\n", entry->filename);
}

/* Destroy a PCI Entry

   @param entry - the entry to destroy
*/
void lhq_pci_entry_free(LKDDB_PCI_ENTRY *entry) {
    free(entry);
}

/* declare pci list type */
LKDDB_LIST_DECLARE(pci,LKDDB_PCI_ENTRY)

#endif