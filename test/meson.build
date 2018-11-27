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
cdata.set('LHQ_DEBUG', '1')

config_h = configure_file(
     configuration: cdata,
     output: 'config.h',
)

load_sources = [
    'load.c',
]

e = executable('test-load', sources: load_sources, include_directories: incdir)
test('Load LKDDB', e, workdir : meson.source_root())
benchmark('Bench Load LKDDB', e, args: '100', workdir : meson.source_root())