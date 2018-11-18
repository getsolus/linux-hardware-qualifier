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
#include "usb.h"

int main() {
    FILE *lkddb = fopen("data/lkddb.list", "r");
    if(!lkddb) {
        fprintf(stderr, "Failed to open 'data/lkddb.list'. Exiting.\n");
    }
    LKDDB_USB_ENTRY *entry;
    while(!feof(lkddb) ){
        entry = lhq_usb_entry_new();
        if( lhq_usb_entry_parse(entry, lkddb) ){
            lhq_usb_entry_print(entry, stderr);
        } else {
            while(!feof(lkddb) && getc(lkddb) != '\n');
        }
        lhq_usb_entry_free(entry);
    }
    //lhq_usb_entry_free(entry);
    fclose(lkddb);
    return 0;
}
