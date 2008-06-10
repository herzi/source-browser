AC_INIT(autotool,0.0.1,mailto:herzi@gnome-de.org)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)

CXX=no
F77=no

AC_PROG_CC
AC_PROG_LIBTOOL

AM_PROG_CC_C_O

PKG_CHECK_MODULES([SB],[gconf-2.0 gio-2.0 gtk+-2.0])

PKG_CHECK_MODULES(GCONF,[gconf-2.0],[progress_has_gconf=yes],[progress_has_gconf=no])
AM_CONDITIONAL(WITH_GNOME,[test "x${progress_has_gconf}" = "xyes"])
AM_CONDITIONAL(WITH_OSX,[test "x${progress_has_gconf}" != "xyes"])

if test "x${progress_has_gconf}" != "xyes"; then
	AC_PROG_OBJC

	PLATFORM_CFLAGS=
	PLATFORM_LIBS="-framework AppKit"
	AC_SUBST(PLATFORM_CFLAGS)
	AC_SUBST(PLATFORM_LIBS)
else
	OBJC='$(CC)'
	AC_SUBST(OBJC)
	AM_CONDITIONAL(am__fastdepOBJC,true)
fi


dnl  -----------------------
dnl | GTK+ platform check   |----------------------------------
dnl  -----------------------

IGE_PLATFORM_CHECK
if test "x$IGE_PLATFORM" = xx11; then
   PLATFORM_CFLAGS="$PLATFORM_CFLAGS"
   PLATFORM_LDFLAGS="$PLATFORM_LDFLAGS"
else
   PLATFORM_CFLAGS="$PLATFORM_CFLAGS"
   PLATFORM_LDFLAGS="$PLATFORM_LDFLAGS -framework Carbon"
fi
AC_SUBST(PLATFORM_CFLAGS)
AC_SUBST(PLATFORM_LDFLAGS)

dnl  -------
dnl | GConf |--------------------------------------------------
dnl  -------

AM_GCONF_SOURCE_2

dnl  --------
dnl | Output |-------------------------------------------------
dnl  --------

AC_OUTPUT(Makefile)
