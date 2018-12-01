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

#ifndef __LINUX_HARDWARE_QUALIFIER_PCI_RESULT_H__
#define __LINUX_HARDWARE_QUALIFIER_PCI_RESULT_H__

#include "ids_index.h"
#include "lhq_types.h"
#include "pci.h"
#include "pci_class_ids.h"
#include "pci_ids.h"
#include "types_index.h"
#include <stdio.h>
#include <string.h>

/* Representation of a LHQ PCI Search Result

   @field entry             - the original PCI Entry
   @field vendor            - the PCI Vendor
   @field class             - the PCI Device Class
   @field class             - the PCI Device Subclass
*/
typedef struct {
    LKDDB_PCI_ENTRY entry;
    LKDDB_PCI_ID vendor;
    LKDDB_PCI_CLASS_ID class;
    LKDDB_PCI_CLASS_ID subclass;
} LHQ_PCI_RESULT;

/* Create a new LHQ_PCI_RESULT

   @returns pointer to the new LHQ_PCI_RESULT
*/
LHQ_PCI_RESULT *lhq_pci_result_new() {
    LHQ_PCI_RESULT *result = (LHQ_PCI_RESULT *)calloc(1, sizeof(LHQ_PCI_RESULT));
    return result;
}

/* Search for a PCI Class in the Index

   @param ids         - the IDs Index
   @param class       - the PCI Class to find
   @param subclass    - the PCI Subclass to find
   @param subsubclass - the PCI Subsubclass to find
*/
void lhq_pci_result_class_search(LHQ_IDS_INDEX *ids,
                                 LKDDB_PCI_CLASS_ID *class,
                                 LKDDB_PCI_CLASS_ID *subclass,
                                 LKDDB_PCI_CLASS_ID *subsubclass) {

    unsigned int length = ids->lists[LHQ_ID_PCI_CLASS]->length;
    unsigned int index  = lhq_pci_class_id_search_and_copy(ids, class, 0);
    if(index != length) {
        index = lhq_pci_class_id_search_and_copy(ids, subclass, index + 1);
        if(index != length) {
            index = lhq_pci_class_id_search_and_copy(ids, subsubclass, index + 1);
        }
    }
}

/* Search for a PCI Device in the Index

   @param result - the result to fill
   @param ids    - the IDs Index
   @param types  - the Types Index
*/
void lhq_pci_result_search(LHQ_PCI_RESULT *result, LHQ_IDS_INDEX *ids, LHQ_TYPES_INDEX *types) {

    lhq_pci_id_vendor(&result->entry.id, &result->vendor);

    unsigned int length = ids->lists[LHQ_ID_PCI]->length;
    unsigned int index  = lhq_pci_id_search_and_copy(ids, &result->vendor, 0);
    if(index == length) {
        return;
    }
    index = lhq_pci_id_search_and_copy(ids, &(result->entry.id), index + 1);
    if(index == length) {
        return;
    }
    lhq_pci_class_id_class(&result->entry.class, &result->class);
    lhq_pci_class_id_subclass(&result->entry.class, &result->subclass);
    lhq_pci_result_class_search(ids, &result->class, &result->subclass, &result->entry.class);
    lhq_pci_search_and_copy(types, &result->entry, 0);
}

/* Print a summary of this PCI Result

   @param result - the result to print
   @param out    - the file to write to
*/
void lhq_pci_result_entry_print(LHQ_PCI_RESULT *result, FILE *out) {
    fprintf(out, "PCI Result:\n");
    fprintf(out, "\tPCI ID: %s:%s\n", result->entry.id.vendor, result->entry.id.device);
    fprintf(out, "\tPCI Vendor: %s\n", result->vendor.name);
    fprintf(out, "\tPCI Product: %s\n", result->entry.id.name);
    fprintf(out, "\tKernel Config Options: %s\n", result->entry.configOpts);
    fprintf(out, "\tKernel Source File: %s\n", result->entry.filename);
}

/* define the lhq_pci_result_list functions */
LHQ_LIST_DECLARE(pci_result, LHQ_PCI_RESULT)

#endif
