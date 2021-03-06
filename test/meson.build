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

test_c_args = '-Wno-unused-function'

e1 = executable('test-load', 'load.c', link_with: lib_lhq_static, include_directories: incdir, c_args: test_c_args)
test('Load LKDDB', e1, workdir : meson.source_root())
benchmark('Bench Load LKDDB', e1, args: '100', workdir : meson.source_root())

e2 = executable('test-search-acpi', 'search-acpi.c', link_with: lib_lhq_static, include_directories: incdir, c_args: test_c_args)
test('Search LKDDB for ACPI Device', e2, workdir : meson.source_root())
benchmark('Bench Search LKDDB ACPI', e2, args: '1000', workdir : meson.source_root())

e3 = executable('test-search-pci', 'search-pci.c', link_with: lib_lhq_static, include_directories: incdir, c_args: test_c_args)
test('Search LKDDB for PCI Device', e3, workdir : meson.source_root())
benchmark('Bench Search LKDDB PCI', e3, args: '1000', workdir : meson.source_root())

e4 = executable('test-search-usb', 'search-usb.c', link_with: lib_lhq_static, include_directories: incdir, c_args: test_c_args)
test('Search LKDDB for USB Device', e4, workdir : meson.source_root())
benchmark('Bench Search LKDDB USB', e4, args: '1000', workdir : meson.source_root())

e5 = executable('test-search-acpi-all', 'search-acpi-all.c', link_with: lib_lhq_static, include_directories: incdir, c_args: test_c_args)
test('Search LKDDB for All ACPI Devices', e5, workdir : meson.source_root())
benchmark('Bench Search LKDDB ACPI ALL', e5, args: '1000', workdir : meson.source_root())

e6 = executable('test-search-pci-all', 'search-pci-all.c', link_with: lib_lhq_static, include_directories: incdir, c_args: test_c_args)
test('Search LKDDB for All PCI Devices', e6, workdir : meson.source_root())
benchmark('Bench Search LKDDB PCI ALL', e6, args: '100', workdir : meson.source_root())

e7 = executable('test-search-usb-all', 'search-usb-all.c', link_with: lib_lhq_static, include_directories: incdir, c_args: test_c_args)
test('Search LKDDB for All USB Devices', e7, workdir : meson.source_root())
benchmark('Bench Search LKDDB USB ALL', e7, args: '100', workdir : meson.source_root())

e8 = executable('test-search-all', 'search-all.c', link_with: lib_lhq_static, include_directories: incdir, c_args: test_c_args)
test('Search LKDDB for All Devices', e8, workdir : meson.source_root())
benchmark('Bench Search LKDDB ALL', e8, args: '100', workdir : meson.source_root())
