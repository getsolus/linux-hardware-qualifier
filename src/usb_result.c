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

#include "usb_result.h"

#include "util.h"

#include <stdio.h>

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

const char *lhq_usb_result_format = "USB Result:\n\
\tUSB ID: %s:%s\n\
\tUSB Vendor: %s\n\
\tUSB Product: %s\n\
\tKernel Config Options: %s\n\
\tKernel Source File: %s\n\
";

/* Print a summary of this USB Result

   @param result - the result to print
   @param out    - the file to write to
*/
void lhq_usb_result_entry_print(LHQ_USB_RESULT *result, FILE *out) {
    fprintf(out, lhq_usb_result_format, result->entry.id.vendor, result->entry.id.product, result->vendor.name,
            result->entry.id.name, result->entry.configOpts, result->entry.filename);
}

/* Free memory allocated to this result

   @param result - the result to free
*/
void lhq_usb_result_free(LHQ_USB_RESULT *result) {
    AUTO_FREE(result->entry.id.vendor);
    AUTO_FREE(result->entry.id.product);
    AUTO_FREE(result->entry.class.bClass);
    AUTO_FREE(result->entry.class.bSubClass);
    AUTO_FREE(result->entry.class.bProtocol);
    AUTO_FREE(result->entry.interfaceClass.bClass);
    AUTO_FREE(result->entry.interfaceClass.bSubClass);
    AUTO_FREE(result->entry.interfaceClass.bProtocol);
}

/* define the lhq_usb_result_list functions */
LHQ_LIST_DEFINE(usb_result, LHQ_USB_RESULT)
