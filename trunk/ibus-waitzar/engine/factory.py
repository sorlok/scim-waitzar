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

import ibus
import engine
import os

from gettext import dgettext
_  = lambda a : dgettext("ibus-waitzar", a)
N_ = lambda a : a


class EngineFactory(ibus.EngineFactoryBase):
    FACTORY_PATH = "/com/redhat/IBus/engines/WaitZar/Factory"
    ENGINE_PATH = "/com/redhat/IBus/engines/WaitZar/Engine"
    NAME = _("WaitZar")
    LANG = "my"
    ICON = os.getenv("IBUS_WAITZAR_LOCATION") + "/icons/wz.svg"
    AUTHORS = "Seth N. Hetu <sorlok_reaves@yahoo.com>"
    CREDITS = "GPLv3"

    def __init__(self, bus):
        print('Factory called to init Engine')
        
        self.__bus = bus
        #engine.Engine.CONFIG_RELOADED(bus)
        super(EngineFactory, self).__init__(bus)

        self.__id = 0
        self.__config = self.__bus.get_config()

        self.__config.connect("reloaded", self.__config_reloaded_cb)
        self.__config.connect("value-changed", self.__config_value_changed_cb)
        
        print('Factory finished with initEngine')

    def create_engine(self, engine_name):
        print('Factory called to create Engine: ' + engine_name )
        if engine_name == "waitzar":
            self.__id += 1
            print('Factory returning created engine: ' + self.ENGINE_PATH )
            return engine.Engine(self.__bus, "%s/%d" % (self.ENGINE_PATH, self.__id))

        print('Factory delegating call to create_engine')
        return super(EngineFactory, self).create_engine(engine_name)

    def __config_reloaded_cb(self, config):
        #engine.Engine.CONFIG_RELOADED(self.__bus)
        pass

    def __config_value_changed_cb(self, config, section, name, value):
        #engine.Engine.CONFIG_VALUE_CHANGED(self.__bus, section, name, value)
        pass


