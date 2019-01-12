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

#ifndef __LINUX_HARDWARE_QUALIFIER_KERNEL_FLAG_H__
#define __LINUX_HARDWARE_QUALIFIER_KERNEL_FLAG_H__

#include "lhq_list.h"

#include <stdio.h>
#include <string.h>

/* Representation of a LHQ Kernel Flags

   @field name  - the name of the flag
   @field value - the value of the flag
*/

typedef struct {
    char *name;
    char *value;
} LHQ_KERNEL_FLAG;

/* Create a new LHQ_KERNEL_FLAG

   @returns pointer to the new LHQ_KERNEL_FLAG
*/
LHQ_KERNEL_FLAG *lhq_kernel_flag_entry_new() {
    LHQ_KERNEL_FLAG *result = (LHQ_KERNEL_FLAG *)calloc(1, sizeof(LHQ_KERNEL_FLAG));
    return result;
}

/* Check if entry is the same as other, copy pointers from other if so

   @param entry - the entry to copy to
   @param other - the entry to compare against and copy from
   @returns 0 if equal otherwise < 0 or > 0
*/
int lhq_kernel_flag_compare_and_copy(LHQ_KERNEL_FLAG *entry, LHQ_KERNEL_FLAG *other) {
    int compare = strcmp(entry->name, other->name);
    if(compare == 0) {
        entry->value = other->value;
    }
    return compare;
}

/* Try to read an Kernel Flag

   @param entry - the entry to write to
   @param file  - the binary data to read from

   @returns 1 if there are more entries to read, 0 otherwise
*/
int lhq_kernel_flag_parse(LHQ_KERNEL_FLAG *entry, char **file) {
    /* skip comments */
    while ((**file == '#') || (**file == '\n')) {
        (*file)++;
        *file = strchr(*file, '\n') + 1;
    }
    if (**file == '\0') {
        return -1;
    }
    /* Format: flag=value\n */
    /* name */
    entry->name = *file;
    *file       = strchr(*file, '=') + 1;
    (*file)[-1] = '\0';
    /* value */
    entry->value = *file;
    *file        = strchr(*file, '\n') + 1;
    /* check for next entry */
    if((**file) != '\0') {
        (*file)[-1] = '\0';
        return 1;
    }
    return 0;
}
/* Print a summary of the entry to a file

   @param entry - the entry to print
   @param out   - the file to write to
*/
void lhq_kernel_flag_entry_print(LHQ_KERNEL_FLAG *entry, FILE *out) {
    fprintf(out, "Kernel Flag:\n");
    fprintf(out, "\tName: %s\n", entry->name);
    fprintf(out, "\tValue: %s\n", entry->value);
}

/* Destroy an LHQ_KERNEL_FLAG

   @param entry - the entry to destroy
*/
void lhq_kernel_flag_entry_free(LHQ_KERNEL_FLAG *entry) { free(entry); }

/* define the lhq_kernel_flag_list functions */
LHQ_LIST_DECLARE(kernel_flag, LHQ_KERNEL_FLAG)

#endif
