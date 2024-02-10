module_srcs = []

# Lua modules:
# These are byte-compiled and embedded in liblege, then preloaded by the library
luajit = find_program('luajit')
luajit_compile_opts = ['-b', '-t', 'h']
if get_option('debug')
  luajit_compile_opts += '-g'
else
  luajit_compile_opts += '-s'
endif
luajit_compile_opts += ['@INPUT@', '@OUTPUT@']
luac = generator(luajit,
  arguments: luajit_compile_opts,
  output: 'lmod_@BASENAME@.h',
)

module_srcs += luac.process(
  'c_libs.lua',
  )

# C modules:
module_srcs += files(
  'enum.c',
  'log.cpp',
  'lutf8lib.c',
  'readonly.cpp',
  'strict.cpp',
  'struct.c',
  'task.cpp',
  'weak.cpp',
  )
