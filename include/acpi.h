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

#ifndef __LINUX_HARDWARE_QUALIFIER_ACPI_H__
#define __LINUX_HARDWARE_QUALIFIER_ACPI_H__

#include "lhq_list.h"

#include <stdio.h>

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

/* define the lhq_apci_list functions */
LHQ_LIST_DECLARE(acpi, LHQ_ACPI_ENTRY)

/* Check if entry is the same as other, copy pointers from other if so

   @param entry - the entry to copy to
   @param other - the entry to compare against and copy from
   @returns 0 if equal otherwise < 0 or > 0
*/
int lhq_acpi_compare_and_copy(LKDDB_ACPI_ENTRY *entry, LKDDB_ACPI_ENTRY *other);

/* Try to read an ACPI entry from the lkddb file

   @param entry - the entry to write to
   @param file  - the binary data to read from

   @returns 1 if there are more entries to read, 0 otherwise
*/
int lhq_acpi_entry_parse(LKDDB_ACPI_ENTRY *entry, char **file);


/* Representation of a LHQ ACPI Search Result

   @field entry - the original ACPI Entry
*/
typedef struct {
    LKDDB_ACPI_ENTRY entry;
} LHQ_ACPI_RESULT;

/* define the lhq_apci_result_list functions */
LHQ_LIST_DECLARE(acpi_result, LHQ_ACPI_RESULT)

/* Print a summary of this ACPI Result

   @param result - the result to print
   @param out    - the file to write to
*/
void lhq_acpi_result_entry_print(LHQ_ACPI_RESULT *result, FILE *out);

/* Free memory allocated to this result

   @param result - the result to free
*/
void lhq_acpi_result_free(LHQ_ACPI_RESULT *result);

/* Search for all of the system ACPI devices and look them un in the LKDDB

   @param results - the list of results to save to
*/
int lhq_acpi_find_devices(LHQ_LIST *results);

#endif
