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

#include "acpi_result.h"
#include "ids_index.h"

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

void lhq_search_acpi(LHQ_TYPES_INDEX *types) {
    LHQ_ACPI_RESULT result = {
        .entry = {
            .id = "80860F28",
        },
    };
    lhq_acpi_result_search(&result, types);
    lhq_acpi_result_entry_print(&result, stdout);
}

int main(int argc, char **argv) {
    int time = 1;
    if(argc > 1) {
        time = atoi(argv[1]);
        if(time == 0) {
            time = 1;
        }
    }
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

    for(; time > 0; time--) {
        lhq_search_acpi(typesIndex);
    }

    lhq_types_index_free(typesIndex);
    lhq_ids_index_free(idsIndex);
    return 0;
}
