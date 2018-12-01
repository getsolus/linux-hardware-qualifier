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

#ifndef __LINUX_HARDWARE_QUALIFIER_USB_CLASS_IDS_H__
#define __LINUX_HARDWARE_QUALIFIER_USB_CLASS_IDS_H__

#include "lhq_list.h"

#include <stdio.h>
#include <string.h>

/* Representation of a LKDDB USB Class ID

   @param bClass    - the 4 character hexadecimal USB Class ID
   @param bSubClass - the 4 character hexadecimal USB Subclass ID
   @param bProtocol - the 4 character hexadecimal USB Protocol ID
   @param name      - the name of this class
*/
typedef struct {
    char *bClass;
    char *bSubClass;
    char *bProtocol;

    char *name;
} LKDDB_USB_CLASS_ID;

/* Create a new LKDDB_USB_CLASS_ID

   @returns pointer to the new LKDDB_USB_CLASS_ID
*/
LKDDB_USB_CLASS_ID *lhq_usb_class_id_new() {
    LKDDB_USB_CLASS_ID *result = (LKDDB_USB_CLASS_ID *)calloc(1, sizeof(LKDDB_USB_CLASS_ID));
    return result;
}

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
    int compare = strcmp(entry->bClass, other->bClass);
    if(compare != 0) return compare;
    compare = strcmp(entry->bSubClass, other->bSubClass);
    if(compare != 0) return compare;
    compare = strcmp(entry->bProtocol, other->bProtocol);
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

/* Print a summary of this USB Class ID

   @param entry - the entry to print
   @param out   - the file to write to
*/
void lhq_usb_class_id_entry_print(LKDDB_USB_CLASS_ID *entry, FILE *out) {
    fprintf(out, "USB Class ID:\n");
    fprintf(out, "\tIDs: %s:%s:%s\n", entry->bClass, entry->bSubClass, entry->bProtocol);
    fprintf(out, "\tName: %s\n", entry->name);
}

/* define the lhq_class_id_list functions */
LHQ_LIST_DECLARE(usb_class_id, LKDDB_USB_CLASS_ID)

#endif
