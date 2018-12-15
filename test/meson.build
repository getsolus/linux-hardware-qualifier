# Get configuration bits together
path_prefix = get_option('prefix')
path_datadir = join_paths(path_prefix, get_option('datadir'))
path_sysconfdir = join_paths(path_prefix, get_option('sysconfdir'))
path_mandir = join_paths(path_prefix, get_option('mandir'))

# build config.h
cdata = configuration_data()
cdata.set_quoted('PACKAGE_VERSION', meson.project_version())
cdata.set_quoted('PACKAGE_NAME', '')
cdata.set_quoted('DATADIR', 'data')
cdata.set_quoted('SYSCONFDIR', path_sysconfdir)
cdata.set('LHQ_DEBUG', '0')

config_h = configure_file(
     configuration: cdata,
     output: 'config.h',
)

e1 = executable('test-load', 'load.c', include_directories: incdir)
test('Load LKDDB', e1, workdir : meson.source_root())
benchmark('Bench Load LKDDB', e1, args: '100', workdir : meson.source_root())

e2 = executable('test-search-acpi', 'search-acpi.c', include_directories: incdir)
test('Search LKDDB for ACPI Device', e2, workdir : meson.source_root())
benchmark('Bench Search LKDDB ACPI', e2, args: '1000', workdir : meson.source_root())

e3 = executable('test-search-pci', 'search-pci.c', include_directories: incdir)
test('Search LKDDB for PCI Device', e3, workdir : meson.source_root())
benchmark('Bench Search LKDDB PCI', e3, args: '1000', workdir : meson.source_root())

e4 = executable('test-search-usb', 'search-usb.c', include_directories: incdir)
test('Search LKDDB for USB Device', e4, workdir : meson.source_root())
benchmark('Bench Search LKDDB USB', e4, args: '1000', workdir : meson.source_root())

e5 = executable('test-search-acpi-all', 'search-acpi-all.c', include_directories: incdir)
test('Search LKDDB for All ACPI Devices', e5, workdir : meson.source_root())
benchmark('Bench Search LKDDB ACPI ALL', e5, args: '1000', workdir : meson.source_root())
