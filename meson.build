project(
    'linux-hardware-qualifier',
    'c',
    version: '0.0.1',
    license: [
        'Apache-2.0'
    ],
    default_options: [
        'c_std=c11',
        'prefix=/usr',
        'sysconfdir=/etc',
        'localstatedir=/var',
    ],    
)

am_cflags = [
    '-fstack-protector',
    '-Wall',
    '-pedantic',
    '-Wstrict-prototypes',
    '-Wundef',
    '-fno-common',
    '-Werror-implicit-function-declaration',
    '-Wformat',
    '-Wformat-security',
    '-Werror=format-security',
    '-Wconversion',
    '-Wunused-variable',
    '-Wunreachable-code',
    '-W',
    '-D_DEFAULT_SOURCE'
]

# Add our main flags
add_global_arguments(am_cflags, language: 'c')
add_global_arguments('-Db_sanitize=address', language : 'c')

# Get configuration bits together
path_prefix = get_option('prefix')
path_datadir = join_paths(path_prefix, get_option('datadir'))
path_sysconfdir = join_paths(path_prefix, get_option('sysconfdir'))
path_mandir = join_paths(path_prefix, get_option('mandir'))

# Build the project
incdir = include_directories('include')
subdir('src')
subdir('test')

report = [
    '    Build configuration:',
    '    ====================',
    '',
    '    prefix:                                 @0@'.format(path_prefix),
    '    datadir:                                @0@'.format(path_datadir),
    '    sysconfdir:                             @0@'.format(path_sysconfdir),
    '    mandir:                                 @0@'.format(path_mandir),
]

# Output some stuff to validate the build config
message('\n\n\n' + '\n'.join(report) + '\n\n')
