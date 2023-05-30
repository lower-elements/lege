util_srcs = [
  'util.c',
]
util_incdir = include_directories('include')
util_deps = [
  hedley_dep,
  lua_dep,
  sdl_dep,
  ]

util_lib = static_library('lege-util',
  sources: util_srcs,
  include_directories: util_incdir,
  dependencies: util_deps,
)

util_dep = declare_dependency(link_with: util_lib, include_directories: util_incdir, dependencies: util_deps)
