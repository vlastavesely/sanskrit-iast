#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([sanskrit-iast], [3.1.0], [vlastavesely@protonmail.ch])
AC_CONFIG_SRCDIR([transliteration.c])
AC_CONFIG_HEADERS([config.h])

PROGNAME=iast
AC_SUBST([PROGNAME])
AC_DEFINE_UNQUOTED([PROGNAME], ["$PROGNAME"], [Name of the target binary])

# Checks for programs.
AC_PROG_CC
AC_PROG_MKDIR_P
AC_PROG_INSTALL
AC_PROG_LN_S
AC_CHECK_TOOL([AR], [ar])

PKG_PROG_PKG_CONFIG
PKG_INSTALLDIR

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

# Enable for gcov if option --enable-gcov is present.
USE_GCOV=no
AC_MSG_CHECKING([whether gcov should be enabled])
AC_ARG_ENABLE(gcov,
	AS_HELP_STRING([--enable-gcov], [enable gcov code coverage analysis]), [
	if test "x$enableval" != "xno" ; then
		if test "$GCC" != "yes"; then
			AC_MSG_ERROR([gcc is required for --enable-gcov])
		fi
		AC_MSG_RESULT(yes)
		COVERAGE_CFLAGS="-O0 -g -fno-inline -fprofile-arcs -ftest-coverage"
		COVERAGE_LFLAGS="-fprofile-arcs -lgcov"
		USE_GCOV=yes
	else
		AC_MSG_RESULT(no)
		COVERAGE_CFLAGS=""
		COVERAGE_LFLAGS=""
		USE_GCOV=no
	fi
], [AC_MSG_RESULT(no)])
AC_SUBST(COVERAGE_CFLAGS)
AC_SUBST(COVERAGE_LFLAGS)
AC_SUBST(USE_GCOV)

AC_CONFIG_FILES([Makefile iast.pc])
AC_OUTPUT
