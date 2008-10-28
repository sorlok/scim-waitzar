/*
 *  scim_waitzar_lookup_table.cpp
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
 * Modified from scim_native_lookup_table.cpp in scim-pinyin-0.5.91
 * Copyright (c) 2005 James Su <suzhe@tsinghua.org.cn>
 */


#define Uses_STL_AUTOPTR
#define Uses_STL_FUNCTIONAL
#define Uses_STL_VECTOR
#define Uses_STL_IOSTREAM
#define Uses_STL_FSTREAM
#define Uses_STL_ALGORITHM
#define Uses_STL_MAP
#define Uses_STL_UTILITY
#define Uses_STL_IOMANIP
#define Uses_C_STDIO
#define Uses_SCIM_UTILITY
#define Uses_SCIM_IMENGINE
#define Uses_SCIM_ICONV
#define Uses_SCIM_CONFIG_BASE
#define Uses_SCIM_CONFIG_PATH
#define Uses_SCIM_LOOKUP_TABLE


#include <scim.h>
#include "scim_waitzar_lookup_table.h"
/*#include "scim_pinyin_private.h"
#include "scim_phrase.h"
#include "scim_native_lookup_table.h"*/


/**
 * Constructor: also sets labels
 */
WZLookupTable::WZLookupTable (int page_size) : LookupTable (page_size)
{
    this->regenerateCandidateLabels();
}


void WZLookupTable::nullifyCandidateLabels()
{
    candidate_labels.clear();
    candidate_labels.push_back(L"");
    set_candidate_labels (candidate_labels);
}


void WZLookupTable::regenerateCandidateLabels()
{
    candidate_labels.clear();
    for (int i = 0; i <= 9; ++i) {
	wchar_t* num_str = new wchar_t[2];
        num_str [0] = '1' + i;
	if (i==9)
	    num_str [0] = '0';
	num_str[1] = 0x0000;
        candidate_labels.push_back (num_str);
    }
    set_candidate_labels (candidate_labels);
}


/**
 * Can set to NULL to clear all entries... 
 */
void WZLookupTable::setCandidates(WordBuilder *currentModelState)
{
	//Delete old entries
	this->clear();
	
	//Add new entries?
	if (currentModelState==NULL) 
	    return;
	
	//Construct a string for each new word
	std::vector<unsigned int> words = currentModelState->getPossibleWords();
	for (int i=0; i<words.size(); i++) {
	    //Get this word (in Myanmar3) 
	    //unsigned int oldOut = currentModelState->getOutputEncoding();
	    //currentModelState->setOutputEncoding(ENCODING_UNICODE);
	    std::vector<unsigned short> wordKeys = currentModelState->getWordKeyStrokes(words[i], ENCODING_UNICODE);
	    //currentModelState->setOutputEncoding(oldOut);
	
	    //Copy this word's key strokes
	    WideString word;// = new wchar_t[wordKeys.size()+1];
	    for (int x=0; x<wordKeys.size(); x++) {
		    word += ((wchar_t)wordKeys[x]);
	    }
	    //word[wordKeys.size()] = 0x0000;
	    
	    //Put it in our array
	    possible_entries.push_back(word);
	}
	
}


//This function is for a workaround to a scim bug. Do NOT use it under normal circumstances.
void WZLookupTable::forceCandidate(WideString str)
{
    possible_entries.push_back(str);
}


WideString WZLookupTable::get_candidate (int index) const
{
    if (index < 0 || index >= (int) number_of_candidates ())
        return WideString ();
    
    return possible_entries[index];
}



   void WZLookupTable::clear () 
  {
        LookupTable::clear ();
        possible_entries.clear();
    }
    
    uint32 WZLookupTable::number_of_candidates () const 
    {
        return possible_entries.size();
    }


//Null functionality
AttributeList WZLookupTable::get_attributes (int index) const
{
    AttributeList at;
    return at;
}



