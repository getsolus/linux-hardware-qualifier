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

executable(
    'linux-hardware-qualifier',
    'main.c',
    include_directories: incdir,
    dependencies: depends,
    install: true
)
