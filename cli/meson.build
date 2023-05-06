cli_sources = [
  'main.c',
]
cli_deps = [
  lege_dep,
  dependency('c-flags', fallback: ['c-flags', 'libcflags_dep']),
  ]

cli = executable('lege',
  sources: cli_sources,
  dependencies: cli_deps,
  install: true
)
