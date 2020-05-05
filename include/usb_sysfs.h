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

/* Find common parts of a USB result

   @param results - list of results to save to
   @param device  - directory entry for the the current USB device
*/
int lhq_usb_find_common(LHQ_LIST *results, struct dirent *device);

/* Find a USB device with an empty interface

   @param results - list of results to save to
   @param device  - directory entry for the the current USB device
*/
void lhq_usb_find_nointerface(LHQ_LIST *results, struct dirent *device);

/* Find a USB device with an interface

   @param results    - list of results to save to
   @param device     - directory entry for the the current USB device
   @param interface  - directory entry for the the current USB interface
*/
void lhq_usb_find_interface(LHQ_LIST *results, struct dirent *device, struct dirent *interface);

int lhq_usb_find_device(LHQ_LIST *results, struct dirent *device);

/*
int lhq_usb_sysfs_parse_interface_id(LKDDB_USB_CLASS_ID *id, char *path) {
    int status = lhq_file_to_string(path, "bInterfaceClass", id->class.bClass);
    if( status != 0) return status;
    status = lhq_file_to_string(path, "bInterfaceSubClass", id->class.bSubClass);
    if( status != 0) return status;
    return lhq_file_to_string(path, "bInterfaceProtocol", id->class.bProtocol);
}
*/

int lhq_usb_find_devices(LHQ_LIST *results);

#endif
