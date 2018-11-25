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
#include "index.h"
#include "config.h"

LHQ_INDEX * lhq_build_types_index(FILE *lkddb) {
    LHQ_INDEX *typesIndex = lhq_index_new(3, lkddb);

    lhq_acpi(typesIndex);
    lhq_pci(typesIndex);
    lhq_usb(typesIndex);
    //lhq_index_summary(typesIndex);
    return typesIndex;
}

LHQ_INDEX * lhq_build_ids_index(FILE *ids) {
    LHQ_INDEX *idsIndex = lhq_index_new(4, ids);

    lhq_pci_class_ids(idsIndex);
    lhq_pci_ids(idsIndex);
    lhq_usb_class_ids(idsIndex);
    lhq_usb_ids(idsIndex);
    //lhq_index_summary(idsIndex);
    return idsIndex;
}

int main() {
    FILE *lkddb = fopen(DATADIR"/"PACKAGE_NAME"/lkddb.list", "r");
    if(lkddb == NULL) {
        fprintf(stderr, "Failed to open '%s'. Exiting.\n", DATADIR"/"PACKAGE_NAME"/lkddb.list");
        return -1;
    }
    FILE *ids = fopen("data/ids.list", "r");
    if(ids == NULL) {
        fprintf(stderr, "Failed to open '%s'. Exiting.\n", "data/ids.list");
        return -1;
    }
    LHQ_INDEX *typesIndex = lhq_build_types_index(lkddb);
    LHQ_INDEX *idsIndex = lhq_build_ids_index(ids);
    fclose(lkddb);
    fclose(ids);

    lhq_index_free(typesIndex);
    lhq_index_free(idsIndex);
    return 0;
}
