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

#include <stdio.h>
#include "acpi.h"
#include "pci.h"
#include "pci_class_ids.h"
#include "pci_ids.h"
#include "usb.h"
#include "usb_class_ids.h"
#include "usb_ids.h"

int main() {
    FILE *lkddb = fopen("data/lkddb.list", "r");
    if(!lkddb) {
        fprintf(stderr, "Failed to open 'data/lkddb.list'. Exiting.\n");
    }
    FILE *lkddb_ids = fopen("data/ids.list", "r");
    if(!lkddb_ids) {
        fprintf(stderr, "Failed to open 'data/ids.list'. Exiting.\n");
    }
    lhq_acpi(lkddb);
    lhq_pci(lkddb);
    lhq_usb(lkddb);

    lhq_pci_class_ids(lkddb_ids);
    lhq_pci_ids(lkddb_ids);
    lhq_usb_class_ids(lkddb_ids);
    lhq_usb_ids(lkddb_ids);
    fclose(lkddb);
    fclose(lkddb_ids);
    return 0;
}
