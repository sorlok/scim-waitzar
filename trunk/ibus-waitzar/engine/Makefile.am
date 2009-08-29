#
# ibus-waitzar - The WaitZar engine for IBus
#     Copyright (c) 2009 Seth N. Hetu <seth.hetu@gmail.com>
#
# Makefile.am file based on ibus-pinyin's file:
#     Copyright (c) 2007-2008 Huang Peng <shawn.p.huang@gmail.com>
#     Modified under the terms of the GNU General Public License version 3.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
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

engine_waitzar_PYTHON = \
	main.py \
	waitzar.py \
	$(NULL)
engine_waitzar_DATA = \
	$(NULL)
engine_waitzardir = $(datadir)/ibus-waitzar/engine

libexec_SCRIPTS = ibus-engine-waitzar


EXTRA_DIST = \
	$(NULL)

CLEANFILES = \
	$(NULL)

test:
	$(ENV) \
		IBUS_PINYIN_LOCATION=$(abs_top_srcdir) \
		DBUS_DEBUG=true \
		LANG=en_US \
		PYTHONPATH=$(abs_top_srcdir):$(pyexecdir) \
		$(PYTHON) $(srcdir)/main.py

