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

#ifndef __LINUX_HARDWARE_QUALIFIER_PCI_RESULT_H__
#define __LINUX_HARDWARE_QUALIFIER_PCI_RESULT_H__

#include "lhq_types.h"
#include "ids_index.h"
#include "types_index.h"

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

/* Search for a PCI Class in the Index

   @param ids         - the IDs Index
   @param class       - the PCI Class to find
   @param subclass    - the PCI Subclass to find
   @param subsubclass - the PCI Subsubclass to find
*/
void lhq_pci_result_class_search(LHQ_IDS_INDEX *ids,
                                 LKDDB_PCI_CLASS_ID *class,
                                 LKDDB_PCI_CLASS_ID *subclass,
                                 LKDDB_PCI_CLASS_ID *subsubclass);

/* Search for a PCI Device in the Index

   @param result - the result to fill
   @param ids    - the IDs Index
   @param types  - the Types Index
*/
void lhq_pci_result_search(LHQ_PCI_RESULT *result, LHQ_IDS_INDEX *ids, LHQ_TYPES_INDEX *types);

/* Print a summary of this PCI Result

   @param result - the result to print
   @param out    - the file to write to
*/
void lhq_pci_result_entry_print(LHQ_PCI_RESULT *result, FILE *out);

/* Free memory allocated to this result

   @param result - the result to free
*/
void lhq_pci_result_free(LHQ_PCI_RESULT *result);

/* define the lhq_pci_result_list functions */
LHQ_LIST_DECLARE(pci_result, LHQ_PCI_RESULT)

#endif
