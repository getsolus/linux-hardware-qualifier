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

#ifndef __LINUX_HARDWARE_QUALIFIER_PCI_CLASS_IDS_H__
#define __LINUX_HARDWARE_QUALIFIER_PCI_CLASS_IDS_H__

#include "lhq_list.h"

/* Representation of a LKDDB PCI Class ID

   @field classMask - the six character hexadecimal mask for a PCI Class
   @field name      - the name of the PCI Class
*/
typedef struct {
    char *classMask;
    char *name;
} LKDDB_PCI_CLASS_ID;

/* Convert a full PCI Class ID to one suitable for finding just the PCI Class

   @param entry - the full PCI Class ID
   @param class - the extracted PCI Class
*/
void lhq_pci_class_id_class(LKDDB_PCI_CLASS_ID *entry, LKDDB_PCI_CLASS_ID *class);

/* Convert a full PCI Class ID to one suitable for finding just the PCI Subclass

   @param entry    - the full PCI Class ID
   @param subclass - the extracted PCI Subclass
*/
void lhq_pci_class_id_subclass(LKDDB_PCI_CLASS_ID *entry, LKDDB_PCI_CLASS_ID *subclass);

/* Check if entry is the same as other, copy pointers from other if so

   @param entry - the entry to copy to
   @param other - the entry to compare against and copy from
   @returns 0 if equal otherwise < 0 or > 0
*/
int lhq_pci_class_id_compare_and_copy(LKDDB_PCI_CLASS_ID *entry, LKDDB_PCI_CLASS_ID *other);

/* Parse a PCI Class ID from a file

   @param entry - the entry to write into
   @param file  - the file to read from

   @returns 0 if done, 1 if more to parse
*/
int lhq_pci_class_id_entry_parse(LKDDB_PCI_CLASS_ID *entry, char **file);

/* define the lhq_class_id_list functions */
LHQ_LIST_DECLARE(pci_class_id, LKDDB_PCI_CLASS_ID)

#endif
