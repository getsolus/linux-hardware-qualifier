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

#ifndef __LINUX_HARDWARE_QUALIFIER_USB_RESULT_H__
#define __LINUX_HARDWARE_QUALIFIER_USB_RESULT_H__

#include "lhq_types.h"

#include "ids_index.h"
#include "types_index.h"

/* Representation of a LHQ USB Search Result

   @field entry             - the original USB Entry
   @field vendor            - the USB Vendor
   @field class             - the USB Device Class
   @field subclass          - the USB Device Subclass
   @field interfaceClass    - the USB Interface Class
   @field interfaceSubclass - the USB Interface Subclass
*/
typedef struct {
    LKDDB_USB_ENTRY entry;
    LKDDB_USB_ID vendor;
    LKDDB_USB_CLASS_ID class;
    LKDDB_USB_CLASS_ID subclass;
    LKDDB_USB_CLASS_ID interfaceClass;
    LKDDB_USB_CLASS_ID interfaceSubclass;
} LHQ_USB_RESULT;

/* Search for a USB Class in the Index

   @param ids      - the IDs Index
   @param class    - the USB Class to find
   @param subclass - the USB Subclass to find
   @param protocol - the USB Protocol to find
*/
void lhq_usb_result_class_search(LHQ_IDS_INDEX *ids,
                                 LKDDB_USB_CLASS_ID *class,
                                 LKDDB_USB_CLASS_ID *subclass,
                                 LKDDB_USB_CLASS_ID *protocol);

/* Search for a USB Device in the Index

   @param result - the result to fill
   @param ids    - the IDs Index
   @param types  - the Types Index
*/
void lhq_usb_result_search(LHQ_USB_RESULT *result, LHQ_IDS_INDEX *ids, LHQ_TYPES_INDEX *types);

/* Print a summary of this USB Result

   @param result - the result to print
   @param out    - the file to write to
*/
void lhq_usb_result_entry_print(LHQ_USB_RESULT *result, FILE *out);

/* Free memory allocated to this result

   @param result - the result to free
*/
void lhq_usb_result_free(LHQ_USB_RESULT *result);

/* define the lhq_usb_result_list functions */
LHQ_LIST_DECLARE(usb_result, LHQ_USB_RESULT)

#endif
