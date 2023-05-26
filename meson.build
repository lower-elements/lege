project('lege', 'c',
  version: '0.1.0',
  license: 'lgpl-3', # Not lgpl-3-or-later
  default_options: ['warning_level=3', 'c_std=c17'],
)

abi_version = 0

hedley_dep = dependency('hedley', fallback: ['hedley', 'hedley_dep'], required: false)
  lua_dep = dependency('luajit', fallback: ['luajit', 'lua_dep'])

subdir('util')
subdir('lege')

if get_option('build_cli')
  subdir('cli')
endif

if get_option('build_examples')
  subdir('examples')
endif
