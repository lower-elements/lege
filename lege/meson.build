cfg = configuration_data()

version = meson.project_version().split('.')
cfg.set('LEGE_VERSION_MAJOR', version[0])
cfg.set('LEGE_VERSION_MINOR', version[1])
cfg.set('LEGE_VERSION_PATCH', version[2])
cfg.set_quoted('LEGE_VERSION_STRING', meson.project_version())

cfg_h = configure_file(input: 'lege.h.in', output: 'lege.h', configuration: cfg)

lege_cflags = ['-DBUILDING_LIBLEGE']
lege_deps = [
  dependency('luajit', fallback: ['luajit', 'lua_dep'])
  ]
lege_srcs = [
  cfg_h,
  'engine.c',
  'util.c',
  ]
lege_priv_incdir = include_directories('include')
lege_pub_incdir = include_directories('.')

lege_lib = library('lege',
  sources: lege_srcs,
  c_args: lege_cflags,
  include_directories: [lege_priv_incdir, lege_pub_incdir],
  dependencies: lege_deps,
  soversion: abi_version,
  gnu_symbol_visibility: 'hidden',
  install: true
  )

lege_dep = declare_dependency(link_with: lege_lib, include_directories: lege_pub_incdir)
