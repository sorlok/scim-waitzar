#!/bin/sh
#
# ibus-waitzar - Burmese Input for iBus
#
# Copyright (c) 2009 Seth N. Hetu <sorlok_reaves@yahoo.com>
#
# This file is part of ibus-waitzar.
#
# ibus-wiatzar is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
#
# ibus-waitzar is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with ibus-waitzar.  If not, see <http://www.gnu.org/licenses/>.

# Based on:
# ibus-tmpl, copyright 2007-2008 Peng Huang <shawn.p.huang@gmail.com>
# GPL2

import gobject
import pango
import ibus
import os
import sys
import datetime
from ibus import keysyms
from ibus import modifier

import libwaitzar


#Define properties in a method similar to ibus-anthy
ENCODING_UNICODE, \
ENCODING_ZAWGYI, \
ENCODING_WININNWA \
        = range(3)
NUMBERS_MYANMAR, \
NUMBERS_ARABIC \
        = range(2)


#Must factor out later...
PKG_DATA_DIR = u'/usr/share/ibus-waitzar/'


log_debug = True

def delete_log():
    if (log_debug):
        if not os.path.exists(os.path.join(os.path.expanduser("~"),".wz")):
          os.makedirs(os.path.join(os.path.expanduser("~"),".wz"))
        f = open(os.path.join(os.path.expanduser("~"),".wz/log.txt"), 'w')
        f.close()
    
def append_log(line):
    if (log_debug):
        if not os.path.exists(os.path.join(os.path.expanduser("~"),".wz")):
          os.makedirs(os.path.join(os.path.expanduser("~"),".wz"))
        f = open(os.path.join(os.path.expanduser("~"),".wz/log.txt"), 'a')
        f.write(line + '\n')
        f.close()

def append_error():
    if (log_debug):
        if not os.path.exists(os.path.join(os.path.expanduser("~"),".wz")):
          os.makedirs(os.path.join(os.path.expanduser("~"),".wz"))
        import traceback
        f = open(os.path.join(os.path.expanduser("~"),".wz/log.txt"), 'a')
        traceback.print_exc(None, f)
        f.close()

class Engine(ibus.EngineBase):
    def __init__(self, bus, object_path):
        super(Engine, self).__init__(bus, object_path)
        
        delete_log()
        append_log("Creating engine... ibus version " + ibus.get_version())
        append_log("Time: " + str(datetime.datetime.now()))

        #Set our initial encoding state
        self.__encoding = ENCODING_UNICODE
        self.__typenum = NUMBERS_MYANMAR
        self.__prop_dict = {}
        self.__prop_list = self.__init_props()

        #Prop 1 - Check for saved value
        try:
            mode = self.__prefs.get_value('common', 'output_encoding')
            mode = 'OutputEncoding.' + ['Unicode', 'Zawgyi', 'WinInnwa'][mode]
            self.__output_encoding_activate(mode, ibus.PROP_STATE_CHECKED)
        except: pass

        #Prop 2 - Check for saved value
        try:
            mode = self.__prefs.get_value('common', 'type_numerals')
            mode = 'TypeNumerals.' + ['Burmese', 'Arabaic'][mode]
            self.__type_numerals_activate(mode, ibus.PROP_STATE_CHECKED)
        except: pass
        
        self.__has_focus = False

        #My strings
        self.__typed_string = u""
        self.__aux_string = u""
        self.__guess_string = u""
        self.__prefix_string = u""
        self.__postfix_string = u""
        self.__preedit_string = u""

        self.__is_invalidate = False
        self.__lookup_table = ibus.LookupTable()
        	
        append_log("Engine created")
        
        modelPath = '/usr/share/waitzar/model2/Myanmar.model'
        mywordsPaths = libwaitzar.StringVec()
        mywordsPaths.append('/usr/share/waitzar/model2/mywords.txt')
        
        self.model = libwaitzar.WordBuilder(modelPath, mywordsPaths)
        self.model.reset(True)
        self.sentence = libwaitzar.SentenceList()
        
        append_log("Model created")
        
    if ibus.get_version() >= '1.2.0':
        def process_key_event(self, keyval, keycode, state):
            try:
                return self.process_key_event_internal(keyval, keycode, state)
            except:
                append_error()
                return False
    else:
        def process_key_event(self, keyval, state):
            try:
                return self.process_key_event_internal(keyval, 0, state)
            except:
                append_error()
                return False



    #Handle properties
    def property_activate(self, prop_name, state):
        try:
            append_log("Property activate: " + str(prop_name))
            if state == ibus.PROP_STATE_CHECKED:
                if self.__output_encoding_activate(prop_name, state):
                    return
                if self.__type_numerals_activate(prop_name, state):
                    return
            else:
                if prop_name == 'setup':
                    self.__start_setup()
                else:
                    self.__prop_dict[prop_name].set_state(state)
        except:
            append_error()


    #Change our output encoding
    def __output_encoding_activate(self, prop_name, state):
        append_log("Testing output encoding... " + str(pop_name))
        if not prop_name.startswith(u"OutputEncoding."):
            return False
        append_log("Yes")

        #Check each encoding
        possible_encodings = {
            u"OutputEncoding.Unicode"   :  (ENCODING_UNICODE, u"Unicode 5.1", PKG_DATA_DIR + u"/icons/input_uni.svg"),
            u"OutputEncoding.Zawgyi"     :  (ENCODING_ZAWGYI, u"Zawgyi-One", PKG_DATA_DIR + u"/icons/input_zg.svg"),
            u"OutputEncoding.WinInnwa"  :  (ENCODING_WININNWA, u"Win Innwa", PKG_DATA_DIR + u"/icons/input_wi.svg")
        }
        if prop_name not in possible_encodings:
            append_log("Error: Unknow prop_name = %s" % prop_name)
            return True
        self.__prop_dict[prop_name].set_state(state)
        self.update_property(self.__prop_dict[prop_name])

        #Now, try to react to the change
        mode, label, icon = possible_encodings[prop_name]
        if self.__encoding == mode:
            return True

        #Update the property
        self.__encoding = mode
        prop = self.__prop_dict[u"OutputEncoding"]
        prop.label = label
        prop.icon = icon
        self.update_property(prop)

        #Reset the model
        self.reset()
        self.__invalidate()


    #Change how we type numbers
    def __type_numerals_activate(self, prop_name, state):
        if not prop_name.startswith(u"TypeNumerals."):
            return False

        #Check each encoding
        numeral_types = {
            u"TypeNumerals.Burmese"   :  (NUMBERS_MYANMAR, u"Burmese Numerals", PKG_DATA_DIR + u"/icons/num_mm.svg"),
            u"TypeNumerals.Arabic"     :  (NUMBERS_ARABIC, u"Regular Numerals", PKG_DATA_DIR + u"/icons/num_english.svg")
        }
        if prop_name not in numeral_types:
            append_log("Error: Unknow prop_name = %s" % prop_name)
            return True
        self.__prop_dict[prop_name].set_state(state)
        self.update_property(self.__prop_dict[prop_name])

        #Now, try to react to the change
        mode, label, icon = numeral_types[prop_name]
        if self.__typenum == mode:
            return True

        #Update the property
        self.__typenum = mode
        prop = self.__prop_dict[u"TypeNumerals"]
        prop.label = label
        prop.icon = icon
        self.update_property(prop)

        #Reset the model
        self.reset()
        self.__invalidate()


    def __start_setup(self):
        #Nothing for now
        pass


    def process_key_event_internal(self, keyval, keycode, state):
        #Ignore if: 
        #    - The key is being released
        #    - The keycode is (somehow) zero
        #    - The program does not have focus
        if (keycode==0 or not self.__has_focus or (state & modifier.RELEASE_MASK)):
            return False

        #If any modifiers are pressed besides shift, ignore this key event
        if (state & (modifier.CONTROL_MASK | modifier.ALT_MASK)):
            return False;

        #Debug: output key information
        #append_log('key press: ' + str(keyval) + ' , ' + str(keycode) + ' , ' + str(state))

        #
        # MAIN INPUT LOOP -- respond to user key presses, trigger calls for __update() if necessary
        #
        #First check: are we typing a letter from [a..z]? (Capitals included)
        #   If so, type this letter into our model
        #
        letter = keyval + (keysyms.a - keysyms.A) if keyval in xrange(keysyms.A, keysyms.Z + 1) else keyval
        if letter in xrange(keysyms.a, keysyms.z + 1):
            if self.model.typeLetter(chr(letter)):
                #Append this letter to our typed_string, and update all guesses
                self.__typed_string += unichr(letter)
                self.__invalidate()
            return True


        # 
        # Space/Enter - type to commit the word or sentence. Space also advances the cursor
        #
        elif keyval == keysyms.space or keyval == keysyms.Return:
            if self.isGuessingWord():
                #If we've typed a word, pick the current selection
                if self.__guess_string:
                    self.pickGuess(-1)
            elif self.isHangingPhrase():
                #If we're typing a sentence, the space bar serves two different functions
                if keyval == keysyms.space and (self.sentence.getCursorIndex()==-1 or self.sentence.getCursorIndex()<self.sentence.size()-1):
                    #We're not at the end of the sentence; advance the cursor
                    self.sentence.moveCursorRight(1, self.model)
                    self.recalcPrefixString()
                else:
                    #We're at the end of the sentence. Commit it to the editor
                    self.typeSentence('')
            else:
                return False
            return True

        # 
        # Numerals - Shortcut to that candidate word, or type the number
        #
        elif keyval == keyval >= keysyms._0 and keyval <= keysyms._9:
            number = keyval-keysyms._1 if keyval > keysyms._0 else 9
            if self.isGuessingWord():
                #Shortcut to that word
                self.pickGuess(number)
            else:  #Always type numbers
                #Insert that number, or its Myanmar equivalent
                self.sentence.insert(keyval - keysyms._0)
                self.sentence.moveCursorRight(0, True, self.model)
                self.recalcPrefixString()
            return True

        # 
        # Left/Right/Up/Down - Change the selected candidate, or move the prefix cursor
        #
        elif keyval==keysyms.Up or keyval==keysyms.Down or keyval==keysyms.Left or keyval==keysyms.Right:
            amount = 1 if (keyval==keysyms.Down or keyval==keysyms.Right) else -1
            if self.isGuessingWord():
                #Shortcut to that word
                self.moveRight(amount)
            elif self.isHangingPhrase():
                #Move the sentence cursor
                self.sentence.moveCursorRight(amount, self.model)
                self.recalcPrefixString()
            else:
                return False
            return True

        # 
        # PgUp/PgDown - Change the lookup table's page
        #
        elif keyval==keysyms.Page_Up or keyval==keysyms.KP_Page_Up or keyval==keysyms.Page_Down or keyval==keysyms.KP_Page_Down:
            go_down = True if keyval==keysyms.Page_Down or keyval==keysyms.KP_Page_Down else False
            if self.isGuessingWord():
                if go_down:
                    self.page_down()
                else:
                    self.page_up()
                return True
            else:
                return False

        # 
        # Period/Comma - Type the sentence, add punctuation
        #
        elif keyval==keysyms.comma or keyval==keysyms.period:
            if self.isGuessingWord():
                return True #Consume input
            else:
                #Insert this character and type the sentence
                self.typeSentence(unichr(self.model.getStopCharacter(keyval==keysyms.period)))
                return True

        # 
        # Delete/Backspace - Modify our letter string (or sentence string)
        #
        elif keyval==keysyms.Delete or keyval==keysyms.BackSpace:
            if self.isGuessingWord():
                if keyval==keysyms.BackSpace:
                    #Delete the previous letter
                    self.model.backspace()
                    if self.__typed_string:
                        self.__typed_string = self.__typed_string[:-1]
                        self.__invalidate()
            elif self.isHangingPhrase():
                didDelete = False
                if keyval==keysyms.BackSpace:
                    #Delete the previous word
                    didDelete = self.sentence.deletePrev(self.model)
                else:
                    #Delete the next word
                    didDelete = self.sentence.deleteNext()

                #Perform local deletion operations
                if didDelete:
                    self.recalcPrefixString()
            else:
                return False
            return True


        # 
        # Escape - general cancel key
        #
        elif keyval == keysyms.Escape:
            if self.isGuessingWord():
                #Soft reset
                self.model.reset(True)
                self.__typed_string = ''
                self.recalcPrefixString()
            elif self.isHangingPhrase():
                #Regular reset
                self.reset()
            else:
                return False
            return True


        # 
        # Home/End - just consume these events to avoid interface confusion
        #
        elif keyval==keysyms.Home or keyval==keysyms.End:
            if self.isGuessingWord() or self.isHangingPhrase():
                return True


        # 
        # Anything Else - don't process it
        #
        else:
            return False



    #Are we currently guessing a word?
    def isGuessingWord(self):
        return bool(self.__aux_string)

    #Are we currently in the middle of typing a sentence?
    def isHangingPhrase(self):
        return bool(self.__prefix_string) or bool(self.__postfix_string)

    #Move the cursor based on the model
    def moveRight(self, amount):
        if self.model.moveRight(amount):
            #TODO: amount can be something besides -1 and 1; we should allow for this.
            if amount>0:
                self.cursor_down()
            elif amount <0:
                self.cursor_up()
        self.__invalidate()

    #Pick the guess with a given id, -1 for the currently-selected guess
    def pickGuess(self, id):
        #Get this guess
        guess = self.model.typeSpace(id)
        if not guess[0]:
            return

        #Add it to our sentence
        self.sentence.insert(guess[1])

        #TEMP
        #append_log('guessing: ' + str(id)) 
        #append_log('res: ' + str(guess[0])) 
        #append_log('res: ' + str(guess[1])) 

        #Update the prefix string and model
        self.recalcPrefixString()
        self.model.reset(False)
        self.__typed_string = ''

    #Type the current sentence, appending an "Extra" string if requested to
    def typeSentence(self, extra):
        #Commit the modified string
        self.__prefix_string += self.__postfix_string
        if extra:
            self.__prefix_string += extra
        self.__commit_string(self.__prefix_string)

        #Reset
        self.reset()

    #Calculate our prefix string, which is essentially the sentence we've typed with the current "guess" inserted wherever the cursor is
    def recalcPrefixString(self):
        #Init
        self.__prefix_string = ''
        self.__postfix_string = ''
        put_count = 0

        #For each word, append it to either prefix or postfix
        atID = self.sentence.__iter__()
        try: 
            while True:
                word = self.model.getWordKeyStrokes(atID.next())
            
                #Append to prefix or postfix?
                if self.sentence.getCursorIndex() >= put_count:
                    self.__prefix_string += word
                else:
                    self.__postfix_string += word

                #Increment
                put_count += 1
        except StopIteration: pass #Python's way of doing iterators

        #Force a call to invalidate()
        self.__invalidate()



    #Schedule a recalculation for later
    def __invalidate(self):
        if self.__is_invalidate:
            return
        self.__is_invalidate = True
        gobject.idle_add(self.__update, priority = gobject.PRIORITY_LOW)


    def page_up(self):
        if self.__lookup_table.page_up():
            self.page_up_lookup_table()
            self.__invalidate()
            return True
        return False

    def page_down(self):
        if self.__lookup_table.page_down():
            self.page_down_lookup_table()
            self.__invalidate()
            return True
        return False

    def cursor_up(self):
        if self.__lookup_table.cursor_up():
            self.cursor_up_lookup_table()
            return True
        return False

    def cursor_down(self):
        if self.__lookup_table.cursor_down():
            self.cursor_down_lookup_table()
            return True
        return False

    def __commit_string(self, text):
        self.commit_text(ibus.Text(text))
        self.__typed_string = u""
        self.__update()

    def __update(self):
        try:
            #First, update our strings
            self.updateAuxString()
            self.updateGuessString()
            self.updateTableEntries()

            #Debug: update log
            #append_log('update called: ' + str([self.__typed_string, self.__preedit_string]))

            #Cache lengths
            preedit_len = len(self.__preedit_string)
            aux_len = len(self.__aux_string)
            prefix_len = len(self.__prefix_string)
            guess_len = len(self.__guess_string)

            #Get an attribute list for the preedit string
            attrs = ibus.AttrList()
            if self.__preedit_string:
                #Put the entire string in red; underline only the current "guessed" part
                attrs.append(ibus.AttributeForeground(0xff0000, 0, preedit_len))
                attrs.append(ibus.AttributeUnderline(pango.UNDERLINE_SINGLE, prefix_len, prefix_len + guess_len))

            #Now update all strings
            self.update_preedit_text(ibus.Text(self.__preedit_string, attrs), prefix_len, preedit_len > 0)

            #Additional attributes
            attrs = ibus.AttrList()
            attrs.append(ibus.AttributeForeground(0x000000, 0, aux_len))
            attrs.append(ibus.AttributeUnderline(pango.UNDERLINE_SINGLE, 0, 0))
            self.update_auxiliary_text(ibus.Text(self.__aux_string, attrs), aux_len > 0)
            

            #Update our lookup table, too
            self.__lookup_table.set_cursor_pos(self.model.getCurrSelectedID())
            self.update_lookup_table(self.__lookup_table, aux_len>0)

            #Done - now in a valid state
            self.__is_invalidate = False
        except:
           append_error()
           return False


    def focus_in(self):
        self.register_properties(self.__prop_list)
        self.__has_focus = True
        self.reset()

    def focus_out(self):
        self.__has_focus = False
        self.reset()
    
    def updateAuxString(self):
        #Update our auxiliary string
        parenStr = self.model.getParenString()
        if parenStr:
            parenStr = '('+parenStr+')'
        self.__aux_string = self.__typed_string + parenStr

    def updateGuessString(self):
        #Update the current guess
        self.__guess_string = u""
        if self.model.getPossibleWords():
            self.__guess_string = self.model.getWordKeyStrokes(self.model.getPossibleWords()[self.model.getCurrSelectedID()])
        self.__preedit_string = '%s%s%s' % (self.__prefix_string, self.__guess_string, self.__postfix_string)

    def updateTableEntries(self):
        #Update our lookup table entries
        self.__lookup_table.clean()
        if self.__preedit_string:
            #Loop through each entry; add its unicode value (for display)
            words = self.model.getPossibleWords()
            for word in words:
                candidate = self.model.getWordKeyStrokes(word, libwaitzar.encoding.unicode)
                self.__lookup_table.append_candidate(ibus.Text(candidate))

    #For external debugging ONLY
    def testUpdate(self):
        self.__update()


    def __init_props(self):
        #Create all of our properties in one go
        wz_props = ibus.PropList()

        #
        #Property 1: Output Encoding
        #
        enc_key = u"OutputEncoding"
        encoding_prop = ibus.Property(
                key = enc_key,
                type = ibus.PROP_TYPE_NORMAL,
                label = u"",
                tooltip = u"Switch Output Encoding",
                icon = PKG_DATA_DIR + u"/icons/input_uni.svg"
        )
        self.__prop_dict[enc_key] = encoding_prop

        #List all properties in this menu
        props = ibus.PropList()
        props.append(ibus.Property(
            key = enc_key + "." + u"Unicode",
            type = ibus.PROP_TYPE_NORMAL,
            label = u"Unicode 5.1",
            icon = PKG_DATA_DIR + u"/icons/input_uni.svg"))
        props.append(ibus.Property(
            key = enc_key + "." + u"Zawgyi",
            type = ibus.PROP_TYPE_NORMAL,
            label = "Zawgyi-One",
            icon = PKG_DATA_DIR + u"/icons/input_zg.svg"))
        props.append(ibus.Property(
            key = enc_key + "." + u"WinInnwa",
            type = ibus.PROP_TYPE_NORMAL,
            label = "Win Innwa",
            icon = PKG_DATA_DIR + u"/icons/input_wi.svg"))

        #set the state, copy all values into the main dictionary, our main menu item, and the return value
        props[self.__encoding].set_state(ibus.PROP_STATE_CHECKED)
        for prop in props:
            self.__prop_dict[prop.key] = prop
        encoding_prop.set_sub_props(props)
        wz_props.append(encoding_prop)


        #
        #Property 2: Type Burmese Numerals
        #
        enc_key = u"TypeNumerals"
        encoding_prop = ibus.Property(
                key = enc_key,
                type = ibus.PROP_TYPE_NORMAL,
                label = u"",
                tooltip = "Type Burmese Numerals",
                icon = PKG_DATA_DIR + u"/icons/num_mm.svg"
        )
        self.__prop_dict[enc_key] = encoding_prop

        #List all properties in this menu
        props = ibus.PropList()
        props.append(ibus.Property(
            key = enc_key + "." + u"Burmese",
            type = ibus.PROP_TYPE_NORMAL,
            label = "Burmese Numerals",
            icon = PKG_DATA_DIR + u"/icons/num_mm.svg"))
        props.append(ibus.Property(
            key = enc_key + "." + u"Arabic",
            type = ibus.PROP_TYPE_NORMAL,
            label = "Regular Numerals",
            icon = PKG_DATA_DIR + u"/icons/num_english.svg"))

        #set the state, copy all values into the main dictionary, our main menu item, and the return value
        props[self.__encoding].set_state(ibus.PROP_STATE_CHECKED)
        for prop in props:
            self.__prop_dict[prop.key] = prop
        encoding_prop.set_sub_props(props)
        wz_props.append(encoding_prop)

        #
        #Done
        #
        wz_props.append(ibus.Property(key=u"setup", tooltip="Configure WaitZar"))
        return wz_props


    def reset(self):
        #Reset the model
        self.model.reset(True)

        #Make sure our encoding is up-to-date
        currEncoding = (libwaitzar.encoding.unicode, libwaitzar.encoding.zawgyi, libwaitzar.encoding.wininnwa)[self.__encoding]
        if (self.model.getOutputEncoding() != currEncoding):
            self.model.setOutputEncoding(currEncoding)
        
        #Reset our array and counter
        self.sentence.clear()

        #Reset all strings
        self.__typed_string = u""
        self.__prefix_string = u""
        self.__postfix_string = u""
        self.__update()

    def property_activate(self, prop_name):
        print "PropertyActivate(%s)" % prop_name





