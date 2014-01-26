#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([Nathaniel], [1.0], [ruairitobrien@gmail.com])

AM_INIT_AUTOMAKE([foreign])

AC_CONFIG_SRCDIR([src/VR.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB([GL], [glPushMatrix])
AC_CHECK_LIB([GLU], [gluLookAt])
AC_CHECK_LIB([glut], [glutPostRedisplay])
AC_CHECK_LIB([m], [pow])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_CONFIG_FILES([Makefile
                 src/Makefile])

AC_OUTPUT
