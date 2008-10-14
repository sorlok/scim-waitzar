/*
 *  scim_waitzar_config.h
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
 * Modified from scim_thai_config.h in scim-thai-0.1.0.
 * Copyright (C) 2006 Theppitak Karoonboonyanan <thep@linux.thai.net>
 */

#ifndef __SCIM_WAITZAR_IMENGINE_CONFIG_H__
#define __SCIM_WAITZAR_IMENGINE_CONFIG_H__

#ifndef SCIM_WAITZAR_ICON_FILE
    #define SCIM_WAITZAR_ICON_FILE           (SCIM_ICONDIR"/scim-waitzar.png")
#endif

#ifndef SCIM_WAITZAR_MODEL_FILE
    #define SCIM_WAITZAR_MODEL_FILE             (SCIM_WAITZAR_MODELDIR"/Myanmar.model")
#endif

#ifndef SCIM_WAITZAR_MYWORDS_FILE
    #define SCIM_WAITZAR_MYWORDS_FILE     (SCIM_WAITZAR_MODELDIR"/mywords.txt")
#endif

//Configuration options
#define SCIM_CONFIG_IMENGINE_WAITZAR_BURMESE_NUMERALS  "/IMEngine/WaitZar/BurmeseNumerals"
#define SCIM_CONFIG_IMENGINE_WAITZAR_DEFAULT_ENCODING  "/IMEngine/WaitZar/DefaultEncoding"

#endif  /* __SCIM_WAITZAR_IMENGINE_CONFIG_H__ */
/*
vi:ts=4:nowrap:ai:expandtab
*/
