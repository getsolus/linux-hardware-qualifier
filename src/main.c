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

#include "config.h"

#include "acpi_result.h"
#include "acpi_sysfs.h"
#include "pci_result.h"
#include "pci_sysfs.h"
#include "ids_index.h"

#include <stdio.h>

LHQ_TYPES_INDEX *lhq_build_types_index(FILE *lkddb) {
    LHQ_TYPES_INDEX *index = lhq_types_index_new(lkddb);
    lhq_types_index_populate(index);
#if LHQ_DEBUG > 0
    lhq_types_index_summary(index);
#endif
    return index;
}

LHQ_IDS_INDEX *lhq_build_ids_index(FILE *ids) {
    LHQ_IDS_INDEX *idsIndex = lhq_ids_index_new(ids);

    lhq_ids_index_populate(idsIndex);
#if LHQ_DEBUG > 0
    lhq_ids_index_summary(idsIndex);
#endif
    return idsIndex;
}

void lhq_search_acpi(LHQ_TYPES_INDEX *types) {
    LHQ_LIST *results = lhq_acpi_result_list_new();
    int status = lhq_acpi_find_devices(results);
    if( status != 0 ) {
        goto CLEANUP;
    }
    LHQ_ACPI_RESULT *result = (LHQ_ACPI_RESULT*)results->data;
    for( unsigned int i = 0; i < results->length; i++ ) {
        lhq_acpi_result_search(result, types);
        lhq_acpi_result_entry_print(result, stdout);
        result++;
    }
CLEANUP:
    lhq_list_free(results);
}

void lhq_search_pci(LHQ_IDS_INDEX *ids, LHQ_TYPES_INDEX *types) {
    LHQ_LIST *results = lhq_pci_result_list_new();
    int status = lhq_pci_find_devices(results);
    if( status != 0 ) {
        goto CLEANUP;
    }
    LHQ_PCI_RESULT *result = (LHQ_PCI_RESULT*)results->data;
    for( unsigned int i = 0; i < results->length; i++ ) {
        lhq_pci_result_search(result, ids, types);
        lhq_pci_result_entry_print(result, stdout);
        lhq_pci_result_free(result);
        result++;
    }
CLEANUP:
    lhq_list_free(results);
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
    FILE *ids = fopen(DATADIR "/" PACKAGE_NAME "/ids.list", "r");
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
        lhq_search_pci(idsIndex,typesIndex);
    }

    lhq_types_index_free(typesIndex);
    lhq_ids_index_free(idsIndex);
    return 0;
}
