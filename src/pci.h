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
#include "lhq_string.h"
#include "lhq_list.h"

/* PCI Device Format string for fscanf */
const char * LKDDB_PCI_FORMAT = "pci %s %s %s %s %s : %[^:\n] : %s\n";

/* Representation of a LKDDB PCI Entry */
typedef struct {
    char  *vendor;
    char  *device;

    char  *subVendor;
    char  *subDevice;

    char  *classMask;

    char *configOpts;
    char *filename;
} LKDDB_PCI_ENTRY;

/* Create a new LKDDB_PCI_ENTRY

   @param contents - pointer to the actual string contents
   @param length   - length of the string (excluding \0 terminator)

   @retruns pointer to ther new LHQ_STRING
*/
LKDDB_PCI_ENTRY* lhq_pci_entry_new() {
    LKDDB_PCI_ENTRY *result = (LKDDB_PCI_ENTRY*)calloc(1,sizeof(LKDDB_PCI_ENTRY));
    return result;
}

char * lhq_pci_entry_parse(LKDDB_PCI_ENTRY *entry, char * file) {
    file = strchr(file, ' ') + 1;
    entry->vendor           = file;
    file = strchr(file, ' ') + 1;
    file[-1] = '\0';
    entry->device          = file;
    file = strchr(file, ' ') + 1;
    file[-1] = '\0';
    entry->subVendor       = file;
    file = strchr(file, ' ') + 1;
    file[-1] = '\0';
    entry->subDevice    = file;
    file = strchr(file, ' ') + 1;
    file[-1] = '\0';
    entry->classMask    = file;
    file = strchr(file, ':') + 2;
    file[-3] = '\0';
    entry->configOpts         = file;
    file = strchr(file, ':') + 2;
    file[-3] = '\0';
    entry->filename           = file;
    file = strstr(file, "\n");
    if( file != NULL ){
        file++;
        file[-1] = '\0';
        if( strncmp(file, "pci", 3) != 0 ){
            return NULL;
        }
    }
    return file;
}
void lhq_pci_entry_print(LKDDB_PCI_ENTRY *entry, FILE *out) {
    fprintf(out, "PCI Entry:\n");
    fprintf(out, "\tVendor: %s:%s\n", entry->vendor, entry->subVendor);
    fprintf(out, "\tDevice: %s:%s\n", entry->device, entry->subDevice);
    fprintf(out, "\tClassMask: %s\n", entry->classMask);
    fprintf(out, "\tConfig Options: %s\n", entry->configOpts);
    fprintf(out, "\tSource: %s\n", entry->filename);
}

void lhq_pci_entry_free(LKDDB_PCI_ENTRY *entry) {
    free(entry);
}

LKDDB_LIST_DECLARE(pci,LKDDB_PCI_ENTRY)

void lhq_pci(const char * lkddb) {
    LKDDB_PCI_ENTRY entry;
    LKDDB_LIST *list = lhq_pci_list_new();
    char * ptr = lkddb;
    ptr = strstr(ptr, "\npci");
    *ptr = '\0';
    ptr++;
    while(ptr != NULL){
        ptr = lhq_pci_entry_parse(&entry, ptr);
        lhq_pci_list_append(list, &entry);
    }
    lhq_list_compact(list);
    fprintf(stderr, "Length: %d, Capacity: %d\n", list->length, list->capacity);
    //lhq_pci_list_print(list,stderr);
    lhq_pci_list_free(list);
}

#endif
