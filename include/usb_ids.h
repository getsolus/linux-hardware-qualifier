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

#ifndef __LINUX_HARDWARE_QUALIFIER_USB_IDS_H__
#define __LINUX_HARDWARE_QUALIFIER_USB_IDS_H__

#include <stdio.h>
#include <string.h>
#include "index.h"
#include "lhq_types.h"

/* Representation of a LKDDB USB ID

   @field vendor  - the 4 character hexadecimal USB Vendor ID
   @field product - the 4 character hexadecimal USB Product ID
   @field name    - the name of the device or vendor
*/
typedef struct {
    char *vendor;
    char *product;

    char *name;
} LKDDB_USB_ID;

/* Create a new LKDDB_USB_ID

   @param contents - pointer to the actual string contents
   @param length   - length of the string (excluding \0 terminator)

   @retruens pointer to ther new LKDDB_USB_ID
*/
LKDDB_USB_ID* lhq_usb_id_new() {
    LKDDB_USB_ID *result = (LKDDB_USB_ID*)calloc(1,sizeof(LKDDB_USB_ID));
    return result;
}

/* Parse a LKDDB_USB_ID from a file

   @param entry - the entry to parse into
   @param file  - the file to parse
   @returns 0 if no more to parse, 1 if more
*/
int lhq_usb_id_entry_parse(LKDDB_USB_ID *entry, char ** file) {
    /* vendor */
    *file = strchr(*file, ' ') + 1;
    entry->vendor = *file;
    /* product */
    *file = strchr(*file, ' ') + 1;
    (*file)[-1] = '\0';
    entry->product = *file;
    /* name */
    *file = strchr(*file, ' ') + 1;
    (*file)[-1] = '\0';
    entry->name = *file;
    *file = strchr(*file, '\n');
    /* check for more */
    if( *file != NULL ){
        (*file)++;
        (*file)[-1] = '\0';
        if( strncmp(*file, "usb_ids", 7) != 0 ){
            return 0;
        }
    }
    return 1;
}

/* Print a sumary of a USB_ID to a file

   @param entry - the entry to print
   @param file  - the file to write to
*/
void lhq_usb_id_entry_print(LKDDB_USB_ID *entry, FILE *out) {
    fprintf(out, "USB ID:\n");
    fprintf(out, "\tIDs: %s:%s\n", entry->vendor, entry->product);
    fprintf(out, "\tName: %s\n", entry->name);
}

/* delclare USB ID list type */
LKDDB_LIST_DECLARE(usb_id,LKDDB_USB_ID)

/* Parse USB IDS from index

   @param index - the index to parse
*/
void lhq_usb_ids(LHQ_INDEX *index) {
    LKDDB_USB_ID entry;
    LKDDB_LIST *list = index->lists[LHQ_ID_USB];
    index->cursor = strstr(index->cursor, "\nusb_ids");
    while( lhq_usb_id_entry_parse(&entry, &(index->cursor)) ) {
        lhq_list_append(list, (void*)&entry);
    }
    lhq_list_append(list, (void*)&entry);
    lhq_list_compact(list);
#ifdef LHQ_DEBUG
#if LHQ_DEBUG > 0
    fprintf(stderr, "Length: %d, Capacity: %d\n", list->length, list->capacity);
    lhq_usb_id_list_print(list,stderr);
#endif
#endif
}

#endif
