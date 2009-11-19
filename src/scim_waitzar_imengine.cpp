/*
 *  scim_waitzar_imengine.cpp
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
 * Modified from scim_thai_imengine.cpp in scim-thai-0.1.0.
 * Copyright (C) 2006 Theppitak Karoonboonyanan <thep@linux.thai.net>
 */

#define Uses_SCIM_UTILITY
#define Uses_SCIM_IMENGINE
#define Uses_SCIM_CONFIG_BASE

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <string.h>
#include <scim.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include "scim_waitzar_imengine_factory.h"
#include "scim_waitzar_imengine_config.h"
#include "scim_waitzar_imengine.h"
#include "intl.h"

//Constants definitions
#define SCIM_PROP_WAITZAR_ENCODING                       "/IMEngine/Waitzar/Encoding"
#define SCIM_PROP_WAITZAR_ENCODING_UNICODE        "/IMEngine/Waitzar/Encoding/Unicode"
#define SCIM_PROP_WAITZAR_ENCODING_ZAWGYI          "/IMEngine/Waitzar/Encoding/Zawgyi"
#define SCIM_PROP_WAITZAR_ENCODING_WININNWA      "/IMEngine/Waitzar/Encoding/WinInnwa"

//Set up our language properties
static Property _waitzar_encoding_prop                   (SCIM_PROP_WAITZAR_ENCODING, "UNI");
static Property _waitzar_encoding_unicode_prop      (SCIM_PROP_WAITZAR_ENCODING_UNICODE, "Unicode 5.1+");
static Property _waitzar_encoding_zawgyi_prop       (SCIM_PROP_WAITZAR_ENCODING_ZAWGYI, "Zawgyi-One");
static Property _waitzar_encoding_wininnwa_prop     (SCIM_PROP_WAITZAR_ENCODING_WININNWA, "Win Innwa");


//Use "C" language linking for our function names (i.e., don't allow C++ to mangle the names to allow signature overloading.)
//This allows SCIM to call our init functions in a global way. (I think...)
//Not working... how am I doing this wrongly? Ah, nevermind. I'll just set it manually.
/*extern "C" {
    void scim_module_init (void)
    {
        bindtextdomain (GETTEXT_PACKAGE, SCIM_WAITZAR_LOCALEDIR);
        bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    }

    void scim_module_exit (void)
    {
        //_scim_pinyin_factory.reset ();
        //_scim_config.reset ();
    }

    uint32 scim_imengine_module_init (const ConfigPointer &config)
    {
        _waitzar_encoding_prop.set_tip (_("The encoding of the text WaitZar outputs. Change this only if you know what you're doing."));
        _waitzar_encoding_unicode_prop.set_tip (_("The encoding as defined in Unicode 5.1 and later. The default & recommended option."));
	_waitzar_encoding_zawgyi_prop.set_tip (_("The encoding used by the Zawgyi-One font, a popular font on the web which conflicts with Unicode."));
	_waitzar_encoding_wininnwa_prop.set_tip (_("The encoding used by the Win Innwa font family (inc. Win Kalaw), a popular legacy font which conflicts with ASCII."));

      //  _scim_config = config;
        return 1;
    }*/

/*    IMEngineFactoryPointer scim_imengine_module_create_factory (uint32 engine)
    {
        if (engine != 0) return IMEngineFactoryPointer (0);
        if (_scim_pinyin_factory.null ()) {
            PinyinFactory *factory = new PinyinFactory (_scim_config);
            if (factory && factory->valid ())
                _scim_pinyin_factory = factory;
            else
                delete factory;
        }
        return _scim_pinyin_factory;
    }*/
/*}*/



WaitZarInstance::WaitZarInstance (WaitZarFactory* factory,
                            const String&      encoding,
                            int                id,
                           bool type_numerals, int def_encoding)
    : IMEngineInstanceBase (factory, encoding, id)
{
	this->LOG_ON = true;
	this->typeMyanmarNumerals = type_numerals;

	blank_lookup.nullifyCandidateLabels();
	blank_lookup.forceCandidate(L" ");


    //Create our user's scim-waitzar directory
    char* userHomeWZ = new char[500];
    strcpy(userHomeWZ, getenv ("HOME"));
    strcat(userHomeWZ, "/.scim-waitzar");
    if (mkdir(userHomeWZ, S_IRWXU|S_IROTH|S_IXOTH|S_IRWXG)!=0) {
	if (errno!=EEXIST) {
	    //Disable logging....
		LOG_ON = false;
	}
    }

    //Special mywords file?
    char* mywordsPath2 = new char[200];
    strcpy(mywordsPath2, userHomeWZ);
    strcat(mywordsPath2, "/mywords.txt");

    //Re-create our log file
    if (LOG_ON) {
        strcat(userHomeWZ, "/wzlog.txt");
        logFile = fopen(userHomeWZ,"w");
        if (logFile==NULL) {
   	    //Disable logging...
 	    LOG_ON = false;
        }
    }

    //Try to find the model file & mywords file(s)
    char* modelPath = new char[200];
    char* mywordsPath = new char[200];
    strcpy(modelPath, "/usr/share/waitzar/model2/Myanmar.model");
    if (fopen(modelPath,  "r")==NULL) {
	strcpy(modelPath, "/usr/local/share/waitzar/model2/Myanmar.model");
        if (fopen(modelPath,  "r")==NULL) {
	    if (LOG_ON) {
                fprintf(logFile, "ERROR: Cannot find model file. WaitZar will not function.\n");
	        fflush(logFile);
	    }
	}
    }
    strcpy(mywordsPath, "/usr/share/waitzar/model2/mywords.txt");
    if (fopen(mywordsPath,  "r")==NULL) {
	strcpy(mywordsPath, "/usr/local/share/waitzar/model2/mywords.txt");
        if (fopen(mywordsPath,  "r")==NULL) {
	    strcpy(mywordsPath, "");
	    if (LOG_ON) {
                fprintf(logFile, "WARNING: Cannot find mywords file. WaitZar will still function, but this is odd.\n");
	        fflush(logFile);
	    }
	}
    }
    std::vector<std::string> myWords;
    if (strlen(mywordsPath)>0) {
      myWords.push_back(mywordsPath);
    }
    if (fopen(mywordsPath2, "r")!=NULL) {
      myWords.push_back(mywordsPath2);
    } else {
      strcpy(mywordsPath2, "");
    }

    //Sample log lines...
    if (LOG_ON) {
        time_t rawtime;
        struct tm * timeinfo;
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );

        fprintf(logFile, "WaitZar log file.\n");
	fflush(logFile);
        fprintf(logFile, "Scim was last re-started: %s\n", asctime (timeinfo));
	fflush(logFile);
        fprintf(logFile, "Model found at: %s\n", modelPath);
	fflush(logFile);
	if (strlen(mywordsPath2)>0) {
          fprintf(logFile, "User-defiend dictionary found at: %s\n", mywordsPath2);
	  fflush(logFile);
	}
        fprintf(logFile, "Icon is located in: %s\n", SCIM_WAITZAR_ICON_FILE);
	fflush(logFile);
        fprintf(logFile, "Settings: \n");
	fflush(logFile);
        fprintf(logFile, "    burmese_numerals: %s\n", (type_numerals?"yes":"no"));
	fflush(logFile);
        fprintf(logFile, "    default_encoding: %s\n", (def_encoding==ENCODING_UNICODE?"Unicode 5.1+":(def_encoding==ENCODING_ZAWGYI?"Zawgyi-One":(def_encoding==ENCODING_WININNWA?"Win Innwa":"unknown"))));
	fflush(logFile);
    }

    delete [] userHomeWZ;
    model = new WordBuilder(modelPath, myWords);
    model->setOutputEncoding(def_encoding);

    if (LOG_ON) {
        fprintf(logFile, "Model has loaded correctly.\n");
        fflush(logFile);
    }

    delete [] modelPath;
    delete [] mywordsPath;
    delete [] mywordsPath2;


    //Init all data structures
   // currSelectionID = 0;
    /*currStr = new wchar_t[50];
    wcscpy(currStr, L"");
    currStrWithParen = new wchar_t[50];
    wcscpy(currStrWithParen, L"");
    currPreeditStr = new wchar_t[150];
    wcscpy(currPreeditStr, L"");*/
    preedit_string = new wchar_t[500] ;
    wcscpy(preedit_string, L"");
    prefix_string = new wchar_t[400];
    wcscpy(prefix_string, L"");
    postfix_string = new wchar_t[400];
    wcscpy(postfix_string, L"");
    guess_string = new wchar_t[100];
    wcscpy(guess_string, L"");
    sentence = new SentenceList();

    aux_string = new wchar_t[150];
    wcscpy(aux_string, L"");
    typed_string = new wchar_t[150];
    wcscpy(typed_string, L"");

    //Used for any single keypress's additional needs
    extra_str = new wchar_t[500];


    has_focus = false;
}

WaitZarInstance::~WaitZarInstance ()
{
}


void WaitZarInstance::focus_in ()
{
	this->resetInstance();
	this->loadProperties();

	has_focus = true;
}

void WaitZarInstance::focus_out ()
{
	has_focus = false;

	this->resetInstance();
}



void WaitZarInstance::trigger_property (const String &property)
{
	//One of our custom buttons was pressed. Right now, that can only change the encoding.
	int currEncoding = model->getOutputEncoding();
	int newEncoding = 0;
	String newLbl = "";
	if (property == SCIM_PROP_WAITZAR_ENCODING_UNICODE) {
	    newEncoding = ENCODING_UNICODE;
	    newLbl = "UNI";
	} else if (property == SCIM_PROP_WAITZAR_ENCODING_ZAWGYI) {
	    newEncoding = ENCODING_ZAWGYI;
	    newLbl = "ZG";
	} else if (property == SCIM_PROP_WAITZAR_ENCODING_WININNWA) {
	    newEncoding = ENCODING_WININNWA;
	    newLbl = "WI";
	}

	if (newEncoding>0 && newEncoding!=currEncoding) {
	    //Update our model.
	    model->setOutputEncoding(newEncoding);

	    //Update our label
	    _waitzar_encoding_prop.set_label (newLbl);
	    update_property (_waitzar_encoding_prop);
	}

}


void WaitZarInstance::loadProperties()
{
    //Add our properties to a proplist
    PropertyList proplist;
    proplist.push_back (_waitzar_encoding_prop);
    proplist.push_back (_waitzar_encoding_unicode_prop);
    proplist.push_back (_waitzar_encoding_zawgyi_prop);
    proplist.push_back (_waitzar_encoding_wininnwa_prop);

    //Now, register these properties
    register_properties(proplist);

    //Set tooltips
    _waitzar_encoding_prop.set_tip (_("The encoding of the text WaitZar outputs. Change this only if you know what you're doing."));
    _waitzar_encoding_unicode_prop.set_tip (_("The encoding as defined in Unicode 5.1 and later. The default & recommended option."));
    update_property (_waitzar_encoding_unicode_prop);
    _waitzar_encoding_zawgyi_prop.set_tip (_("The encoding used by the Zawgyi-One font, a popular font on the web which conflicts with Unicode."));
    update_property (_waitzar_encoding_zawgyi_prop);
    _waitzar_encoding_wininnwa_prop.set_tip (_("The encoding used by the Win Innwa font family (inc. Win Kalaw), a popular legacy font which conflicts with ASCII."));
    update_property (_waitzar_encoding_wininnwa_prop);

    //Finally, double-check the label... although this really shouldn't change.
    String newLbl = "";
    int encoding = model->getOutputEncoding();
    if (encoding == ENCODING_UNICODE)
	    newLbl = "UNI";
    else if (encoding == ENCODING_ZAWGYI)
	newLbl = "ZG";
    else if (encoding == ENCODING_WININNWA)
	newLbl = "WI";
    else
	newLbl = "??";
    _waitzar_encoding_prop.set_label (newLbl);
    update_property (_waitzar_encoding_prop);
}


bool WaitZarInstance::process_key_event (const KeyEvent& key)
{
    //Pass along null key presses (event generated?) and key releases.
    //Also ignore if we don't have the focus. This is included in scim_pinyin, and I see no reason to exclude it here.
    if (key.is_key_release() || key.code == 0 || !has_focus) {
        /*|| __is_context_intact_key (key.code)*/
        return false;
    }

    //Basically ignore control key masks
    if (
	   ((key.mask & SCIM_KEY_ShiftMask) != 0)
        || ((key.mask & SCIM_KEY_ControlMask) != 0)
        || ((key.mask & SCIM_KEY_AltMask) != 0)
        || ((key.mask & SCIM_KEY_MetaMask) != 0)
        || ((key.mask & SCIM_KEY_SuperMask) != 0)
        || ((key.mask & SCIM_KEY_HyperMask) != 0)
        || ((key.mask & SCIM_KEY_QuirkKanaRoMask) != 0)
    ) {
	return false;
    }


    //INPUT:
    //(a-z)
    if (key.code>='a' && key.code<='z') {
	if (model->typeLetter(key.code)) {
	    //Append this character
	    int len = wcslen(typed_string);
	    typed_string[len] = key.code;
	    typed_string[len+1] = 0x0000;

	    //Update all guesses
	    updateGuesses();
	}
	return true;
    } else if (key.code == ' ') {
	if (isGuessingWord()) {
	    if (guess_string[0] != 0x0000) {
		pickGuess(-1);
	    }
	    return true;
	} else if (isHangingPhrase()) {
	    if (sentence->getCursorIndex()==-1 || sentence->getCursorIndex()<((int)sentence->size()-1)) {
		//Advance the cursor
		sentence->moveCursorRight(1, model);
		recalcPrefixString();
		updateGuesses();
		//updateCaret();
	    } else {
		//The cursor's at the end of the line, or there's only one word
		typeSentence(L"");
	    }
	    return true;
	}
    } else if (key.code>='0' && key.code<='9') {
	int number = key.code=='0' ? 9 : key.code-'1';
	if (isGuessingWord()) {
	    //Shortcut-jump to that number.
	    pickGuess(number);
	    return true;
	} else if (typeMyanmarNumerals) {
	    //Insert that number, or its myanmar equivalent
	    sentence->insert(key.code-'0');
	    sentence->moveCursorRight(0, true, model);
	    recalcPrefixString();
	    updateGuesses();
	    return true;
	}
    } else if (key.code==SCIM_KEY_Return) {
	if (isGuessingWord()) {
	    if (guess_string[0] != 0x0000) {
		pickGuess(-1);
	    }
	    return true;
	} else if (isHangingPhrase()) {
	    typeSentence(L"");
	    return true;
	}
    } else if (key.code==SCIM_KEY_Left || key.code==SCIM_KEY_Right) {
	int amt = key.code==SCIM_KEY_Right ? 1 : -1;
	if (isGuessingWord()) {
	    moveRight(amt);
	    return true;
	} else if (isHangingPhrase()) {
	    sentence->moveCursorRight(amt, model);
	    recalcPrefixString();
	    updateGuesses();
	    //updateCaret();
	    return true;
	}
    } else if (key.code==SCIM_KEY_Up || key.code==SCIM_KEY_Down) {
	int amt = key.code==SCIM_KEY_Down ? 1 : -1;
	if (isGuessingWord()) {
	    //Page Up/Down
	    if (key.code==SCIM_KEY_Up && lookup.number_of_candidates()>0) {
		lookup.page_up();
		update_lookup_table(lookup);
	    } else if (key.code==SCIM_KEY_Down && lookup.number_of_candidates()>0)  {
		lookup.page_down();
		update_lookup_table(lookup);
	    }
	    updateGuesses();
	    return true;
	}
    } else if (key.code==',' || key.code=='.') {
	if (isGuessingWord()) {
	    //Consume input
	    return true;
	} else /*if (isHangingPhrase())*/ {
	    //Insert this character & type the sentence
	    extra_str[0] = model->getStopCharacter(key.code=='.');
	    extra_str[1] = 0x0000;
	    typeSentence(extra_str);
	    return true;
	}
    } else if (key.code==SCIM_KEY_Delete || key.code==SCIM_KEY_BackSpace) {
        if (isGuessingWord()) {
	    if (key.code==SCIM_KEY_BackSpace) {
		model->backspace();
		int len = wcslen(typed_string);
		if (len>0) {
		    typed_string[len-1] = 0x0000;
		    updateGuesses();
		}
	    }
	    return true;
	} else if (isHangingPhrase()) {
	    //Arrange the cursor properly
	    bool didDelete = false;
	    if (key.code==SCIM_KEY_BackSpace) {
		didDelete = sentence->deletePrev(model);
	    } else {
		didDelete = sentence->deleteNext();
	    }

	    //Delete this word
	    if (didDelete) {
		recalcPrefixString();
	        updateGuesses();
		//updateCaret();
	    }
	    return true;
	}
    } else if (key.code==SCIM_KEY_Escape) {
	if (isGuessingWord()) {
	    //Soft reset
	    model->reset(true);
	    wcscpy(typed_string, L"");
            recalcPrefixString();
	    updateGuesses();
	    return true;
	} else if (isHangingPhrase()) {
	    resetInstance();
	    return true;
	}
    } else if (key.code==SCIM_KEY_Home || key.code==SCIM_KEY_End) {
	if (isGuessingWord() || isHangingPhrase()) {
	    //Avoid confusion
	    return true;
	}
    }

    //Global else:
    return false;
}




inline bool WaitZarInstance::isGuessingWord()
{
	return aux_string[0] != 0x0000;
}

inline bool WaitZarInstance::isHangingPhrase()
{
	return prefix_string[0]!=0x0000 || postfix_string[0]!=0x0000;
}


void WaitZarInstance::updateGuesses()
{
    //Update aux_string
    if (wcslen(model->getParenString())==0)
        wcscpy(aux_string, typed_string);
    else
        swprintf(aux_string, 500, L"%ls(%ls)", typed_string, model->getParenString());
       update_aux_string(aux_string);
	if (wcslen(aux_string)>0) {
            show_aux_string();
            show_lookup_table();
	} else {
	    hide_aux_string();
	    hide_lookup_table();
	}

    //Update the current guess
    wcscpy(guess_string, L"");
    if (model->getPossibleWords().size()>0)
	copyStringFromKeyStrokes(guess_string, model->getWordKeyStrokes(model->getPossibleWords()[model->getCurrSelectedID()]));
    swprintf(preedit_string, 500, L"%ls%ls%ls", prefix_string, guess_string, postfix_string);
    update_preedit_string(preedit_string);
    if (wcslen(preedit_string)>0)
        show_preedit_string();
    else
        hide_preedit_string();

   /* fprintf(logFile, "now on: %ls\n", typed_string);
    fprintf(logFile, "   prefix_string: %ls\n", prefix_string);
    fprintf(logFile, "   guess_string: %ls\n", guess_string);
    fprintf(logFile, "   postfix_string: %ls\n", postfix_string);
    fflush(logFile);*/

    //Update the candidate table & cursor
    lookup.setCandidates(model);
    lookup.show_cursor(model->getPossibleWords().size()>0);
    lookup.set_cursor_pos(model->getCurrSelectedID()); //I think...
    update_lookup_table(lookup);

    //Update the caret
    update_preedit_caret(wcslen(prefix_string));

    //TEMP: Bugfix on behalf of SCIM
    if (lookup.number_of_candidates()==0) {
	update_lookup_table(blank_lookup);
    }
}


void WaitZarInstance::moveRight(int amt)
{
    //Move the cursor
    if (model->moveRight(amt)) {
	updateGuesses();
    }
}


void WaitZarInstance::pickGuess(int id)
{
    //Get this guess
    std::pair<bool, unsigned int> guess = model->typeSpace(id);
    if (guess.first) {
        //Append in the correct location
	sentence->insert(guess.second);

	//Update the prefix string & model
	recalcPrefixString();
	model->reset(false);
	wcscpy(typed_string, L"");

	//Update all visible strings
	updateGuesses();

	//Update the caret
	//updateCaret();
    }
}


void WaitZarInstance::typeSentence(const wchar_t* extra)
{
    //Commit the modified string
    wcscat(prefix_string, postfix_string);
    wcscat(prefix_string, extra);
    commit_string(prefix_string);

    //Reset
    resetInstance();
}


void WaitZarInstance::resetInstance()
{
    //Reset the model
    model->reset(true);

    //Reset our array & counter
    sentence->clear();

    //Reset all strings
    wcscpy(prefix_string, L"");
    wcscpy(postfix_string, L"");
    wcscpy(typed_string, L"");
    updateGuesses();
}



void WaitZarInstance::recalcPrefixString()
{
    wcscpy(prefix_string, L"");
    wcscpy(postfix_string, L"");
    //prevWordCaretPositions.clear();
    int put_count = 0;
    wchar_t *temp = new wchar_t[100];
    int currChars = 0;

    for (std::list<int>::iterator addIT=sentence->begin(); addIT!=sentence->end(); addIT++) {
	std::vector<unsigned short> strokes = model->getWordKeyStrokes(*addIT);
	copyStringFromKeyStrokes(temp, strokes);

	//Append to prefix or postfix?
	// The final preedit_string is displayed as prefix + guess + postfix
	if (sentence->getCursorIndex() >= put_count++) {
	    wcscat(prefix_string, temp);
	} else {
	    wcscat(postfix_string, temp);
	}

	currChars += strokes.size();
	//prevWordCaretPositions.push_back(currChars);
    }
    delete [] temp;
}


/*void WaitZarInstance::updateCaret()
{
    int caretPos = 0;
    int cursor = sentence->getCursorIndex();
    if (cursor>-1) {
	caretPos = prevWordCaretPositions[cursor];
    }
    update_preedit_caret(caretPos);
}*/



void WaitZarInstance::reset()
{
    resetInstance();
}


void WaitZarInstance::copyStringFromKeyStrokes(wchar_t* dest, std::vector<unsigned short> src)
{
	for (int i=0; i<src.size(); i++) {
	    dest[i] = src[i];
	}
	dest[src.size()] = 0x0000;
}



