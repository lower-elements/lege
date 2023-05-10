cli_sources = [
  'args.c',
  'cmd_run.c',
  'error_reporting.c',
  'main.c',
  'optparse.c',
]
cli_incdir = include_directories('include')
cli_deps = [
  lege_dep,
  util_dep,
  dependency('luajit', fallback: ['luajit', 'lua_dep']),
  ]

cli = executable('lege',
  sources: cli_sources,
  include_directories: cli_incdir,
  dependencies: cli_deps,
  install: true
)
