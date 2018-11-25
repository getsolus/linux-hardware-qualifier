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

config_h = configure_file(
     configuration: cdata,
     output: 'config.h',
)

sources = [
    'main.c',
    'lhq_string.h',
    'lhq_types.h',
    'acpi.h',
    'pci.h',
    'pci_class_ids.h',
    'pci_ids.h',
    'usb.h',
    'usb_class_ids.h',
    'usb_ids.h'
]

depends = []

executable(
    'linux-hardware-qualifier',
    sources,
    dependencies: depends,
    install: true
)
