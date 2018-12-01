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

#include "config.h"
#include "ids_index.h"
#include "types_index.h"
#include "usb_result.h"
#include <stdio.h>

LHQ_TYPES_INDEX *lhq_build_types_index(FILE *lkddb) {
    LHQ_TYPES_INDEX *index = lhq_types_index_new(lkddb);
    lhq_types_index_populate(index);
#ifdef LHQ_DEBUG
#if LHQ_DEBUG > 0
    lhq_types_index_summary(index);
#endif
#endif
    return index;
}

LHQ_IDS_INDEX *lhq_build_ids_index(FILE *ids) {
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
    LHQ_USB_RESULT result = {
        .entry = {
            .id = {
                .vendor = "8087",
                .product = "0a2a",
            },
            .class = {
                .bClass = "e0",
                .bSubClass = "01",
                .bProtocol = "01",
            },
            .interfaceClass = {
                .bClass = "e0",
                .bSubClass = "01",
                .bProtocol = "01",
            },
        },
    };
    lhq_usb_result_search(&result, ids, types);
}

int main() {
    FILE *lkddb = fopen(DATADIR "/" PACKAGE_NAME "/lkddb.list", "r");
    if(lkddb == NULL) {
        fprintf(stderr, "Failed to open '%s'. Exiting.\n", DATADIR "/" PACKAGE_NAME "/lkddb.list");
        return -1;
    }
    FILE *ids = fopen("data/ids.list", "r");
    if(ids == NULL) {
        fprintf(stderr, "Failed to open '%s'. Exiting.\n", "data/ids.list");
        return -1;
    }
    LHQ_TYPES_INDEX *typesIndex = lhq_build_types_index(lkddb);
    LHQ_IDS_INDEX *idsIndex     = lhq_build_ids_index(ids);
    fclose(lkddb);
    fclose(ids);

    lhq_search_usb(idsIndex, typesIndex);

    lhq_types_index_free(typesIndex);
    lhq_ids_index_free(idsIndex);
    return 0;
}
