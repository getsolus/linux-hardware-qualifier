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
#include "usb.h"
#include "usb_class_ids.h"
#include "usb_ids.h"

void lhq_acpi(FILE * lkddb) {
    rewind(lkddb);
    LKDDB_ACPI_ENTRY entry;
    LKDDB_LIST *list = lhq_acpi_list_new();
    while(!feof(lkddb) ){
        if( lhq_acpi_entry_parse(&entry, lkddb) ){
            lhq_acpi_list_append(list, &entry);
        } else {
            while(!feof(lkddb) && getc(lkddb) != '\n');
        }
    }
    fprintf(stderr, "Length: %d, Capacity: %d\n", list->length, list->capacity);
    //lhq_acpi_list_print(list,stderr);
    lhq_acpi_list_free(list);
}

void lhq_pci(FILE * lkddb) {
    rewind(lkddb);
    LKDDB_PCI_ENTRY entry;
    LKDDB_LIST *list = lhq_pci_list_new();
    while(!feof(lkddb) ){
        if( lhq_pci_entry_parse(&entry, lkddb) ){
            lhq_pci_list_append(list, &entry);
        } else {
            while(!feof(lkddb) && getc(lkddb) != '\n');
        }
    }
    fprintf(stderr, "Length: %d, Capacity: %d\n", list->length, list->capacity);
    //lhq_pci_list_print(list,stderr);
    lhq_pci_list_free(list);
}

void lhq_usb(FILE * lkddb) {
    rewind(lkddb);
    LKDDB_USB_ENTRY entry;
    LKDDB_LIST *list = lhq_usb_list_new();
    while(!feof(lkddb) ){
        if( lhq_usb_entry_parse(&entry, lkddb) ){
            lhq_usb_list_append(list, &entry);
        } else {
            while(!feof(lkddb) && getc(lkddb) != '\n');
        }
    }
    fprintf(stderr, "Length: %d, Capacity: %d\n", list->length, list->capacity);
    //lhq_usb_list_print(list,stderr);
    lhq_usb_list_free(list);
}

void lhq_usb_class_ids(FILE * lkddb_ids) {
    rewind(lkddb_ids);
    LKDDB_USB_CLASS_ID entry;
    LKDDB_LIST *list = lhq_usb_class_id_list_new();
    while(!feof(lkddb_ids) ){
        if( lhq_usb_class_id_entry_parse(&entry, lkddb_ids) ){
            lhq_usb_class_id_list_append(list, &entry);
        } else {
            while(!feof(lkddb_ids) && getc(lkddb_ids) != '\n');
        }
    }
    fprintf(stderr, "Length: %d, Capacity: %d\n", list->length, list->capacity);
    //lhq_usb_class_id_list_print(list,stderr);
    lhq_usb_class_id_list_free(list);
}

void lhq_usb_ids(FILE * lkddb_ids) {
    rewind(lkddb_ids);
    LKDDB_USB_ID entry;
    LKDDB_LIST *list = lhq_usb_id_list_new();
    while(!feof(lkddb_ids) ){
        if( lhq_usb_id_entry_parse(&entry, lkddb_ids) ){
            lhq_usb_id_list_append(list, &entry);
        } else {
            while(!feof(lkddb_ids) && getc(lkddb_ids) != '\n');
        }
    }
    fprintf(stderr, "Length: %d, Capacity: %d\n", list->length, list->capacity);
    //lhq_usb_id_list_print(list,stderr);
    lhq_usb_id_list_free(list);
}

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

    lhq_usb_class_ids(lkddb_ids);
    lhq_usb_ids(lkddb_ids);
    fclose(lkddb);
    fclose(lkddb_ids);
    return 0;
}
