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

#ifndef __LINUX_HARDWARE_QUALIFIER_ACPI_H__
#define __LINUX_HARDWARE_QUALIFIER_ACPI_H__

#include <stdio.h>
#include <string.h>
#include "index.h"
#include "lhq_types.h"

/* Representation of a LKDDB ACPI Entry

   @field id         - the ID of the ACPI device
   @field configOpts - a list of configuration options needed for this
                       device when building the kernel
   @field filename   - the name of the source file where this device is declared
*/
typedef struct {
    char *id;

    char *configOpts;
    char *filename;
} LKDDB_ACPI_ENTRY;

/* Create a new LKDDB_ACPI_ENTRY

   @returns pointer to ther new LKDDB_ACPI_ENTRY
*/
LKDDB_ACPI_ENTRY* lhq_acpi_entry_new() {
    LKDDB_ACPI_ENTRY *result = (LKDDB_ACPI_ENTRY*)calloc(1,sizeof(LKDDB_ACPI_ENTRY));
    return result;
}

/* Try to read an ACPI entry from the lkddb file

   @param entry - the entry to write to
   @param file  - the binary data to read from

   @returns 1 if there are more entries to read, 0 otherwise
*/
int lhq_acpi_entry_parse(LKDDB_ACPI_ENTRY *entry, char ** file) {
    /* Format: acpi "id" : configs : filename\n */
    /* id */
    *file = strchr(*file, '"') + 1;
    entry->id = *file;
    *file = strchr(*file, '"') + 1;
    (*file)[-1] = '\0';
    /* config opts */
    *file = strchr(*file, ':') + 2;
    entry->configOpts         = *file;
    /* filename */
    *file = strchr(*file, ':') + 2;
    (*file)[-3] = '\0';
    entry->filename           = *file;
    *file = strstr(*file, "\n");
    /* check for next entry */
    if( (*file) != NULL ){
        (*file)++;
        (*file)[-1] = '\0';
        if( strncmp(*file, "acpi", 4) != 0 ){
            return 0;
        }
    }
    return 1;
}
/* Print a summary of the entry to a file

   @param entry - the entry to print
   @param out   - the file to write to
*/
void lhq_acpi_entry_print(LKDDB_ACPI_ENTRY *entry, FILE *out) {
    fprintf(out, "ACPI Entry:\n");
    fprintf(out, "\tID: %s\n", entry->id);
    fprintf(out, "\tConfig Options: %s\n", entry->configOpts);
    fprintf(out, "\tSource: %s\n", entry->filename);
}

/* Destroy an LKDDB_ACPI_ENTRY

   @param entry - the entry to destroy
*/
void lhq_acpi_entry_free(LKDDB_ACPI_ENTRY *entry) {
    free(entry);
}

/* define the lhq_acpi_list functions */
LKDDB_LIST_DECLARE(acpi,LKDDB_ACPI_ENTRY)

#endif
