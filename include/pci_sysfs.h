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

#ifndef __LINUX_HARDWARE_QUALIFIER_PCI_SYSFS_H__
#define __LINUX_HARDWARE_QUALIFIER_PCI_SYSFS_H__

#include "lhq_list.h"
#include "pci_result.h"
#include "util.h"

#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void lhq_pci_find_device(LHQ_LIST *results, struct dirent *entry) {
    LHQ_PCI_RESULT *result = (LHQ_PCI_RESULT *)lhq_list_next(results);
    static char buff[128];
    static char path[128];
    sprintf(buff, "/sys/bus/pci/devices/%s", entry->d_name);
    realpath(buff, path);
    int status = lhq_file_to_string(path, "/device", &(result->entry.id.device), 2, 4);
    if(status <= 0) goto CLEANUP;
    status = lhq_file_to_string(path, "/vendor", &(result->entry.id.vendor), 2, 4);
    if(status <= 0) goto CLEANUP;
    status = lhq_file_to_string(path, "/subsystem_device", &(result->entry.id.subDevice), 2, 4);
    if(status <= 0) goto CLEANUP;
    status = lhq_file_to_string(path, "/subsystem_vendor", &(result->entry.id.subVendor), 2, 4);
    if(status <= 0) goto CLEANUP;
    status = lhq_file_to_string(path, "/class", &(result->entry.class.classMask), 2, 6);
    if(status <= 0) goto CLEANUP;
    lhq_list_inc(results);
    return;
CLEANUP:
    if(status < 0) lhq_pci_result_free(result);
}

int lhq_pci_find_devices(LHQ_LIST *results) {
    DIR *devices = opendir("/sys/bus/pci/devices");
    if(devices == NULL) {
        return -1;
    }
    struct dirent *device = readdir(devices);
    while(device != NULL) {
        if(device->d_type == DT_LNK) {
            lhq_pci_find_device(results, device);
        }
        device = readdir(devices);
    }
    return closedir(devices);
}

#endif
