project('lege', 'c',
  version: '0.1.0',
  license: 'gpl-3', # Not gpl-3-or-later
  default_options: ['warning_level=3', 'c_std=c17'],
)

abi_version = 0

subdir('lege')

if get_option('build_cli')
  subdir('cli')
endif

if get_option('build_examples')
  subdir('examples')
endif
