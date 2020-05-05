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

#include "usb_ids.h"

#include <string.h>

/* Convert a full USB ID to one suitable for finding just the USB Vendor

   @param entry  - the full USB ID
   @param vendor - the extracted USB Vendor
*/
void lhq_usb_id_vendor(LKDDB_USB_ID *entry, LKDDB_USB_ID *vendor) {
    vendor->vendor  = entry->vendor;
    vendor->product = "....";
}

/* Check if entry is the same as other, copy pointers from other if so

   @param entry - the entry to copy to
   @param other - the entry to compare against and copy from
   @returns 0 if equal otherwise < 0 or > 0
*/
int lhq_usb_id_compare_and_copy(LKDDB_USB_ID *entry, LKDDB_USB_ID *other) {
    int compare = strncmp(entry->vendor, other->vendor, 4);
    if(compare != 0) return compare;
    compare = strncmp(entry->product, other->product, 4);
    if((entry->name == NULL) && (compare == 0)) {
        entry->name = other->name;
    }
    return compare;
}

/* Parse a LKDDB_USB_ID from a file

   @param entry - the entry to parse into
   @param file  - the file to parse
   @returns 0 if no more to parse, 1 if more
*/
int lhq_usb_id_entry_parse(LKDDB_USB_ID *entry, char **file) {
    /* vendor */
    *file         = strchr(*file, ' ') + 1;
    entry->vendor = *file;
    /* product */
    *file          = strchr(*file, ' ') + 1;
    (*file)[-1]    = '\0';
    entry->product = *file;
    /* name */
    *file       = strchr(*file, ' ') + 1;
    (*file)[-1] = '\0';
    entry->name = *file;
    *file       = strstr(*file, "\n");
    /* check for more */
    if(*file != NULL) {
        (*file)++;
        (*file)[-1] = '\0';
        if(strncmp(*file, "usb_ids", 7) != 0) {
            return 0;
        }
    }
    return 1;
}

/* define the lhq_usb_id_list functions */
LHQ_LIST_DEFINE(usb_id, LKDDB_USB_ID)
