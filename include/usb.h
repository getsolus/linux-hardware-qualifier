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

#ifndef __LINUX_HARDWARE_QUALIFIER_USB_H__
#define __LINUX_HARDWARE_QUALIFIER_USB_H__

#include "lhq_list.h"

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

    char *bcdDeviceLo;
    char *bcdDeviceHi;

    char *configOpts;
    char *filename;
} LKDDB_USB_ENTRY;

/* Parse a USB Entry from a file

   @param entry - the entry to parse into
   @param file  - the file to read from
   @returns 0 if there are no more to parse, 1 if more to parse
*/
int lhq_usb_entry_parse(LKDDB_USB_ENTRY *entry, char **file);

/* Check if entry is the same as other, copy pointers from other if so

   @param entry - the entry to copy to
   @param other - the entry to compare against and copy from
   @returns 0 if equal otherwise < 0 or > 0
*/
int lhq_usb_compare_and_copy(LKDDB_USB_ENTRY *entry, LKDDB_USB_ENTRY *other);

/* define the lhq_usb_list functions */
LHQ_LIST_DECLARE(usb, LKDDB_USB_ENTRY)

#endif
