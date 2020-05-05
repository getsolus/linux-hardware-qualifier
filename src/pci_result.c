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

#include "pci_result.h"

#include "util.h"

#include <stdio.h>

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

const char *lhq_pci_result_format = "\
PCI Result:\n\
\tPCI ID: %s:%s\n\
\tPCI Vendor: %s\n\
\tPCI Product: %s\n\
\tPCI Subsystem: %s:%s\n\
\tKernel Config Options: %s\n\
\tKernel Source File: %s\n\
";

/* Print a summary of this PCI Result

   @param result - the result to print
   @param out    - the file to write to
*/
void lhq_pci_result_entry_print(LHQ_PCI_RESULT *result, FILE *out) {
    fprintf(out, lhq_pci_result_format, result->entry.id.vendor, result->entry.id.device, result->vendor.name,
            result->entry.id.name, result->entry.id.subVendor, result->entry.id.subDevice, result->entry.configOpts,
            result->entry.filename);
}

/* Free memory allocated to this result

   @param result - the result to free
*/
void lhq_pci_result_free(LHQ_PCI_RESULT *result) {
    AUTO_FREE(result->entry.id.device);
    AUTO_FREE(result->entry.id.vendor);
    AUTO_FREE(result->entry.id.subDevice);
    AUTO_FREE(result->entry.id.subVendor);
    AUTO_FREE(result->entry.class.classMask);
    AUTO_FREE(result->class.classMask);
    AUTO_FREE(result->subclass.classMask);
}

/* define the lhq_pci_result_list functions */
LHQ_LIST_DEFINE(pci_result, LHQ_PCI_RESULT)
