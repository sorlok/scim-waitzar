#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(scim-waitzar, 1.0.0, sorlok_reaves@yahoo.com)
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/scim_waitzar_imengine.h])
AC_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE

# Initialize GETTEXT (we'll almost definitely need it.)
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.17])
GETTEXT_PACKAGE=scim-waitzar
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [The gettext domain])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL

# libtool option to control which symbols are exported
# right now, symbols starting with _ are not exported
#LIBTOOL_EXPORT_OPTIONS='-export-symbols-regex "^[[^_]].*"'
#AC_SUBST(LIBTOOL_EXPORT_OPTIONS)


#Include libwaitzar
PKG_CHECK_MODULES(DEPS, waitzar >= 1.0.0)
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)


#Necessary Libraries
dnl *************
dnl     SCIM
dnl *************
SCIM_VERSION=0.99.8
PKG_CHECK_MODULES(SCIM,[scim >= $SCIM_VERSION])

# Check if we should build the setup module
PKG_CHECK_MODULES(SCIM_GTKUTILS,[scim-gtkutils >= $SCIM_VERSION],
		  		[SCIM_HAS_GTKUTILS=yes],
				[SCIM_HAS_GTKUTILS=no])
AC_SUBST(SCIM_VERSION)

# Does scim have gtkutils?
AM_CONDITIONAL(SCIM_BUILD_SETUP, [test "$SCIM_HAS_GTKUTILS" = "yes"])
if test "$SCIM_HAS_GTKUTILS" = "yes"; then
  SCIM_BUILD_SETUP=1
else
  SCIM_BUILD_SETUP=0
fi
AC_SUBST(SCIM_BUILD_SETUP)


#Not sure what this is for....
if test "x$debug" = "xyes"; then
  test "$cflags_set" = set || CFLAGS="$CFLAGS -g"
  test "$cppflags_set" = set || CPPFLAGS="$CXXFLAGS -g"
  SCIM_DEBUG_FLAGS="-DENABLE_DEBUG=1"
else
  SCIM_DEBUG_FLAGS=""
fi
AC_SUBST(SCIM_DEBUG_FLAGS)


#Set the Icon directory and Module directory, to install into later.
SCIM_ICONDIR=`$PKG_CONFIG --variable=icondir scim`
SCIM_MODULEDIR=`$PKG_CONFIG --variable=moduledir scim`

#Sensible default
if test "x$SCIM_ICONDIR" = "x"; then
  SCIM_ICONDIR=${datadir}/scim/icons
fi

#Sensible default
if test "x$SCIM_MODULEDIR" = "x"; then
  SCIM_MODULEDIR=${libdir}/scim-1.0
fi

AC_SUBST(SCIM_ICONDIR)
AC_SUBST(SCIM_MODULEDIR)
        
AC_CONFIG_FILES([Makefile 
                 data/Makefile
                 src/Makefile])
AC_OUTPUT

