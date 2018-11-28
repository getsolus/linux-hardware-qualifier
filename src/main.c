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
#include "ids_index.h"
#include "types_index.h"
#include "config.h"

LHQ_TYPES_INDEX * lhq_build_types_index(FILE *lkddb) {
    LHQ_TYPES_INDEX *index = lhq_types_index_new(lkddb);
    lhq_types_index_populate(index);
#ifdef LHQ_DEBUG
#if LHQ_DEBUG > 0
    lhq_types_index_summary(index);
#endif
#endif
    return index;
}

LHQ_IDS_INDEX * lhq_build_ids_index(FILE *ids) {
    LHQ_IDS_INDEX *idsIndex = lhq_ids_index_new(ids);

    lhq_ids_index_populate(idsIndex);
#ifdef LHQ_DEBUG
#if LHQ_DEBUG > 0
    lhq_ids_index_summary(idsIndex);
#endif
#endif
    return idsIndex;
}

void lhq_search_usb(LHQ_IDS_INDEX *ids, LHQ_TYPES_INDEX *types) {
    LKDDB_USB_ID entry = {
        .vendor  = "8087",
        .product = "0a2a"
    };
    LKDDB_USB_ID vendor = {
        .vendor  = entry.vendor,
        .product = "...."
    };
    unsigned int length = ids->lists[LHQ_ID_USB]->length;
    unsigned int index = lhq_usb_id_search_and_copy(ids, &vendor, 0);
    if( index == length ) {
        printf("Vendor Not Found.\n");
        return;
    }
    printf("Found Vendor:\n");
    lhq_usb_id_entry_print(&vendor, stdout);
    index = lhq_usb_id_search_and_copy(ids, &entry, index+1);
    if( index == length ) {
        printf("Device Not Found.\n");
        return;
    }
    printf("Found Device:\n");
    lhq_usb_id_entry_print(&entry, stdout);

    LKDDB_USB_CLASS_ID bClass = {
        .bClass    = "e0",
        .bSubClass = "..",
        .bProtocol = ".."
    };
    LKDDB_USB_CLASS_ID bSubClass = {
        .bClass    = bClass.bClass,
        .bSubClass = "01",
        .bProtocol = ".."
    };
    LKDDB_USB_CLASS_ID bProtocol = {
        .bClass    = bClass.bClass,
        .bSubClass = bSubClass.bSubClass,
        .bProtocol = "01"
    };
    length = ids->lists[LHQ_ID_USB_CLASS]->length;
    index = lhq_usb_class_id_search_and_copy(ids, &bClass, 0);
    if( index == length ) {
        printf("Class Not Found.\n");
    } else {
        printf("Found Class:\n");
        lhq_usb_class_id_entry_print(&bClass, stdout);
        index = lhq_usb_class_id_search_and_copy(ids, &bSubClass, index+1);
        if( index == length ) {
            printf("SubClass Not Found.\n");
        } else {
            printf("Found SubClass:\n");
            lhq_usb_class_id_entry_print(&bSubClass, stdout);
            index = lhq_usb_class_id_search_and_copy(ids, &bProtocol, index+1);
            if( index == length ) {
                printf("Protocol Not Found.\n");
            } else {
                printf("Found Protocol:\n");
                lhq_usb_class_id_entry_print(&bProtocol, stdout);
            }
        }
    }

    LKDDB_USB_CLASS_ID iClass = {
        .bClass    = "e0",
        .bSubClass = "..",
        .bProtocol = ".."
    };
    LKDDB_USB_CLASS_ID iSubClass = {
        .bClass    = iClass.bClass,
        .bSubClass = "01",
        .bProtocol = ".."
    };
    LKDDB_USB_CLASS_ID iProtocol = {
        .bClass    = iClass.bClass,
        .bSubClass = iSubClass.bSubClass,
        .bProtocol = "01"
    };
    length = ids->lists[LHQ_ID_USB_CLASS]->length;
    index = lhq_usb_class_id_search_and_copy(ids, &iClass, 0);
    if( index == length ) {
        printf("Class Not Found.\n");
    } else {
        printf("Found Class:\n");
        lhq_usb_class_id_entry_print(&iClass, stdout);
        index = lhq_usb_class_id_search_and_copy(ids, &iSubClass, index+1);
        if( index == length ) {
            printf("SubClass Not Found.\n");
        } else {
            printf("Found SubClass:\n");
            lhq_usb_class_id_entry_print(&iSubClass, stdout);
            index = lhq_usb_class_id_search_and_copy(ids, &iProtocol, index+1);
            if( index == length ) {
                printf("Protocol Not Found.\n");
            } else {
                printf("Found Protocol:\n");
                lhq_usb_class_id_entry_print(&iProtocol, stdout);
            }
        }
    }
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
    LHQ_TYPES_INDEX *typesIndex = lhq_build_types_index(lkddb);
    LHQ_IDS_INDEX *idsIndex = lhq_build_ids_index(ids);
    fclose(lkddb);
    fclose(ids);

    lhq_search_usb(idsIndex, typesIndex);

    lhq_types_index_free(typesIndex);
    lhq_ids_index_free(idsIndex);
    return 0;
}
