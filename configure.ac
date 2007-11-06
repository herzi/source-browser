AC_INIT(autotool,0.0.1,mailto:herzi@gnome-de.org)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)

AC_PROG_CC
AC_PROG_LIBTOOL

PKG_CHECK_MODULES([SB],[gtk+-2.0])

dnl  -----------------------
dnl | GTK+ platform check   |----------------------------------
dnl  -----------------------

IGE_PLATFORM_CHECK
if test "x$IGE_PLATFORM" = xx11; then
   PLATFORM_CFLAGS=
   PLATFORM_LDFLAGS=
else
   PLATFORM_CFLAGS=
   PLATFORM_LDFLAGS="-framework Carbon"
fi
AC_SUBST(PLATFORM_CFLAGS)
AC_SUBST(PLATFORM_LDFLAGS)

dnl  --------
dnl | Output |-------------------------------------------------
dnl  --------

AC_OUTPUT(Makefile)
