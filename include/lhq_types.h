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

#ifndef __LINUX_HARDWARE_QUALIFIER_TYPES_H__
#define __LINUX_HARDWARE_QUALIFIER_TYPES_H__

enum {
    LHQ_TYPE_ACPI,
    LHQ_TYPE_PCI,
    LHQ_TYPE_USB,
    LHQ_TYPE_COUNT
};

enum {
    LHQ_ID_PCI_CLASS,
    LHQ_ID_PCI,
    LHQ_ID_USB_CLASS,
    LHQ_ID_USB,
    LHQ_ID_COUNT
};

#endif
