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

#ifndef __LINUX_HARDWARE_QUALIFIER_USB_H__
#define __LINUX_HARDWARE_QUALIFIER_USB_H__

#include <stdio.h>
#include <string.h>
#include "lhq_string.h"
#include "lhq_list.h"

/* USB Device Format string for fscanf */
const char * LKDDB_USB_FORMAT = "usb %s %s %s %s %s %s %s %s %s %s : %[^:\n] : %s\n";

/* Representation of a LKDDB USB Entry */
typedef struct {
    char  idVendor[5];
    char  idProduct[5];

    char  bDeviceClass[3];
    char  bDeviceSubClass[3];
    char  bDeviceProtocol[3];
    char  bInterfaceClass[3];
    char  bInterfaceSubClass[3];
    char  bInterfaceProtocol[3];

    char  bcdDeviceLo[5];
    char  bcdDeviceHi[5];

    LHQ_STRING configOpts;
    LHQ_STRING filename;
} LKDDB_USB_ENTRY;

/* Create a new LKDDB_USB_ENTRY

   @param contents - pointer to the actual string contents
   @param length   - length of the string (excluding \0 terminator)

   @returns pointer to ther new LHQ_STRING
*/
LKDDB_USB_ENTRY* lhq_usb_entry_new() {
    LKDDB_USB_ENTRY *result = (LKDDB_USB_ENTRY*)calloc(1,sizeof(LKDDB_USB_ENTRY));
    return result;
}

int lhq_usb_entry_parse(LKDDB_USB_ENTRY *entry, FILE * file) {
    return fscanf(file, LKDDB_USB_FORMAT,
                  entry->idVendor,        entry->idProduct,
                  entry->bDeviceClass,    entry->bDeviceSubClass,    entry->bDeviceProtocol,
                  entry->bInterfaceClass, entry->bInterfaceSubClass, entry->bInterfaceProtocol,
                  entry->bcdDeviceLo,     entry->bcdDeviceHi,
                  entry->configOpts, entry->filename
    ) == 12;
}

void lhq_usb_entry_print(LKDDB_USB_ENTRY *entry, FILE *out) {
    fprintf(out, "USB Entry:\n");
    fprintf(out, "\tIDs: %s:%s\n", entry->idVendor, entry->idProduct);
    fprintf(out, "\tDevice: %s:%s:%s\n", entry->bDeviceClass, entry->bDeviceSubClass, entry->bDeviceProtocol);
    fprintf(out, "\tInterface: %s:%s:%s\n", entry->bInterfaceClass, entry->bInterfaceSubClass, entry->bInterfaceProtocol);
    fprintf(out, "\tBCD: %s:%s\n", entry->bcdDeviceLo, entry->bcdDeviceHi);
    fprintf(out, "\tConfig Options: %s\n", entry->configOpts);
    fprintf(out, "\tSource: %s\n", entry->filename);
}

LKDDB_LIST_DECLARE(usb,LKDDB_USB_ENTRY)

void lhq_usb(FILE * lkddb) {
    rewind(lkddb);
    LKDDB_USB_ENTRY entry;
    LKDDB_LIST *list = lhq_usb_list_new();
    while(!feof(lkddb) ){
        if( lhq_usb_entry_parse(&entry, lkddb) ){
            lhq_usb_list_append(list, &entry);
        } else {
            while(!feof(lkddb) && getc(lkddb) != '\n');
        }
    }
    fprintf(stderr, "Length: %d, Capacity: %d\n", list->length, list->capacity);
    //lhq_usb_list_print(list,stderr);
    lhq_usb_list_free(list);
}

#endif
