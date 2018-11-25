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

int main() {
    LHQ_INDEX *typesIndex = lhq_index_new(3, "data/lkddb.list");
    if( typesIndex == NULL ) {
        return -1;
    }
    LHQ_INDEX *idsIndex = lhq_index_new(4, "data/ids.list");
    if( idsIndex == NULL ) {
        lhq_index_free(typesIndex);
        return -1;
    }
    char * ptrID = (char*)idsIndex->raw;
    lhq_acpi(typesIndex);
    lhq_pci(typesIndex);
    lhq_pci_class_ids(&ptrID);
    lhq_pci_ids(&ptrID);
    lhq_usb(typesIndex);
    lhq_usb_class_ids(&ptrID);
    lhq_usb_ids(&ptrID);

    //lhq_index_summary(typesIndex);
    lhq_index_summary(idsIndex);

    lhq_index_free(typesIndex);
    lhq_index_free(idsIndex);
    return 0;
}
