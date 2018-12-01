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
#include "lhq_types.h"
#include "ids_index.h"

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

   @returns pointer to ther new LKDDB_USB_ID
*/
LKDDB_USB_ID* lhq_usb_id_new() {
    LKDDB_USB_ID *result = (LKDDB_USB_ID*)calloc(1,sizeof(LKDDB_USB_ID));
    return result;
}

/* Convert a full USB ID to one suitable for finding just the USB Vendor

   @param entry  - the full USB ID
   @param vendor - the extracted USB Vendor
*/
void lhq_usb_id_vendor(LKDDB_USB_ID *entry, LKDDB_USB_ID *vendor){
    vendor->vendor = entry->vendor;
    vendor->product = "....";
}

/* Check if entry is the same as other, copy pointers from other if so

   @param entry - the entry to copy to
   @param other - the entry to compare against and copy from
   @returns 0 if equal otherwise < 0 or > 0
*/
int lhq_usb_id_compare_and_copy(LKDDB_USB_ID *entry, LKDDB_USB_ID *other) {
    /* don't try to compare if entry has already been filled */
    if( entry->name != NULL ) {
        return 0;
    }
    int compare = strcmp(entry->vendor, other->vendor);
    if( compare != 0 ) return compare;
    compare = strcmp(entry->product, other->product);
    if( compare == 0 ){
        entry->name = other->name;
    }
    return compare;
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
    *file = strstr(*file, "\n");
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

/* define the lhq_usb_id_list functions */
LHQ_LIST_DECLARE(usb_id,LKDDB_USB_ID)

#endif
