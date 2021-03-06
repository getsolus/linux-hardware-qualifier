# linux-hardware-qualifier
Checks that your hardware is supported by the kernel and enabled in its config

## LKDDB Types Supported

These represent the bulk of the devices that most people will have attached to
their systems. Therefore, the majority of my focus is directed here.

- [x] acpi
  - [x] lkddb list
  - [x] sysfs
  - [x] search
- [ ] ieee1394
  - [ ] lkddb list
  - [ ] sysfs
  - [ ] search
- [ ] hid
  - [ ] lkddb list
  - [ ] sysfs
  - [ ] search
- [x] pci
  - [x] lkddb ids
  - [x] lkddb class ids
  - [x] lkddb list
  - [x] sysfs
  - [x] search
- [x] usb
  - [x] lkddb ids
  - [x] lkddb class ids
  - [x] lkddb list
  - [x] sysfs
  - [ ] search

## LKDDB Types Unsupported

Some of these entries may be supported in the future, but are currently not planned.
Others, like "platform" devices cannot be detected and have to manually added via
DeviceTree and are therefore also ignored.

- ap
- ccw
- eisa
- fs
- i2c-snd
- kver
- module
- of
- parisc
- pcmcia
- platform
- pnp
- sdio
- serio
- ssb
- tc
- vio
- virtio
- zorro

## Dependencies

- clang
- meson

## Build & Install

```
make
make install
```

## License 
Copyright 2018-2019 Solus Project <copyright@getsol.us>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
