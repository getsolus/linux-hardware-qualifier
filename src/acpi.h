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

#include <string.h>
#include "lhq_string.h"

/* ACPI Device Format string for fscanf */
const char * LKDDB_ACPI_FORMAT = "acpi %[^:\n] : %[^:\n] : %s\n";

/* Representation of a LKDDB ACPI Entry */
typedef struct {
    LHQ_STRING id;

    LHQ_STRING configOpts;
    LHQ_STRING filename;
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

int lhq_acpi_entry_parse(LKDDB_ACPI_ENTRY *entry, FILE * file) {
    return fscanf(file, LKDDB_ACPI_FORMAT, entry->id, entry->configOpts, entry->filename ) == 3;
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

#define LKDDB_ACPI_LIST_START 1000
#define LKDDB_ACPI_LIST_INCREMENT 1000

typedef struct {
    LKDDB_ACPI_ENTRY *data;
    unsigned int length;
    unsigned int capacity;
} LKDDB_ACPI_LIST;

LKDDB_ACPI_LIST* lhq_acpi_list_new() {
    LKDDB_ACPI_LIST *result = (LKDDB_ACPI_LIST*)calloc(1,sizeof(LKDDB_ACPI_LIST));
    result->data     = (LKDDB_ACPI_ENTRY*)calloc(LKDDB_ACPI_LIST_START,sizeof(LKDDB_ACPI_ENTRY));
    result->length   = 0;
    result->capacity = LKDDB_ACPI_LIST_START;
    return result;
}

void lhq_acpi_list_append(LKDDB_ACPI_LIST* list, LKDDB_ACPI_ENTRY *entry) {
    if(list->length == list->capacity) {
        list->capacity += LKDDB_ACPI_LIST_INCREMENT;
        list->data = realloc(list->data, list->capacity*sizeof(LKDDB_ACPI_ENTRY) );
    }
    list->data[list->length] = *entry;
    list->length++;
}

void lhq_acpi_list_print(LKDDB_ACPI_LIST *list, FILE *out) {
    for(unsigned int i = 0; i < list->length; i++ ){
        lhq_acpi_entry_print(&list->data[i], out);
    }
}

void lhq_acpi_list_free(LKDDB_ACPI_LIST* list) {
    free(list->data);
    free(list);
}

#endif
