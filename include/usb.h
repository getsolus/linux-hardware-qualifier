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
#include "lhq_types.h"
#include "usb_class_ids.h"
#include "usb_ids.h"

/* Representation of a LKDDB USB Entry

   @field id             - the USB device ID
   @field class          - the USB device Class
   @field interfaceClass - the USB Device Class of the interface
   @field bcdDeviceLo    - lower byte of BCD Device
   @field bcdDeviceHi    - upper byte of BCD Device
   @field configOpts     - the configuration options required for this device to be supported by the kernel
   @field filename       - the source file where this device is declared
*/
typedef struct {
    LKDDB_USB_ID id;
    LKDDB_USB_CLASS_ID class;
    LKDDB_USB_CLASS_ID interfaceClass;

    char  *bcdDeviceLo;
    char  *bcdDeviceHi;

    char  *configOpts;
    char  *filename;
} LKDDB_USB_ENTRY;

/* Create a new LKDDB_USB_ENTRY

   @param contents - pointer to the actual string contents
   @param length   - length of the string (excluding \0 terminator)

   @returns pointer to ther new LKDDB_USB_ENTRY
*/
LKDDB_USB_ENTRY* lhq_usb_entry_new() {
    LKDDB_USB_ENTRY *result = (LKDDB_USB_ENTRY*)calloc(1,sizeof(LKDDB_USB_ENTRY));
    return result;
}

/* Parse a USB Entry from a file

   @param entry - the entry to parse into
   @param file  - the file to read from
   @returns 0 if there are no more to parse, 1 if more to parse
*/
int lhq_usb_entry_parse(LKDDB_USB_ENTRY *entry, char ** file) {
    /* id */
    *file = strchr(*file, ' ') + 1;
    entry->id.vendor           = *file;
    *file = strchr(*file, ' ') + 1;
    (*file)[-1] = '\0';
    entry->id.product          = *file;
    /* class */
    *file = strchr(*file, ' ') + 1;
    (*file)[-1] = '\0';
    entry->class.bClass       = *file;
    *file = strchr(*file, ' ') + 1;
    (*file)[-1] = '\0';
    entry->class.bSubClass    = *file;
    *file = strchr(*file, ' ') + 1;
    (*file)[-1] = '\0';
    entry->class.bProtocol    = *file;
    /* interface class */
    *file = strchr(*file, ' ') + 1;
    (*file)[-1] = '\0';
    entry->interfaceClass.bClass    = *file;
    *file = strchr(*file, ' ') + 1;
    (*file)[-1] = '\0';
    entry->interfaceClass.bSubClass = *file;
    *file = strchr(*file, ' ') + 1;
    (*file)[-1] = '\0';
    entry->interfaceClass.bProtocol = *file;
    /* bcd */
    *file = strchr(*file, ' ') + 1;
    (*file)[-1] = '\0';
    entry->bcdDeviceLo        = *file;
    *file = strchr(*file, ' ') + 1;
    (*file)[-1] = '\0';
    entry->bcdDeviceHi        = *file;
    /* config options */
    *file = strchr(*file, ':') + 2;
    (*file)[-3] = '\0';
    entry->configOpts         = *file;
    /* filename */
    *file = strchr(*file, ':') + 2;
    (*file)[-3] = '\0';
    entry->filename           = *file;
    *file = strstr(*file, "\n");
    /* check for more */
    if( *file != NULL ){
        (*file)++;
        (*file)[-1] = '\0';
        if( strncmp(*file, "usb", 3) != 0 ){
            return 0;
        }
    }
    return 1;
}

/* Print a summary of this USB Entry

   @param entry - the entry to print
   @param out   - the file to write to
*/
void lhq_usb_entry_print(LKDDB_USB_ENTRY *entry, FILE *out) {
    fprintf(out, "USB Entry:\n");
    lhq_usb_id_entry_print(&(entry->id),out);
    lhq_usb_class_id_entry_print(&(entry->class),out);
    lhq_usb_class_id_entry_print(&(entry->interfaceClass),out);
    fprintf(out, "\tBCD: %s:%s\n", entry->bcdDeviceLo, entry->bcdDeviceHi);
    fprintf(out, "\tConfig Options: %s\n", entry->configOpts);
    fprintf(out, "\tSource: %s\n", entry->filename);
}

/* declare the USB list type */
LKDDB_LIST_DECLARE(usb,LKDDB_USB_ENTRY)

#endif
