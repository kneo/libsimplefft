dnl Process this file with autoconf to produce a configure script.
AC_INIT([libsimplefft], [0.8.0], kkevin@gmx.net)
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR(m4)
AC_CONFIG_SRCDIR(libsimplefft/libsimplefft.h)
AC_CONFIG_SRCDIR(simplefft4j/f4jdefines.h)
LIB_CURRENT=0
LIB_REVISION=7
LIB_AGE=0

AC_SUBST(LIB_CURRENT)
AC_SUBST(LIB_REVISION)
AC_SUBST(LIB_AGE)

AC_CHECK_LIB(m, sin)
AC_CHECK_LIB(m, cos)

AC_ARG_VAR([JDK_HOME], [Java Development Kit (JDK) location])

AC_ARG_ENABLE([jni],[AC_HELP_STRING([--enable-jni],[enable Java Native Interface for libsimplefft])])

AS_IF([test "$enable_jni" = "yes"],[AS_IF([test -z "$JDK_HOME"],[AC_MSG_FAILURE([JDK_HOME has not been set. JDK_HOME must be set at run time to locate libjvm.])])])

AM_CONDITIONAL([JNI_OPT], [test "$enable_jni" = "yes"])

AM_INIT_AUTOMAKE
AC_PROG_CC

AC_PROG_LIBTOOL

AC_CONFIG_HEADER(config.h)
AC_CONFIG_FILES(Makefile libsimplefft/Makefile simplefft4j/Makefile)

AC_OUTPUT
