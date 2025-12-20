dnl Autotools config.m4 for PHP extension corefill

dnl Comments in this file start with the string 'dnl' (discard to next line).
dnl Remove where necessary.

dnl If extension references and depends on an external library package, use
dnl the '--with-corefill' configure option:
dnl PHP_ARG_WITH([corefill],
dnl   [for corefill support],
dnl   [AS_HELP_STRING([--with-corefill],
dnl     [Include corefill support])])

dnl Otherwise use the '--enable-corefill' configure option:
PHP_ARG_ENABLE([corefill],
  [whether to enable corefill support],
  [AS_HELP_STRING([--enable-corefill],
    [Enable corefill support])],
  [no])

AS_VAR_IF([PHP_COREFILL], [no],, [
  dnl This section is executed when extension is enabled with one of the above
  dnl configure options. Adjust and add tests here.

  dnl
  dnl Use and adjust this code block if extension depends on external library
  dnl package which supports pkg-config.
  dnl
  dnl Find library package with pkg-config.
  dnl PKG_CHECK_MODULES([LIBFOO], [foo])
  dnl
  dnl Or if you need to check for a particular library version with pkg-config,
  dnl you can use comparison operators. For example:
  dnl PKG_CHECK_MODULES([LIBFOO], [foo >= 1.2.3])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo < 3.4])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo = 1.2.3])
  dnl
  dnl Add library compilation and linker flags to extension.
  dnl PHP_EVAL_INCLINE([$LIBFOO_CFLAGS])
  dnl PHP_EVAL_LIBLINE([$LIBFOO_LIBS], [COREFILL_SHARED_LIBADD])
  dnl
  dnl Check for library and symbol presence.
  dnl LIBNAME=corefill # you may want to change this
  dnl LIBSYMBOL=corefill # you most likely want to change this
  dnl
  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are using pkg-config:
  dnl PHP_CHECK_LIBRARY([$LIBNAME], [$LIBSYMBOL],
  dnl   [AC_DEFINE([HAVE_COREFILL_FEATURE], [1],
  dnl     [Define to 1 if corefill has the 'FEATURE'.])],
  dnl   [AC_MSG_FAILURE([FEATURE not supported by your corefill library.])],
  dnl   [$LIBFOO_LIBS])
  dnl

  dnl
  dnl Or use and adjust this code block if extension depends on external library
  dnl package, which does not support pkg-config.
  dnl
  dnl Path to library package can be given as parameter (--with-corefill=<DIR>)
  dnl SEARCH_PATH="/usr/local /usr" # you might want to change this
  dnl SEARCH_FOR="/include/corefill.h" # you most likely want to change this
  dnl AS_IF([test -r $PHP_COREFILL/$SEARCH_FOR],
  dnl   [COREFILL_DIR=$PHP_COREFILL],
  dnl   [
  dnl     for i in $SEARCH_PATH; do
  dnl       AS_IF([test -r $i/$SEARCH_FOR],
  dnl         [COREFILL_DIR=$i; break;])
  dnl     done
  dnl   ])
  dnl
  dnl AC_MSG_CHECKING([for corefill library package])
  dnl AS_VAR_IF([COREFILL_DIR],, [
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the corefill library package])
  dnl ], [AC_MSG_RESULT([found in $COREFILL_DIR])])
  dnl
  dnl Add include flag where library package headers are located on the system.
  dnl PHP_ADD_INCLUDE([$COREFILL_DIR/include])
  dnl
  dnl Check for library and symbol presence.
  dnl LIBNAME=corefill # you may want to change this
  dnl LIBSYMBOL=corefill # you most likely want to change this
  dnl
  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are not using pkg-config:
  dnl PHP_CHECK_LIBRARY([$LIBNAME], [$LIBSYMBOL], [
  dnl     PHP_ADD_LIBRARY_WITH_PATH([$LIBNAME],
  dnl       [$COREFILL_DIR/$PHP_LIBDIR],
  dnl       [COREFILL_SHARED_LIBADD])
  dnl     AC_DEFINE([HAVE_COREFILL_FEATURE], [1],
  dnl       [Define to 1 if corefill has the 'FEATURE'.])
  dnl   ],
  dnl   [AC_MSG_FAILURE([FEATURE not supported by your corefill library.])],
  dnl   [-L$COREFILL_DIR/$PHP_LIBDIR -lm])
  dnl

  dnl Add linked libraries flags for shared extension to the generated Makefile.
  dnl PHP_SUBST([COREFILL_SHARED_LIBADD])

  dnl Define a preprocessor macro to indicate that this PHP extension can
  dnl be dynamically loaded as a shared module or is statically built into PHP.
  AC_DEFINE([HAVE_COREFILL], [1],
    [Define to 1 if the PHP extension 'corefill' is available.])

  dnl Configure extension sources and compilation flags.
  PHP_NEW_EXTENSION([corefill],
    [corefill.c \
    utils.c \
    versions/php84.c \
    versions/php85.c],
    [$ext_shared],,
    [-DZEND_ENABLE_STATIC_TSRMLS_CACHE=1])
])
