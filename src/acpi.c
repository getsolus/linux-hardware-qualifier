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

#include <stdio.h>
#include <string.h>

#include "lhq_list.h"
#include "acpi.h"

/* Check if entry is the same as other, copy pointers from other if so

   @param entry - the entry to copy to
   @param other - the entry to compare against and copy from
   @returns 0 if equal otherwise < 0 or > 0
*/
int lhq_acpi_compare_and_copy(LKDDB_ACPI_ENTRY *entry, LKDDB_ACPI_ENTRY *other) {
    int compare = strcmp(entry->id, other->id);
    if(compare == 0) {
        entry->configOpts = other->configOpts;
        entry->filename   = other->filename;
    }
    return compare;
}

/* Try to read an ACPI entry from the lkddb file

   @param entry - the entry to write to
   @param file  - the binary data to read from

   @returns 1 if there are more entries to read, 0 otherwise
*/
int lhq_acpi_entry_parse(LKDDB_ACPI_ENTRY *entry, char **file) {
    /* Format: acpi "id" : configs : filename\n */
    /* id */
    *file       = strchr(*file, '"') + 1;
    entry->id   = *file;
    *file       = strchr(*file, '"') + 1;
    (*file)[-1] = '\0';
    /* config opts */
    *file             = strchr(*file, ':') + 2;
    entry->configOpts = *file;
    /* filename */
    *file           = strchr(*file, ':') + 2;
    (*file)[-3]     = '\0';
    entry->filename = *file;
    *file           = strstr(*file, "\n");
    /* check for next entry */
    if((*file) != NULL) {
        (*file)++;
        (*file)[-1] = '\0';
        if(strncmp(*file, "acpi", 4) != 0) {
            return 0;
        }
    }
    return 1;
}

/* define the lhq_acpi_list functions */
LHQ_LIST_DEFINE(acpi, LKDDB_ACPI_ENTRY)
