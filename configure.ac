#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([sanskrit-iast], [2.0.0], [vlastavesely@protonmail.ch])
AC_CONFIG_SRCDIR([iast.c])
AC_CONFIG_HEADERS([config.h])

PROGNAME=iast
AC_SUBST(PROGNAME)

# Checks for programs.
AC_PROG_CC
AC_PROG_MKDIR_P
AC_PROG_INSTALL

# Checks for libraries.
PKG_CHECK_MODULES(CHECK, check)

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memmove])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
