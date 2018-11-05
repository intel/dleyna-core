connectordir = join_paths(get_option('prefix'), get_option('libdir'), 'dleyna-1.0/connectors')
sysconfdir = join_paths(get_option('prefix'), get_option('sysconfdir'))

install_headers(
    files(
        'connector.h',
        'connector-mgr.h',
        'control-point.h',
        'core.h',
        'error.h',
        'log.h',
        'main-loop.h',
        'service-task.h',
        'settings.h',
        'task-atom.h',
        'task-processor.h',
        'white-list.h',
    ),
    subdir : 'dleyna-1.0/libdleyna/core'
)

libdleyna_core = library(
    'dleyna-core-1.0',
    files(
        'connector-mgr.c',
        'core.c',
        'error.c',
        'log.c',
        'main-loop.c',
        'service-task.c',
        'settings.c',
        'task-processor.c',
        'white-list.c'
    ),
    version : '5.0.0',
    dependencies : [
        dependency('glib-2.0', version : '>= 2.28'),
        dependency('gio-2.0', version : '>= 2.28'),
        dependency('gmodule-2.0', version : '>= 2.28'),
        dependency('gupnp-1.2', version : '>= 1.2.0'),
        config_h
    ],
    c_args : [
        '-DCONNECTOR_DIR="@0@"'.format(connectordir),
        '-DSYS_CONFIG_DIR="@0@"'.format(sysconfdir),
        '-Wno-format-extra-args'
    ],
    install : true
)

pkg.generate(
    libdleyna_core,
    description : 'UPnP & DLNA core library',
    subdirs : 'dleyna-1.0',
    version: meson.project_version(),
)
