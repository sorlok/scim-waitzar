/*
 *  scim_waitzar_imengine.h
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
 * Modified from scim_thai_imengine.h in scim-thai-0.1.0.
 * Copyright (C) 2006 Theppitak Karoonboonyanan <thep@linux.thai.net>
 */

#ifndef __SCIM_WAITZAR_IMENGINE_H__
#define __SCIM_WAITZAR_IMENGINE_H__

#include <scim.h>
#include <WordBuilder.h>
#include <SentenceList.h>
#include <vector>
#include <string>

#include "scim_waitzar_keymap.h"
#include "scim_waitzar_lookup_table.h"

using namespace scim;

class WaitZarFactory;

class WaitZarInstance : public IMEngineInstanceBase
{
/*public:
    typedef thstrict_t TISCMode;*/

public:
    /**
     * Create a new Instance of the WaitZar IME
     * @param factory The WaitZarFactory that is used to generate WaitZar IMEs. This factory... (more later)
     * ...(more later)
     */
    WaitZarInstance (WaitZarFactory* factory, const String& encoding, int id = -1, bool type_numerals=false, int def_encoding=ENCODING_UNICODE);

    /**
     * Destructor method for WaitZarInstance(). Currently untested.
     */
    virtual ~WaitZarInstance ();

    /*void set_isc_mode (TISCMode mode);
    void set_keyboard_layout (ThaiKeymap::Layout layout);

    TISCMode     get_isc_mode () const;
    ThaiKeymap::Layout get_keyboard_layout () const;*/

    // IMEngineInstanceBase virtual methods
    virtual bool process_key_event             (const KeyEvent& key);
    virtual void reset                         (void);
    virtual void focus_in ();
    virtual void focus_out ();
    virtual void trigger_property (const String &property);

private:
	bool LOG_ON;
  /*  void      _forget_previous_chars ();
    void      _remember_previous_char (thchar_t c);
    thcell_t  _get_previous_cell ();*/

private:
    FILE * logFile;
    WordBuilder *model;
    WaitZarKeymap           m_keymap;

    //Data structures for user interaction: prefix-string
    wchar_t                    *preedit_string;
    wchar_t                    *prefix_string;
    wchar_t                    *guess_string;
    wchar_t                    *postfix_string;
    //std::list<int>            *prevTypedWords;
    SentenceList              *sentence;
    //std::vector<int>       prevWordCaretPositions;
    //int                               cursor;

    //Data structures for user interaction: aux-string
    wchar_t                    *aux_string;
    wchar_t                    *typed_string;
    //wchar_t                    *paren_string;

    //Other useful temps
    wchar_t                   *extra_str;
    bool                         has_focus;

    //Functionality
    inline bool isGuessingWord();
    inline bool isHangingPhrase();
    void updateGuesses();
    void moveRight(int amt);
    void pickGuess(int id);
    void typeSentence(const wchar_t* extra);
    void resetInstance();

    //Helpers
    void recalcPrefixString();
    void updateCaret();
    void loadProperties();

    //Statics
    WZLookupTable          lookup;
    WZLookupTable          blank_lookup;

    //Global options
    bool typeMyanmarNumerals;

    void copyStringFromKeyStrokes(wchar_t* dest, std::vector<unsigned short> src);
};

#endif /* __SCIM_WAITZAR_IMENGINE_H__ */

