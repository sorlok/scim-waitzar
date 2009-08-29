#
# ibus-waitzar - The WaitZar engine for IBus
#     Copyright (c) 2009 Seth N. Hetu <seth.hetu@gmail.com>
#
# waitzar.py file based on ibus-pinyin's file:
#     Copyright (c) 2007-2008 Huang Peng <shawn.p.huang@gmail.com>
#     Modified under the terms of the GNU General Public License version 3.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or(at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#Our public (library) classes
__all__ = (
    "WaitZarEngine",
)

#Import required functionality
import ibus
import gobject
import os
import signal
import os.path as path
from ibus import keysyms
from ibus import modifier
from ibus import ascii

IBUS_WAITZAR_LOCATION = os.getenv("IBUS_WAITZAR_LOCATION")
LIBEXECDIR = os.getenv("LIBEXECDIR")


class WaitZarEngine(ibus.EngineBase):
    
    #Perform any one-time inits here
    #(later...)

    #Initializer, guaranteed to be called on object creation (by ibus)
    def __init__(self, conn, object_path):
        #Delegate most of the grunt work to our superclass
        super(WaitZarEngine, self).__init__(conn, object_path)

        #Perform one-time engine inits
        #self.__need_update = False
        #self.reset()
        #(later...)

        #Initialize properties
        self.__prop_list = ibus.PropList()
        
        #(later...)
        #self.__status_property = ibus.Property(u"status")
        #self.__prop_list.append(self.__status_property)
        #self.__letter_property = ibus.Property(u"full_letter")
        #self.__prop_list.append(self.__letter_property)
        #self.__punct_property = ibus.Property(u"full_punct")
        #self.__prop_list.append(self.__punct_property)
        #self.__setup_property = ibus.Property(u"setup")
        #self.__setup_property.tooltip = _(u"Configure PinYin")
        #self.__prop_list.append(self.__setup_property)


    #Later...
    def __refresh_properties(self):
        #if self.__mode == 1: # refresh mode
        #    self.__status_property.icon = path.join(IBUS_PINYIN_LOCATION, "icons", "chinese.svg")
        #    self.__status_property.label = _(u"CN")
        #    self.__status_property.tooltip = _(u"Switch to English mode")
        #else:
        #    self.__status_property.icon = path.join(IBUS_PINYIN_LOCATION, "icons", "english.svg")
        #    self.__status_property.label = _(u"EN")
        #    self.__status_property.tooltip = _(u"Switch to Chinese mode")


        #properties =(
        #    self.__status_property,
        #    self.__letter_property,
        #    self.__punct_property,
        #    )

        #for prop in properties:
        #    self.update_property(prop)



    #Later...
    def __update_candidates(self):
        #while count < len(pinyin_list):
        #    candidate = self.__get_a_candidate(pinyin_list[count:])
        #    self.__preedit_phrases.append(candidate)
        #    count += candidate[pysqlitedb.YLEN]

    #Later...
    def __update_ui(self):
        #if len(self.__candidates) == 0:
        #    self.hide_lookup_table()
        #else:
        #    self.__lookup_table.clean()
        #    candidates = self.__candidates[:]
        #    if len(self.__preedit_phrases) != 1: # we need display the automatically created new phrase
        #        attrs = ibus.AttrList ()
        #        preedit_string = self.__preedit_phrases.get_string()
        #        attr = ibus.AttributeForeground(PinYinEngine.__new_phrase_color, 0, len(preedit_string))
        #        attrs.append (attr)
        #        self.__lookup_table.append_candidate(ibus.Text(preedit_string, attrs))

        #committed_string = self.__committed_phrases.get_string()
        #invalid_pinyin = self.__user_input.get_invalid_string()
        #preedit_string = " ".join([committed_string, self.__preedit_phrases.get_string(), invalid_pinyin])
        #preedit_string = preedit_string.strip()

        #if preedit_string:
        #    self.update_preedit(preedit_string, None, len(preedit_string), True)
        #else:
        #    self.hide_preedit_text()


    #def __invalidate(self):
    #    if self.__need_update:
    #        return
    #    self.__need_update = True

    #def __update(self):
    #    if self.__need_update:
    #        self.__update_candidates()
    #        self.__update_ui()
    #        self.__need_update = False


    #def __get_candidates(self, pinyin_list):
    #    candidates = []

    #    for i in range(len(pinyin_list), 0, -1):
    #        candidates += self.__pydb.select_words_by_pinyin_list(pinyin_list[:i], PinYinEngine.__fuzzy_pinyin)
    #    if not PinYinEngine.__gbk:
    #        candidates = filter(self.__is_gb2312, candidates)
    #    return candidates

    #def __append_char(self, char):
    #    self.__user_input.append(char)
    #    self.__committed_phrases.clean()
    #    self.__invalidate()

    #    return True

    #def __pop_char(self):
    #    if len(self.__user_input) == 0:
    #        return False
    #    if len(self.__committed_phrases) != 0:
    #        self.__committed_phrases.pop()
    #    else:
    #        self.__user_input.pop()
    #    self.__invalidate()
    #    return True


    #def __internal_process_key_event(self, key):
        # When CapsLock is lock, we ignore all key events
        #if key.mask & modifier.LOCK_MASK:
        #    if self.__user_input:
        #        self.reset()
        #    return False

        # ignore NumLock mask
        #key.mask &= ~modifier.MOD2_MASK

        #if self.__is_input_english():
        #    return self.__english_mode_process_key_event(key)
        #else:
        #    if self.__temp_english_mode:
        #        return self.__temp_english_mode_process_key_event(key)
        #    elif self.__i_mode:
        #        return self.__i_mode_process_key_event(key)
        #    else:
        #        return self.__chinese_mode_process_key_event(key)


    def __i_mode_process_key_event(self, key):
        #if key.code in(keysyms.Return, keysyms.KP_Enter):
        #    commit_string = u"".join(self.__user_input.get_chars())
        #    self.commit_string(commit_string)
        #    return True
        #elif key.code >= keysyms._1 and key.code <= keysyms._9:
        #    if not self.__candidates:
        #        return True
        #    index = key.code - keysyms._1
        #    if index >= PinYinEngine.__page_size:
        #        return True
        #    index += self.__lookup_table.get_current_page_start()
        #    if index >= len(self.__candidates):
        #        return True
        #    self.commit_string(self.__candidates[index])
        #    return True

        #More key processing... later...

        #self.__user_input.append(unichr(key.code))
        #self.__invalidate()

        #return True



    def __commit_candidate(self, i):
        #if i == 0:
        #    for phrase in self.__preedit_phrases.get_phrases():
        #        self.__committed_phrases.append(phrase)
        #    return True

        #if i >=1 and i <= len(self.__special_candidates):
        #    self.__committed_special_phrase = self.__special_candidates [i - 1]
        #    return True

        #if len(self.__preedit_phrases) != 1:
        #    i -= 1

        #i -= len(self.__special_candidates)

        #if i >= len(self.__candidates):
        #    return False

        #self.__committed_phrases.append( self.__candidates[i])
        #pinyin_list = self.__user_input.get_pinyin_list()

        #if self.__committed_phrases.length_of_chars() == len(pinyin_list):
        #    return True

        #self.__invalidate()

        #return False

    def __remove_candidate(self, i):
        #if i >= 1:
        #    i -= len(self.__special_candidates)

        #if len(self.__preedit_phrases) != 1:
        #    i -= 1

        #if i >= len(self.__candidates) or i < 0:
        #    return False

        #if self.__candidates[i][pysqlitedb.FREQ] != None: # This phrase was not create by user.
        #    return False

        #candidate = self.__candidates.pop(i)
        #try:
        #    self.__pydb.remove_phrase(candidate)
        #except:
        #    print "Can not remove phrase from db"
        #self.__invalidate()

        #return True

    def __start_setup(self):
        #if PinYinEngine.__setup_pid != 0:
        #    pid, state = os.waitpid(PinYinEngine.__setup_pid, os.P_NOWAIT)
        #    if pid != PinYinEngine.__setup_pid:
        #        os.kill(PinYinEngine.__setup_pid, signal.SIGUSR1)
        #        return
        #    PinYinEngine.__setup_pid = 0
        #setup_cmd = path.join(LIBEXECDIR, "ibus-setup-pinyin")
        #PinYinEngine.__setup_pid = os.spawnl(os.P_NOWAIT, setup_cmd, "ibus-setup-pinyin")


    def process_key_event(self, keyval, keycode, state):
        #key = KeyEvent(keyval, state & modifier.RELEASE_MASK == 0, state)
        #result = self.__internal_process_key_event(key)
        #self.__update()
        #self.__prev_key = key
        #return result

    def commit_string(self, string, need_update = True):
        #self.__temp_english_mode = False
        #self.__i_mode = False
        #self.__candidates = []
        #self.__english_candidates = []
        #self.__cursor = 0
        #self.__user_input.clean()
        #self.__preedit_string = u""
        #self.__committed_phrases.clean()
        #self.__committed_special_phrase = u""
        #self.__need_update = True
        #self.__update()
        #super(PinYinEngine,self).commit_text(ibus.Text(string))
        #self.__prev_char = string[-1]

    def update_preedit(self, preedit_string, preedit_attrs, cursor_pos, visible):
        #if preedit_attrs == None:
        #    preedit_attrs = ibus.AttrList()
        #    attr = ibus.AttributeUnderline(ibus.ATTR_UNDERLINE_SINGLE, 0, len(preedit_string))
        #    preedit_attrs.append(attr)

        #super(PinYinEngine, self).update_preedit_text(ibus.Text(preedit_string, preedit_attrs), cursor_pos, visible)

    def update_aux_string(self, aux_string, aux_attrs, visible):
        #super(PinYinEngine,self).update_auxiliary_text(ibus.Text(aux_string, aux_attrs), visible)

    def page_up(self):
        #if self.__lookup_table.page_up():
        #    self.update_lookup_table(self.__lookup_table, True, True)
        #    return True

        #return True


    def focus_in(self):
        #self.register_properties(self.__prop_list)
        #self.__refresh_properties()
        #if PinYinEngine.__shuangpin:
        #    self.__py_parser = pyparser.ShuangPinParser(PinYinEngine.__shuangpin_schema)
        #else:
        #    self.__py_parser = pyparser.PinYinParser()
        #self.__lookup_table.set_page_size(PinYinEngine.__page_size)
        #self.__user_input.set_parser(self.__py_parser)
        #self.__user_input.set_gbk(PinYinEngine.__gbk)
        #self.__user_input.set_auto_correct(PinYinEngine.__auto_correct)
        #self.__invalidate()

    def focus_out(self):
        #self.reset()

    def enable(self):
        #self.focus_in()

    def property_activate(self, prop_name, prop_state = ibus.PROP_STATE_UNCHECKED):
        #if prop_name == "status":
        #    self.__change_mode()
        #elif prop_name == "full_letter":
        #    self.__full_width_letter [self.__mode] = not self.__full_width_letter [self.__mode]
        #    self.__refresh_properties()
        #elif prop_name == "full_punct":
        #    self.__full_width_punct [self.__mode] = not self.__full_width_punct [self.__mode]
        #    self.__refresh_properties()
        #elif prop_name == "setup":
        #    self.__start_setup()

    def process_helper_event(self, helper_uuid, trans):
        #IMEngine.process_helper_event(self, helper_uuid, trans)

    def update_client_capabilities(self, cap):
        #IMEngine.update_client_capabilities(self, cap)

    @classmethod
    def CONFIG_VALUE_CHANGED(cls, bus, section, name, value):
        #config = bus.get_config()

        #if section != "engine/PinYin":
        #    return
        #if name == "ShuangPinSchema":
        #    PinYinEngine.__shuangpin_schema = \
        #        config.get_value("engine/PinYin", "ShuangPinSchema", "MSPY")
        #    if PinYinEngine.__shuangpin_schema not in pydict.SHUANGPIN_SCHEMAS:
        #        PinYinEngine.__shuangpin_schema = "MSPY"
        #elif name == "FuzzyPinYin":
        #    PinYinEngine.__fuzzy_pinyin = \
        #        config.get_value("engine/PinYin", "FuzzyPinYin", False)
        #....more....
        #else:
        #    print "Unknow name(%s)" % name

    @classmethod
    def CONFIG_RELOADED(cls, bus):
        #config = bus.get_config()

        #PinYinEngine.__shuangpin_schema = \
        #    config.get_value("engine/PinYin", "ShuangPinSchema", "MSPY")
        #if PinYinEngine.__shuangpin_schema not in pydict.SHUANGPIN_SCHEMAS:
        #    PinYinEngine.__shuangpin_schema = "MSPY"

        #PinYinEngine.__fuzzy_pinyin = \
        #    config.get_value("engine/PinYin", "FuzzyPinYin", False)
        #PinYinEngine.__auto_correct = \
        #    config.get_value("engine/PinYin", "AutoCorrect", True)
        #more....

#Lots of helper sub-classes, etc.
#class KeyEvent:
    #def __init__(self, keyval, is_press, state):
    #    self.code = keyval
    #    self.mask = state
    #    if not is_press:
    #        self.mask |= modifier.RELEASE_MASK
    #def __str__(self):
    #    return "%s 0x%08x" % (keysyms.keycode_to_name(self.code), self.mask)

