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

#include <stdio.h>

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

/* Create a new LHQ_USB_RESULT

   @returns pointer to the new LHQ_USB_RESULT
*/
LHQ_USB_RESULT *lhq_usb_result_new() {
    LHQ_USB_RESULT *result = (LHQ_USB_RESULT *)calloc(1, sizeof(LHQ_USB_RESULT));
    return result;
}

/* Search for a USB Class in the Index

   @param ids      - the IDs Index
   @param class    - the USB Class to find
   @param subclass - the USB Subclass to find
   @param protocol - the USB Protocol to find
*/
void lhq_usb_result_class_search(LHQ_IDS_INDEX *ids,
                                 LKDDB_USB_CLASS_ID *class,
                                 LKDDB_USB_CLASS_ID *subclass,
                                 LKDDB_USB_CLASS_ID *protocol) {
    unsigned int length = ids->lists[LHQ_ID_USB_CLASS]->length;
    unsigned int index  = lhq_usb_class_id_search_and_copy(ids, class, 0);
    if(index != length) {
        index = lhq_usb_class_id_search_and_copy(ids, subclass, index + 1);
        if(index != length) {
            index = lhq_usb_class_id_search_and_copy(ids, protocol, index + 1);
        }
    }
}

/* Search for a USB Device in the Index

   @param result - the result to fill
   @param ids    - the IDs Index
   @param types  - the Types Index
*/
void lhq_usb_result_search(LHQ_USB_RESULT *result, LHQ_IDS_INDEX *ids, LHQ_TYPES_INDEX *types) {

    lhq_usb_id_vendor(&result->entry.id, &result->vendor);

    unsigned int length = ids->lists[LHQ_ID_USB]->length;
    unsigned int index  = lhq_usb_id_search_and_copy(ids, &result->vendor, 0);
    if(index == length) {
        return;
    }
    index = lhq_usb_id_search_and_copy(ids, &result->entry.id, index + 1);
    if(index == length) {
        return;
    }
    lhq_usb_class_id_class(&result->entry.class, &result->class);
    lhq_usb_class_id_subclass(&result->entry.class, &result->subclass);
    lhq_usb_result_class_search(ids, &result->class, &result->subclass, &result->entry.class);

    lhq_usb_class_id_class(&result->entry.interfaceClass, &result->interfaceClass);
    lhq_usb_class_id_subclass(&result->entry.interfaceClass, &result->interfaceSubclass);
    lhq_usb_result_class_search(ids, &result->interfaceClass, &result->interfaceSubclass,
                                &result->entry.interfaceClass);

    lhq_usb_search_and_copy(types, &result->entry, 0);
}

/* Print a summary of this USB Result

   @param result - the result to print
   @param out    - the file to write to
*/
void lhq_usb_result_entry_print(LHQ_USB_RESULT *result, FILE *out) {
    fprintf(out, "USB Result:\n");
    fprintf(out, "\tUSB ID: %s:%s\n", result->entry.id.vendor, result->entry.id.product);
    fprintf(out, "\tUSB Vendor: %s\n", result->vendor.name);
    fprintf(out, "\tUSB Product: %s\n", result->entry.id.name);
    fprintf(out, "\tKernel Config Options: %s\n", result->entry.configOpts);
    fprintf(out, "\tKernel Source File: %s\n", result->entry.filename);
}

/* Free memory allocated to this result

   @param result - the result to free
*/
void lhq_usb_result_free(LHQ_USB_RESULT *result) {
    free(result->entry.id.vendor);
    free(result->entry.id.product);
}

/* define the lhq_usb_result_list functions */
LHQ_LIST_DECLARE(usb_result, LHQ_USB_RESULT)

#endif
