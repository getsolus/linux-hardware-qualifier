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

#include "usb_class_ids.h"

#include <string.h>

/* Convert a full USB Class ID to one suitable for finding just the USB Class

   @param entry - the full USB Class ID
   @param class - the extracted USB Class
*/
void lhq_usb_class_id_class(LKDDB_USB_CLASS_ID *entry, LKDDB_USB_CLASS_ID *class) {
    class->bClass    = entry->bClass;
    class->bSubClass = "..";
    class->bProtocol = "..";
}

/* Convert a full USB Class ID to one suitable for finding just the USB Subclass

   @param entry    - the full USB Class ID
   @param subclass - the extracted USB Subclass
*/
void lhq_usb_class_id_subclass(LKDDB_USB_CLASS_ID *entry, LKDDB_USB_CLASS_ID *subclass) {
    subclass->bClass    = entry->bClass;
    subclass->bSubClass = entry->bSubClass;
    subclass->bProtocol = "..";
}

/* Check if entry is the same as other, copy pointers from other if so

   @param entry - the entry to copy to
   @param other - the entry to compare against and copy from
   @returns 0 if equal otherwise < 0 or > 0
*/
int lhq_usb_class_id_compare_and_copy(LKDDB_USB_CLASS_ID *entry, LKDDB_USB_CLASS_ID *other) {
    /* don't try to compare if entry has already been filled */
    if(entry->name != NULL) {
        return 0;
    }
    int compare = strncmp(entry->bClass, other->bClass, 2);
    if(compare != 0) return compare;
    compare = strncmp(entry->bSubClass, other->bSubClass, 2);
    if(compare != 0) return compare;
    compare = strncmp(entry->bProtocol, other->bProtocol, 2);
    if((entry->name == NULL) && (compare == 0)) {
        entry->name = other->name;
    }
    return compare;
}

/* Parse the next available USB Class ID

   @param entry - the entry to parse into
   @param file  - the file to read from
   @returns 0 if no more to read, 1 if more
*/
int lhq_usb_class_id_entry_parse(LKDDB_USB_CLASS_ID *entry, char **file) {
    /* class */
    *file         = strchr(*file, ' ') + 1;
    entry->bClass = *file;
    /* subclass */
    *file            = strchr(*file, ' ') + 1;
    (*file)[-1]      = '\0';
    entry->bSubClass = *file;
    /* protocol */
    *file            = strchr(*file, ' ') + 1;
    (*file)[-1]      = '\0';
    entry->bProtocol = *file;
    /* name */
    *file       = strchr(*file, ' ') + 1;
    (*file)[-1] = '\0';
    entry->name = *file;
    *file       = strstr(*file, "\n");
    /* check for more */
    if(*file != NULL) {
        (*file)++;
        (*file)[-1] = '\0';
        if(strncmp(*file, "usb_class_ids", 13) != 0) {
            return 0;
        }
    }
    return 1;
}

/* define the lhq_class_id_list functions */
LHQ_LIST_DEFINE(usb_class_id, LKDDB_USB_CLASS_ID)
