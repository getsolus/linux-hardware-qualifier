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

#include <stdio.h>
#include <string.h>
#include "index.h"
#include "lhq_types.h"

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

   @param contents - pointer to the actual string contents
   @param length   - length of the string (excluding \0 terminator)

   @retruens pointer to ther new LKDDB_USB_CLASS_ID
*/
LKDDB_USB_CLASS_ID* lhq_usb_class_id_new() {
    LKDDB_USB_CLASS_ID *result = (LKDDB_USB_CLASS_ID*)calloc(1,sizeof(LKDDB_USB_CLASS_ID));
    return result;
}

/* Parse the next available USB Class ID

   @param entry - the entry to parse into
   @param file  - the file to read from
   @returns 0 if no mroe to read, 1 if more
*/
int lhq_usb_class_id_entry_parse(LKDDB_USB_CLASS_ID *entry, char ** file) {
    /* class */
    *file = strchr(*file, ' ') + 1;
    entry->bClass    = *file;
    /* subclass */
    *file = strchr(*file, ' ') + 1;
    (*file)[-1] = '\0';
    entry->bSubClass = *file;
    /* protocol */
    *file = strchr(*file, ' ') + 1;
    (*file)[-1] = '\0';
    entry->bProtocol = *file;
    /* name */
    *file = strchr(*file, ' ') + 1;
    (*file)[-1] = '\0';
    entry->name = *file;
    *file = strchr(*file, '\n');
    /* check for more */
    if( *file != NULL ){
        (*file)++;
        (*file)[-1] = '\0';
        if( strncmp(*file, "usb_class_ids", 13) != 0 ){
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

/* declare the USB Class ID list type */
LKDDB_LIST_DECLARE(usb_class_id,LKDDB_USB_CLASS_ID)

/* Parse all USB Class IDS from the Index

   @param index - the index to populate
*/
void lhq_usb_class_ids(LHQ_INDEX* index) {
    LKDDB_USB_CLASS_ID entry;
    LKDDB_LIST *list = index->lists[LHQ_ID_USB_CLASS];
    index->cursor = strstr(index->cursor, "\nusb_class_ids");
    while( lhq_usb_class_id_entry_parse(&entry, &(index->cursor)) ){
        lhq_list_append(list, (void*)&entry);
    }
    lhq_list_append(list, (void*)&entry);
    lhq_list_compact(list);
#ifdef LHQ_DEBUG
#if LHQ_DEBUG > 0
    fprintf(stderr, "Length: %d, Capacity: %d\n", list->length, list->capacity);
    lhq_usb_class_id_list_print(list,stderr);
#endif
#endif
}

#endif
