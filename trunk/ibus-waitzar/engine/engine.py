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

        self.__is_invalidate = False
        self.__preedit_string = u""
        self.__lookup_table = ibus.LookupTable()
        self.__prop_list = ibus.PropList()
        self.__prop_list.append(ibus.Property(u"test", icon = u"ibus-locale"))
        	
        append_log("Engine created")
        
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

    def process_key_event_internal(self, keyval, keycode, state):
        # ignore key release events
        is_press = ((state & modifier.RELEASE_MASK) == 0)
        if not is_press:
            return False

        append_log('key press: ' + str(keyval) + ' , ' + str(keycode) + ' , ' + str(state))

        if self.__preedit_string:
            if keyval == keysyms.Return:
                self.__commit_string(self.__preedit_string)
                return True
            elif keyval == keysyms.Escape:
                self.__preedit_string = u""
                self.__update()
                return True
            elif keyval == keysyms.BackSpace:
                self.__preedit_string = self.__preedit_string[:-1]
                self.__invalidate()
                return True
            elif keyval == keysyms.space:
                if self.__lookup_table.get_number_of_candidates() > 0:
                    self.__commit_string(self.__lookup_table.get_current_candidate().text)
                else:
                    self.__commit_string(self.__preedit_string)
                return False
            elif keyval >= keysyms._1 and keyval <= keysyms._9:
                index = keyval - keysyms._1
                candidates = self.__lookup_table.get_canidates_in_current_page()
                if index >= len(candidates):
                    return False
                candidate = candidates[index].text
                self.__commit_string(candidate)
                return True
            elif keyval == keysyms.Page_Up or keyval == keysyms.KP_Page_Up:
                self.page_up()
                return True
            elif keyval == keysyms.Page_Down or keyval == keysyms.KP_Page_Down:
                self.page_down()
                return True
            elif keyval == keysyms.Up:
                self.cursor_up()
                return True
            elif keyval == keysyms.Down:
                self.cursor_down()
                return True
            elif keyval == keysyms.Left or keyval == keysyms.Right:
                return True
        if keyval in xrange(keysyms.a, keysyms.z + 1) or \
            keyval in xrange(keysyms.A, keysyms.Z + 1):
            if state & (modifier.CONTROL_MASK | modifier.ALT_MASK) == 0:
                self.__preedit_string += unichr(keyval)
                self.__invalidate()
                return True
        else:
            if keyval < 128 and self.__preedit_string:
                self.__commit_string(self.__preedit_string)

        return False

    def __invalidate(self):
        if self.__is_invalidate:
            return
        self.__is_invalidate = True
        gobject.idle_add(self.__update, priority = gobject.PRIORITY_LOW)


    def page_up(self):
        if self.__lookup_table.page_up():
            self.page_up_lookup_table()
            return True
        return False

    def page_down(self):
        if self.__lookup_table.page_down():
            self.page_down_lookup_table()
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
        self.__preedit_string = u""
        self.__update()

    def __update(self):
        preedit_len = len(self.__preedit_string)
        attrs = ibus.AttrList()
        self.__lookup_table.clean()
        if preedit_len > 0:
            attrs.append(ibus.AttributeForeground(0xff0000, 0, preedit_len))
            self.__lookup_table.append_candidate(ibus.Text("hello"))
        self.update_auxiliary_text(ibus.Text(self.__preedit_string, attrs), preedit_len > 0)
        attrs.append(ibus.AttributeUnderline(pango.UNDERLINE_SINGLE, 0, preedit_len))
        self.update_preedit_text(ibus.Text(self.__preedit_string, attrs), preedit_len, preedit_len > 0)
        self.__update_lookup_table()
        self.__is_invalidate = False

    def __update_lookup_table(self):
        visible = self.__lookup_table.get_number_of_candidates() > 0
        self.update_lookup_table(self.__lookup_table, visible)


    def focus_in(self):
        self.register_properties(self.__prop_list)

    def focus_out(self):
        pass

    def reset(self):
        pass

    def property_activate(self, prop_name):
        print "PropertyActivate(%s)" % prop_name

