/*
 *  scim_waitzar_keymap.h
 *  Copyright (C) 2008 Seth N. Hetu <sorlok_reaves@yahoo.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
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
 * Modified from scim_thai_keymap.h in scim-thai-0.1.0.
 * Copyright (C) 2006 Theppitak Karoonboonyanan <thep@linux.thai.net>
 */

#ifndef __SCIM_WAITZAR_KEYMAP_H__
#define __SCIM_WAITZAR_KEYMAP_H__

#include <scim.h>
#include <scim_event.h>

using namespace scim;

class WaitZarKeymap
{
public:
    enum Layout {
        MYANMAR_KEYBOARD_WAITZAR      = 0
    };

public:
    explicit WaitZarKeymap (Layout layout = MYANMAR_KEYBOARD_WAITZAR);

    void      set_layout (Layout layout);
    Layout    get_layout () const;

    //KeyEvent  map_key (const KeyEvent& rawkey);

private:
    Layout  m_layout;
};

inline
WaitZarKeymap::WaitZarKeymap (Layout layout)
    : m_layout (layout)
{
}

inline void
WaitZarKeymap::set_layout (Layout layout)
{
    m_layout = layout;
}

inline WaitZarKeymap::Layout
WaitZarKeymap::get_layout () const
{
    return m_layout;
}

#endif  // __SCIM_WAITZAR_KEYMAP_H__ 

