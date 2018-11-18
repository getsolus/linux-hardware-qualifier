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

#include <string.h>
#include "lhq_string.h"

/* USB Device Format string for fscanf */
const char * LKDDB_USB_FORMAT = "usb %s %s %s %s %s %s %s %s %s %s : %[^:] : %s\n";

/* Representation of a LKDDB USB Entry */
typedef struct {
    char  idVendor[5];
    char  idProduct[5];

    char  bDeviceClass[2];
    char  bDeviceSubClass[2];
    char  bDeviceProtocol[2];
    char  bInterfaceClass[2];
    char  bInterfaceSubClass[2];
    char  bInterfaceProtocol[2];

    char  bcdDeviceLo[4];
    char  bcdDeviceHi[4];

    LHQ_STRING *configOpts;
    LHQ_STRING *filename;
} LKDDB_USB_ENTRY;

/* Create a new LKDDB_USB_ENTRY

   @param contents - pointer to the actual string contents
   @param length   - length of the string (excluding \0 terminator)

   @retruens pointer to ther new LHQ_STRING
*/
LKDDB_USB_ENTRY* lhq_usb_entry_new() {
    LKDDB_USB_ENTRY *result = (LKDDB_USB_ENTRY*)calloc(1,sizeof(LKDDB_USB_ENTRY));
    result->configOpts = lhq_string_new((char*)calloc(100,sizeof(char)), 100);
    result->filename   = lhq_string_new((char*)calloc(100,sizeof(char)), 100);
    return result;
}

int lhq_usb_entry_parse(LKDDB_USB_ENTRY *entry, FILE * file) {
    char buff[100];
    if( fscanf(file,  "%s",  buff) != 1 ) return -1;
    if( strlen(buff) != 3 ) return -1;
    if( strncmp(buff, "usb", (size_t)3)  != 0 ) return -1;
    if( fscanf(file,  "%s",  buff) != 1 ) return -1;
    fprintf(stderr, "First: %s\n", buff);
    strncpy(entry->idVendor, buff, 4);
    if( fscanf(file, "%s", buff)   != 1 ) return -1;
    strncpy(entry->idProduct, buff, 4);
    return 0;
}

void lhq_usb_entry_print(LKDDB_USB_ENTRY *entry, FILE *out) {
    fprintf(out, "USB Entry:\n");
    fprintf(out, "\tIDs: %s:%s\n", entry->idVendor, entry->idProduct);
}

/* Destroy an LHQ_STRING */
void lhq_usb_entry_free(LKDDB_USB_ENTRY *entry) {
    lhq_string_free(entry->configOpts);
    lhq_string_free(entry->filename);
    free(entry);
}


#endif
