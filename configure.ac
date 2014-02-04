#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([Nathaniel], [1.0], [ruairitobrien@gmail.com])

AM_INIT_AUTOMAKE([subdir-objects foreign])

AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AX_CXX_COMPILE_STDCXX_11

# Checks for libraries.
AC_CHECK_LIB([SDL2], [SDL_CreateTexture])
AC_CHECK_LIB([SDL2_image], [IMG_Load])
AC_CHECK_LIB([SDL2_ttf], [TTF_RenderText_Solid])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_CONFIG_FILES([Makefile
                 content/Makefile
                 src/Makefile])

AC_OUTPUT
