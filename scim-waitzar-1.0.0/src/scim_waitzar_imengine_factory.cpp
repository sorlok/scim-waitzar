/*
 *  scim_waitzar_imengine_factory.cpp
 *  Copyright (C) 2008 Seth N. Hetu <sorlok_reaves@yahoo.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

/*
 * Modified from scim_thai_imengine_factory.cpp in scim-thai-0.1.0.
 * Copyright (C) 2006 Theppitak Karoonboonyanan <thep@linux.thai.net>
 */

#define Uses_SCIM_UTILITY
#define Uses_SCIM_IMENGINE
#define Uses_SCIM_CONFIG_BASE

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <scim.h>
#include "scim_waitzar_imengine_factory.h"
#include "scim_waitzar_imengine.h"
#include "scim_waitzar_imengine_config.h"
#include "intl.h"

/* SCIM Related Definitions */
#define scim_module_init waitzar_LTX_scim_module_init
#define scim_module_exit waitzar_LTX_scim_module_exit
#define scim_imengine_module_init waitzar_LTX_scim_imengine_module_init
#define scim_imengine_module_create_factory waitzar_LTX_scim_imengine_module_create_factory

static ConfigPointer _scim_config (0);

extern "C" {
    void scim_module_init (void)
    {
        bindtextdomain (GETTEXT_PACKAGE, SCIM_WAITZAR_LOCALEDIR);
        bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    }

    void scim_module_exit (void)
    {
        _scim_config.reset ();
    }

    uint32 scim_imengine_module_init (const ConfigPointer& config)
    {
        SCIM_DEBUG_IMENGINE(1) << "Initialize MYANMAR(WaitZar) Engine.\n";

        _scim_config = config;

        return 1;
    }

    IMEngineFactoryPointer scim_imengine_module_create_factory (uint32 engine)
    {
        WaitZarFactory *factory = 0;

        try {
            factory = new WaitZarFactory (String ("721c1bc0-53cc-11dd-8f47-001e4faffb4a"),
                                       _scim_config);
        } catch (...) {
            delete factory;
            factory = 0;
        }

        return factory;
    }
}


WaitZarFactory::WaitZarFactory (const String& uuid, const ConfigPointer& config) : m_uuid (uuid), m_config (config)
{
    SCIM_DEBUG_IMENGINE(1) << "Create Myanmar(WaitZar) Factory :\n";
    SCIM_DEBUG_IMENGINE(1) << "  UUID : " << uuid << "\n";

    set_languages (String ("my"));

    // config
    reload_config (m_config);
    m_reload_signal_connection = m_config->signal_connect_reload (slot (this, &WaitZarFactory::reload_config));
}

WaitZarFactory::~WaitZarFactory ()
{
    m_reload_signal_connection.disconnect ();
}

WideString WaitZarFactory::get_name () const
{
    return utf8_mbstowcs (String (_("WaitZar")));
}

WideString WaitZarFactory::get_authors () const
{
    return utf8_mbstowcs (String ("Seth N. Hetu"));
}

WideString WaitZarFactory::get_credits () const
{
    return WideString ();
}

WideString WaitZarFactory::get_help () const
{
    return WideString ();
}

String WaitZarFactory::get_uuid () const
{
    return m_uuid;
}

String WaitZarFactory::get_icon_file () const
{
    return String (SCIM_WAITZAR_ICON_FILE);
}

IMEngineInstancePointer WaitZarFactory::create_instance (const String& encoding, int id)
{
    return new WaitZarInstance (this, encoding, id, burmese_numerals, default_encoding);
}

void WaitZarFactory::reload_config (const ConfigPointer& config)
{
    if (!config) return;

    //Reload setting: burmese_numerals
    String str = config->read(String(SCIM_CONFIG_IMENGINE_WAITZAR_BURMESE_NUMERALS), String("yes"));
    if (str == String("yes")) {
	this->burmese_numerals = true;
    } else {
	this->burmese_numerals = false;
    }


    //Reload setting: default_encoding
    str = config->read(String(SCIM_CONFIG_IMENGINE_WAITZAR_DEFAULT_ENCODING), String("unicode"));
    if (str == String("zawgyi")) {
	this->default_encoding = ENCODING_ZAWGYI;
    } else if (str == String("wininnwa")) {
	this->default_encoding = ENCODING_WININNWA;
    } else {
	this->default_encoding = ENCODING_UNICODE;
    }
}

