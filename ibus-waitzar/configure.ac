#
# ibus-waitzar - The WaitZar engine for IBus
#     Copyright (c) 2009 Seth N. Hetu <seth.hetu@gmail.com>
#
# configure.ac file based on ibus-pinyin's file:
#     Copyright (c) 2007-2008 Huang Peng <shawn.p.huang@gmail.com>
#     Modified under the terms of the GNU General Public License version 3.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

# if not 1, append datestamp to the version number.
m4_define([package_name], [ibus-waitzar])
m4_define([ibus_released], [0])
m4_define([ibus_major_version], [1])
m4_define([ibus_minor_version], [2])
m4_define([ibus_micro_version], [0])
m4_define(ibus_maybe_datestamp,
    m4_esyscmd([if test x]ibus_released[ != x1; then date +.%Y%m%d | tr -d '\n\r'; fi]))

m4_define([ibus_version],
    ibus_major_version.ibus_minor_version.ibus_micro_version[]ibus_maybe_datestamp)


#Initialize basic data regarding the package, automake, and the macro directory.
AC_INIT([package_name], [ibus_version], [http://code.google.com/p/scim-waitzar/issues/entry], [package_name])
AM_INIT_AUTOMAKE([1.10])
AC_CONFIG_MACRO_DIR([m4])

#Set some package flags
AM_MAINTAINER_MODE

#Check python
AM_PATH_PYTHON([2.5])

# Output files
AC_CONFIG_FILES(po/Makefile.in
Makefile
ibus-waitzar.spec
engine/Makefile
engine/ibus-engine-waitzar
setup/Makefile
setup/ibus-setup-waitzar
icons/Makefile
m4/Makefile
)

AC_OUTPUT

