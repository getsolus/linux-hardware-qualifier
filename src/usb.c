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

#include "usb.h"

#include <string.h>

/* Parse a USB Entry from a file

   @param entry - the entry to parse into
   @param file  - the file to read from
   @returns 0 if there are no more to parse, 1 if more to parse
*/
int lhq_usb_entry_parse(LKDDB_USB_ENTRY *entry, char **file) {
    /* id */
    *file             = strchr(*file, ' ') + 1;
    entry->id.vendor  = *file;
    *file             = strchr(*file, ' ') + 1;
    (*file)[-1]       = '\0';
    entry->id.product = *file;
    /* class */
    *file                  = strchr(*file, ' ') + 1;
    (*file)[-1]            = '\0';
    entry->class.bClass    = *file;
    *file                  = strchr(*file, ' ') + 1;
    (*file)[-1]            = '\0';
    entry->class.bSubClass = *file;
    *file                  = strchr(*file, ' ') + 1;
    (*file)[-1]            = '\0';
    entry->class.bProtocol = *file;
    /* interface class */
    *file                           = strchr(*file, ' ') + 1;
    (*file)[-1]                     = '\0';
    entry->interfaceClass.bClass    = *file;
    *file                           = strchr(*file, ' ') + 1;
    (*file)[-1]                     = '\0';
    entry->interfaceClass.bSubClass = *file;
    *file                           = strchr(*file, ' ') + 1;
    (*file)[-1]                     = '\0';
    entry->interfaceClass.bProtocol = *file;
    /* bcd */
    *file              = strchr(*file, ' ') + 1;
    (*file)[-1]        = '\0';
    entry->bcdDeviceLo = *file;
    *file              = strchr(*file, ' ') + 1;
    (*file)[-1]        = '\0';
    entry->bcdDeviceHi = *file;
    /* config options */
    *file             = strchr(*file, ':') + 2;
    (*file)[-3]       = '\0';
    entry->configOpts = *file;
    /* filename */
    *file           = strchr(*file, ':') + 2;
    (*file)[-3]     = '\0';
    entry->filename = *file;
    *file           = strstr(*file, "\n");
    /* check for more */
    if(*file != NULL) {
        (*file)++;
        (*file)[-1] = '\0';
        if(strncmp(*file, "usb", 3) != 0) {
            return 0;
        }
    }
    return 1;
}

/* Check if entry is the same as other, copy pointers from other if so

   @param entry - the entry to copy to
   @param other - the entry to compare against and copy from
   @returns 0 if equal otherwise < 0 or > 0
*/
int lhq_usb_compare_and_copy(LKDDB_USB_ENTRY *entry, LKDDB_USB_ENTRY *other) {
    int compare = lhq_usb_id_compare_and_copy(&entry->id, &other->id);
    if(compare == 0) {
        entry->configOpts = other->configOpts;
        entry->filename   = other->filename;
    }
    return compare;
}

/* define the lhq_usb_list functions */
LHQ_LIST_DEFINE(usb, LKDDB_USB_ENTRY)
