cmake_minimum_required(VERSION 3.22)
project(compiler-example)
add_definitions("-Wall -g")
enable_testing()
set(CMAKE_C_FLAGS_DEBUG "-O")

set(COMPILER  s-compile.c s-token.c)
# set(COMPILER  s-tree-compile.c s-token.c)
# set(COMPILER  s-yacc.y s-token.c)

add_executable(token     token.c s-token.c)
add_executable(calc  calc.c)
add_executable(s-calc   s-calc-r.c s-token.c)
add_executable(s-calc-left   s-calc.c s-token.c)
add_executable(s-tree   s-tree.c s-token.c)
add_executable(s-prefix      s-prefix.c s-code-print.c  s-token.c)
add_executable(s-rpn      ${COMPILER} s-code-print.c  )
add_executable(s-aarch  ${COMPILER}  s-code-arm-mac.c  )

# add_executable(s-ppc      ${COMPILER} s-code-ppc.c  )
# add_executable(s-09      ${COMPILER} s-code-09.c  )
# add_executable(s-intel   ${COMPILER} s-code-intel.c  )
# add_executable(s-intel-r   ${COMPILER} s-code-intel-r.c  )
# add_executable(s-imac   ${COMPILER} s-code-intel-mac.c  )
add_executable(s-imac64   ${COMPILER} s-code-intel64-mac.c  )
# add_executable(s-imac64-r   ${COMPILER} s-code-intel64-mac-r.c  )
add_executable(s-llvm   ${COMPILER} s-code-llvm.c)
# add_executable(s-sparc    ${COMPILER} s-code-sparc.c  )
# add_executable(s-rs6k    ${COMPILER} s-code-rs6k.c  )
# add_executable(s-rs6k-r    ${COMPILER} s-code-rs6k-r.c  )
# add_executable(s-m68k    ${COMPILER} s-code-m68k.c  )

add_test( NAME test-aarch COMMAND sh test.sh ${CMAKE_C_COMPILER} s-aarch )
