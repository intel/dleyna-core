project('dleyna-core', 'c', version: '0.6.0')

pkg = import('pkgconfig')

conf = configuration_data()

log_levels = [
    0x00,
    0x01,
    0x02,
    0x04,
    0x08,
    0x10,
    0x20,
    0x13,
    0x3F
]

conf.set_quoted('VERSION', meson.project_version())

conf.set('DLEYNA_LOG_LEVEL_DISABLED', log_levels[0], description: 'Log level flag for disabled messages')
conf.set('DLEYNA_LOG_LEVEL_ERROR',    log_levels[1], description: 'Log level flag for errors')
conf.set('DLEYNA_LOG_LEVEL_CRITICAL', log_levels[2], description: 'Log level flag for critical messages')
conf.set('DLEYNA_LOG_LEVEL_WARNING',  log_levels[3], description: 'Log level flag for warnings')
conf.set('DLEYNA_LOG_LEVEL_MESSAGE',  log_levels[4], description: 'Log level flag for messages')
conf.set('DLEYNA_LOG_LEVEL_INFO',     log_levels[5], description: 'Log level flag for informational messages')
conf.set('DLEYNA_LOG_LEVEL_DEBUG',    log_levels[6], description: 'Log level flag for debug messages')
conf.set('DLEYNA_LOG_LEVEL_DEFAULT',  log_levels[7], description: 'Log level flag to display default level messages')
conf.set('DLEYNA_LOG_LEVEL_ALL',      log_levels[8], description: 'Log level flag for all messages')

conf.set('DLEYNA_NEVER_QUIT', 'FALSE', description : 'Default service behavior when last client disconnects')
conf.set_quoted('DLEYNA_CONNECTOR_NAME', 'dbus', description : 'IPC connector name')
conf.set_quoted('DLEYNA_CONNECTOR_LIB_PATTERN', 'libdleyna-connector-', description : 'Starting pattern for dleyna connector libraries')
conf.set('DLEYNA_LOG_TYPE', '0', description: 'Define log output technology')

exclusive = get_option('log_level').contains('0') or get_option('log_level').contains('7') or get_option('log_level').contains('8')

if exclusive and get_option('log_level').length() > 1
    error('Log levels 0, 7 and 8 cannot be used with other log levels')
endif

log_level = 0
foreach value : get_option('log_level')
    log_level += log_levels[value.to_int()]
endforeach

conf.set('DLEYNA_LOG_LEVEL', log_level)

config_h_file = configure_file(output: 'config.h', configuration: conf)
config_h = declare_dependency(
    include_directories : include_directories('.'),
    compile_args : ['-include', 'config.h'],
    sources : config_h_file
)

subdir('libdleyna')
