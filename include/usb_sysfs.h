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

#ifndef __LINUX_HARDWARE_QUALIFIER_USB_SYSFS_H__
#define __LINUX_HARDWARE_QUALIFIER_USB_SYSFS_H__

#include "lhq_list.h"

#include "usb_class_ids.h"
#include "usb_ids.h"

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

void lhq_usb_find_nointerface(LHQ_LIST *results, struct dirent *device) {
    LHQ_USB_RESULT *result = (LHQ_USB_RESULT *)lhq_list_next(results);
    char path[128];
    char buff[128];
    // interface stuff
    result->entry.interfaceClass.bClass = (char*)calloc(1,3);
    strcpy(result->entry.interfaceClass.bClass, "..");
    result->entry.interfaceClass.bSubClass = (char*)calloc(1,3);
    strcpy(result->entry.interfaceClass.bSubClass, "..");
    result->entry.interfaceClass.bProtocol = (char*)calloc(1,3);
    strcpy(result->entry.interfaceClass.bProtocol, "..");
    // device stuff
    sprintf(buff, "/sys/bus/usb/devices/%s", device->d_name);
    realpath(buff, path);
    strcpy(buff, path);
    strcat(buff, "/idProduct");
    FILE *f = fopen(buff, "r");
    if(f != NULL) {
        lhq_file_to_string(f, &(result->entry.id.product), 0, 4);
        fclose(f);
    } else {
        lhq_usb_result_free(result);
        return;
    }
    strcpy(buff, path);
    strcat(buff, "/idVendor");
    f = fopen(buff, "r");
    if(f != NULL) {
        lhq_file_to_string(f, &(result->entry.id.vendor), 0, 4);
        fclose(f);
    } else {
        lhq_usb_result_free(result);
        return;
    }
    strcpy(buff, path);
    strcat(buff, "/bDeviceClass");
    f = fopen(buff, "r");
    if(f != NULL) {
        lhq_file_to_string(f, &(result->entry.class.bClass), 0, 2);
        fclose(f);
    } else {
        lhq_usb_result_free(result);
        return;
    }
    strcpy(buff, path);
    strcat(buff, "/bDeviceSubClass");
    f = fopen(buff, "r");
    if(f != NULL) {
        lhq_file_to_string(f, &(result->entry.class.bSubClass), 0, 2);
        fclose(f);
    } else {
        lhq_usb_result_free(result);
        return;
    }
    strcpy(buff, path);
    strcat(buff, "/bDeviceProtocol");
    f = fopen(buff, "r");
    if(f != NULL) {
        lhq_file_to_string(f, &(result->entry.class.bProtocol), 0, 2);
        fclose(f);
    } else {
        lhq_usb_result_free(result);
        return;
    }
    lhq_list_inc(results);
}

void lhq_usb_find_interface(LHQ_LIST *results, struct dirent *device, struct dirent *interface) {
    LHQ_USB_RESULT *result = (LHQ_USB_RESULT *)lhq_list_next(results);
    char path[128];
    char buff[128];
    // interface stuff
    sprintf(path, "/sys/bus/usb/devices/%s/%s", device->d_name, interface->d_name);
    strcpy(buff, path);
    strcat(buff, "/bInterfaceClass");
    FILE *f = fopen(buff, "r");
    if(f != NULL) {
        lhq_file_to_string(f, &(result->entry.interfaceClass.bClass), 0, 2);
        fclose(f);
    } else {
        return;
    }
    strcpy(buff, path);
    strcat(buff, "/bInterfaceSubClass");
    f = fopen(buff, "r");
    if(f != NULL) {
        lhq_file_to_string(f, &(result->entry.interfaceClass.bSubClass), 0, 2);
        fclose(f);
    } else {
        lhq_usb_result_free(result);
        return;
    }

    strcpy(buff, path);
    strcat(buff, "/bInterfaceProtocol");
    f = fopen(buff, "r");
    if(f != NULL) {
        lhq_file_to_string(f, &(result->entry.interfaceClass.bProtocol), 0, 2);
        fclose(f);
    } else {
        lhq_usb_result_free(result);
        return;
    }
    // device stuff
    sprintf(buff, "/sys/bus/usb/devices/%s", device->d_name);
    realpath(buff, path);
    strcpy(buff, path);
    strcat(buff, "/idProduct");
    f = fopen(buff, "r");
    if(f != NULL) {
        lhq_file_to_string(f, &(result->entry.id.product), 0, 4);
        fclose(f);
    } else {
        lhq_usb_result_free(result);
        return;
    }
    strcpy(buff, path);
    strcat(buff, "/idVendor");
    f = fopen(buff, "r");
    if(f != NULL) {
        lhq_file_to_string(f, &(result->entry.id.vendor), 0, 4);
        fclose(f);
    } else {
        lhq_usb_result_free(result);
        return;
    }
    strcpy(buff, path);
    strcat(buff, "/bDeviceClass");
    f = fopen(buff, "r");
    if(f != NULL) {
        lhq_file_to_string(f, &(result->entry.class.bClass), 0, 2);
        fclose(f);
    } else {
        lhq_usb_result_free(result);
        return;
    }
    strcpy(buff, path);
    strcat(buff, "/bDeviceSubClass");
    f = fopen(buff, "r");
    if(f != NULL) {
        lhq_file_to_string(f, &(result->entry.class.bSubClass), 0, 2);
        fclose(f);
    } else {
        lhq_usb_result_free(result);
        return;
    }
    strcpy(buff, path);
    strcat(buff, "/bDeviceProtocol");
    f = fopen(buff, "r");
    if(f != NULL) {
        lhq_file_to_string(f, &(result->entry.class.bProtocol), 0, 2);
        fclose(f);
    } else {
        lhq_usb_result_free(result);
        return;
    }
    lhq_list_inc(results);
}

int lhq_usb_find_device(LHQ_LIST *results, struct dirent *device) {
    lhq_usb_find_nointerface(results, device);
    char buff[256];
    strcpy(buff, "/sys/bus/usb/devices/");
    strcat(buff, device->d_name);
    DIR *interfaces = opendir(buff);
    if(interfaces == NULL) {
        return -1;
    }
    struct dirent *interface = readdir(interfaces);
    while(interface != NULL) {
        if(interface->d_type == DT_DIR) {
            lhq_usb_find_interface(results, device, interface);
        }
        interface = readdir(interfaces);
    }
    return closedir(interfaces);
}

/*
int lhq_usb_sysfs_parse_interface_id(LKDDB_USB_CLASS_ID *id, char *path) {
    int status = lhq_file_to_string(path, "bInterfaceClass", id->class.bClass);
    if( status != 0) return status;
    status = lhq_file_to_string(path, "bInterfaceSubClass", id->class.bSubClass);
    if( status != 0) return status;
    return lhq_file_to_string(path, "bInterfaceProtocol", id->class.bProtocol);
}
*/

int lhq_usb_find_devices(LHQ_LIST *results) {
    DIR *devices = opendir("/sys/bus/usb/devices");
    if(devices == NULL) {
        return -1;
    }
    struct dirent *device = readdir(devices);
    while(device != NULL) {
        if((device->d_type == DT_LNK) && (strncmp(device->d_name, "usb", 3) != 0)) {
            lhq_usb_find_device(results, device);
        }
        device = readdir(devices);
    }
    return closedir(devices);
}

#endif
