# Get configuration bits together
path_prefix = get_option('prefix')
path_datadir = join_paths(path_prefix, get_option('datadir'))
path_sysconfdir = join_paths(path_prefix, get_option('sysconfdir'))
path_mandir = join_paths(path_prefix, get_option('mandir'))

# build config.h
cdata = configuration_data()
cdata.set_quoted('PACKAGE_VERSION', meson.project_version())
cdata.set_quoted('PACKAGE_NAME', meson.project_name())
cdata.set_quoted('DATADIR', path_datadir)
cdata.set_quoted('SYSCONFDIR', path_sysconfdir)
cdata.set('LHQ_DEBUG', '0')

config_h = configure_file(
     configuration: cdata,
     output: 'config.h',
)

depends = []

lib_lhq_static = static_library(
    'lhq',
    sources: [
        'acpi.c',
        'acpi_result.c',
        'acpi_sysfs.c',
        'ids_index.c',
        'kernel_config.c',
        'kernel_flag.c',
        'lhq_list.c',
        'pci.c',
        'pci_class_ids.c',
        'pci_ids.c',
        'pci_result.c',
        'pci_sysfs.c',
        'types_index.c',
        'usb.c',
        'usb_class_ids.c',
        'usb_ids.c',
        'usb_result.c',
        'usb_sysfs.c',
        'util.c',
    ],
    include_directories: incdir
)

executable(
    'linux-hardware-qualifier',
    'main.c',
    link_with: lib_lhq_static,
    link_flags: '-Wl,--gc-sections,--print-gc-sections',
    include_directories: incdir,
    dependencies: depends,
    install: true
)
