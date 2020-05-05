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

#ifndef __LINUX_HARDWARE_QUALIFIER_USB_CLASS_IDS_H__
#define __LINUX_HARDWARE_QUALIFIER_USB_CLASS_IDS_H__

#include "lhq_list.h"

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

/* Convert a full USB Class ID to one suitable for finding just the USB Class

   @param entry - the full USB Class ID
   @param class - the extracted USB Class
*/
void lhq_usb_class_id_class(LKDDB_USB_CLASS_ID *entry, LKDDB_USB_CLASS_ID *class);

/* Convert a full USB Class ID to one suitable for finding just the USB Subclass

   @param entry    - the full USB Class ID
   @param subclass - the extracted USB Subclass
*/
void lhq_usb_class_id_subclass(LKDDB_USB_CLASS_ID *entry, LKDDB_USB_CLASS_ID *subclass);

/* Check if entry is the same as other, copy pointers from other if so

   @param entry - the entry to copy to
   @param other - the entry to compare against and copy from
   @returns 0 if equal otherwise < 0 or > 0
*/
int lhq_usb_class_id_compare_and_copy(LKDDB_USB_CLASS_ID *entry, LKDDB_USB_CLASS_ID *other);

/* Parse the next available USB Class ID

   @param entry - the entry to parse into
   @param file  - the file to read from
   @returns 0 if no more to read, 1 if more
*/
int lhq_usb_class_id_entry_parse(LKDDB_USB_CLASS_ID *entry, char **file);

/* define the lhq_class_id_list functions */
LHQ_LIST_DECLARE(usb_class_id, LKDDB_USB_CLASS_ID)

#endif
