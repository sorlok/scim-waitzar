AM_CPPFLAGS = \
	-DSCIM_WAITZAR_LOCALEDIR=\"$(datadir)/locale\" \
	-DSCIM_WAITZAR_MODELDIR=\"$(pkgdatadir)\" \
	-DSCIM_ICONDIR=\"$(SCIM_ICONDIR)\"

noinst_HEADERS = \
	gettext.h \
	intl.h \
	scim_waitzar_keymap.h \
	scim_waitzar_imengine_factory.h \
	scim_waitzar_imengine.h \
	scim_waitzar_imengine_config.h \
	scim_waitzar_lookup_table.h 

moduledir = $(SCIM_MODULEDIR)/IMEngine
module_LTLIBRARIES = waitzar.la

waitzar_la_SOURCES = \
	scim_waitzar_keymap.cpp \
	scim_waitzar_imengine_factory.cpp \
	scim_waitzar_imengine.cpp \
	scim_waitzar_lookup_table.cpp 
	
INCLUDES = $(DEPS_CFLAGS)
waitzar_la_LIBADD = $(DEPS_LIBS)

waitzar_la_docdir = ${datadir}/doc/${PACKAGE}





#nobase_waitzar_la_doc_DATA = ${files_USERGUIDE}

#EXTRA_DIST = ${files_USERGUIDE}

waitzar_la_CFLAGS = $(SCIM_CFLAGS) $(SCIM_DEBUG_FLAGS) 
waitzar_la_CXXFLAGS = $(SCIM_CFLAGS) $(SCIM_DEBUG_FLAGS) 

waitzar_la_LDFLAGS = \
	-avoid-version \
	-rpath $(moduledir) \
	-module \
	$(LIBTOOL_EXPORT_OPTIONS) \
	$(SCIM_LIBS)

if SCIM_BUILD_SETUP
setupmoduledir = $(SCIM_MODULEDIR)/SetupUI
setupmodule_LTLIBRARIES = waitzar-imengine-setup.la

waitzar_imengine_setup_la_SOURCES = \
	scim_waitzar_imengine_setup.cpp

waitzar_imengine_setup_la_CFLAGS = $(SCIM_GTKUTILS_CFLAGS) $(SCIM_CFLAGS)
waitzar_imengine_setup_la_CXXFLAGS = $(SCIM_GTKUTILS_CFLAGS) $(SCIM_CFLAGS)

waitzar_imengine_setup_la_LDFLAGS = \
	-avoid-version \
	-rpath $(moduledir) \
	-module \
	$(LIBTOOL_EXPORT_OPTIONS) \
	$(SCIM_GTKUTILS_LIBS) \
	$(SCIM_LIBS)
endif
