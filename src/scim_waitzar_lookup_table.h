/*
 *  scim_waitzar_lookup_table.h
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
 * Modified from scim_native_lookup_table.h in scim-pinyin-0.5.91
 * Copyright (c) 2005 James Su <suzhe@tsinghua.org.cn>
 */


#ifndef __SCIM_WAITZAR_LOOKUP_TABLE_H__
#define __SCIM_WAITZAR_LOOKUP_TABLE_H__

#include <scim.h>
#include <WordBuilder.h>

using namespace scim;
using namespace waitzar;

/**
 * A very simple lookup table that contains strings of Burmese words indexed from 1..10
 */
class WZLookupTable : public LookupTable
{
public:
    WZLookupTable (int page_size = 10);

   virtual void clear ();

    virtual uint32 number_of_candidates () const;


    /**
     * Get a candidate by index
    */
    virtual WideString get_candidate (int index) const;
    virtual AttributeList get_attributes (int index) const;

public:
    void setCandidates(WordBuilder *currentModelState);
    void forceCandidate(WideString str);
    void nullifyCandidateLabels();

private:
    std::vector<WideString>  candidate_labels;
    std::vector<WideString>  possible_entries;
    void regenerateCandidateLabels();

};

#endif //__SCIM_WAITZAR_LOOKUP_TABLE_H__


