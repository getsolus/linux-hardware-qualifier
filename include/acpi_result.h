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

#ifndef __LINUX_HARDWARE_QUALIFIER_ACPI_RESULT_H__
#define __LINUX_HARDWARE_QUALIFIER_ACPI_RESULT_H__

#include "types_index.h"

#include <stdio.h>

/* Representation of a LHQ ACPI Search Result

   @field entry - the original ACPI Entry
*/
typedef struct {
    LKDDB_ACPI_ENTRY entry;
} LHQ_ACPI_RESULT;

/* Search for a ACPI Device in the Index

   @param result - the result to fill
   @param ids    - the IDs Index
   @param types  - the Types Index
*/
void lhq_acpi_result_search(LHQ_ACPI_RESULT *result, LHQ_TYPES_INDEX *types) {
    lhq_acpi_search_and_copy(types, &result->entry, 0);
}

const char *lhq_acpi_result_format = "\
\t{\n\
\t\t\"id\" : \"%s\",\n\
\t\t\"options\" : \"%s\",\n\
\t\t\"source\" : \"%s\",\n\
\t},\n\
";

/* Print a summary of this ACPI Result

   @param result - the result to print
   @param out    - the file to write to
*/
void lhq_acpi_result_entry_print(LHQ_ACPI_RESULT *result, FILE *out) {
    fprintf(out, lhq_acpi_result_format, result->entry.id, result->entry.configOpts, result->entry.filename);
}

/* Free memory allocated to this result

   @param result - the result to free
*/
void lhq_acpi_result_free(LHQ_ACPI_RESULT *result) { free(result->entry.id); }

/* define the lhq_apci_result_list functions */
LHQ_LIST_DECLARE(acpi_result, LHQ_ACPI_RESULT)

#endif
