/*
 *  scim_waitzar_imengine_factory.h
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
 * Modified from scim_thai_imengine_factory.h in scim-thai-0.1.0.
 * Copyright (C) 2006 Theppitak Karoonboonyanan <thep@linux.thai.net>
 */


#ifndef __SCIM_WAITZAR_IMENGINE_FACTORY_H__
#define __SCIM_WAITZAR_IMENGINE_FACTORY_H__

#define Uses_SCIM_ICONV
#include <scim.h>
#include "scim_waitzar_imengine.h"

using namespace scim;

class WaitZarFactory : public IMEngineFactoryBase
{
public:
    WaitZarFactory (const String& uuid, const ConfigPointer& config);
    virtual ~WaitZarFactory ();

    virtual WideString  get_name      () const;
    virtual WideString  get_authors   () const;
    virtual WideString  get_credits   () const;
    virtual WideString  get_help      () const;
    virtual String      get_uuid      () const;
    virtual String      get_icon_file () const;

    virtual IMEngineInstancePointer create_instance (const String& encoding, int id = -1);

private:
    void reload_config (const ConfigPointer &config);

private:
    String m_uuid;

    //
    ConfigPointer  m_config;
    Connection     m_reload_signal_connection;

    /* for preference */
    bool burmese_numerals;
    int default_encoding;

    /* for key bindings */
};

#endif  //__SCIM_WAITZAR_IMENGINE_FACTORY_H__

