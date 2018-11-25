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
#include "lhq_list.h"
#include "lhq_string.h"

/* ACPI Device Format string for fscanf */
const char * LKDDB_ACPI_FORMAT = "acpi %s : %[^:\n] : %s\n";

/* Representation of a LKDDB ACPI Entry */
typedef struct {
    char *id;

    char *configOpts;
    char *filename;
} LKDDB_ACPI_ENTRY;

/* Create a new LKDDB_ACPI_ENTRY

   @param contents - pointer to the actual string contents
   @param length   - length of the string (excluding \0 terminator)

   @retruens pointer to ther new LHQ_STRING
*/
LKDDB_ACPI_ENTRY* lhq_acpi_entry_new() {
    LKDDB_ACPI_ENTRY *result = (LKDDB_ACPI_ENTRY*)calloc(1,sizeof(LKDDB_ACPI_ENTRY));
    return result;
}

int lhq_acpi_entry_parse(LKDDB_ACPI_ENTRY *entry, char ** file) {
    *file = strchr(*file, '"') + 1;
    entry->id = *file;
    *file = strchr(*file, '"') + 1;
    (*file)[-1] = '\0';
    *file = strchr(*file, ':') + 2;
    entry->configOpts         = *file;
    *file = strchr(*file, ':') + 2;
    (*file)[-3] = '\0';
    entry->filename           = *file;
    *file = strstr(*file, "\n");
    if( (*file) != NULL ){
        (*file)++;
        (*file)[-1] = '\0';
        if( strncmp(*file, "acpi", 4) != 0 ){
            return 0;
        }
    }
    return 1;
}

void lhq_acpi_entry_print(LKDDB_ACPI_ENTRY *entry, FILE *out) {
    fprintf(out, "ACPI Entry:\n");
    fprintf(out, "\tID: %s\n", entry->id);
    fprintf(out, "\tConfig Options: %s\n", entry->configOpts);
    fprintf(out, "\tSource: %s\n", entry->filename);
}

/* Destroy an LKDEDB_ACPI_ENTRY */
void lhq_acpi_entry_free(LKDDB_ACPI_ENTRY *entry) {
    free(entry);
}

LKDDB_LIST_DECLARE(acpi,LKDDB_ACPI_ENTRY)

void lhq_acpi(char ** lkddb) {
    LKDDB_ACPI_ENTRY entry;
    LKDDB_LIST *list = lhq_acpi_list_new();
    *lkddb = strstr(*lkddb, "acpi");
    while(lhq_acpi_entry_parse(&entry, lkddb) ) {
        lhq_acpi_list_append(list, &entry);
    }
    lhq_list_compact(list);
    fprintf(stderr, "Length: %d, Capacity: %d\n", list->length, list->capacity);
    //lhq_acpi_list_print(list,stderr);
    lhq_acpi_list_free(list);
}

#endif
