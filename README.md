# linux-hardware-qualifier
Checks that your hardware is supported by the kernel and enabled in its config

## LKDDB Types Supported

These represent the bulk of the devices that most people will have attached to
their systems. Therefore, the majority of my focus is directed here.

- [ ] acpi
  - [ ] lkddb ids
  - [x] lkddb list
  - [ ] sysfs
- [ ] ieee1394
  - [ ] lkddb ids
  - [ ] lkddb list
  - [ ] sysfs
- [ ] hid
  - [ ] lkddb ids
  - [ ] lkddb list
  - [ ] sysfs
- [x] pci
  - [ ] lkddb ids
  - [x] lkddb list
  - [ ] sysfs
- [x] usb
  - [ ] lkddb ids
  - [x] lkddb list
  - [ ] sysfs

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
