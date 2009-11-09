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

import os
import sys
import getopt
import ibus
import factory
import gobject
import datetime

N_ = lambda x: x
N_("Hello")

class IMApp:
    def __init__(self, exec_by_ibus):
        print("IM App initialzation begin")
        self.__component = ibus.Component("org.freedesktop.IBus.WaitZar",
                                          "WaitZar Component",
                                          "1.0.0",
                                          "GPLv3",
                                          "Seth N. Hetu <sorlok_reaves@yahoo.com>")
        self.__component.add_engine("waitzar",
                                    "waitzar",
                                    "Myanmar WaitZar",
                                    "my",
                                    "GPLv3",
                                    "Seth N. Hetu <sorlok_reaves@yahoo.com>",
                                    "",
                                    "en")
        self.__mainloop = gobject.MainLoop()
        self.__bus = ibus.Bus() 
        self.__factory = factory.EngineFactory(self.__bus)
        if exec_by_ibus:
            self.__bus.request_name("org.freedesktop.IBus.WaitZar", 0)
        else:
            self.__bus.register_component(self.__component)
        
        print("IM App initialzation end")

    def run(self):
        self.__mainloop.run()

    def __bus_destroy_cb(self, _ibus):
        self.__mainloop.quit()


def launch_engine(exec_by_ibus):
    IMApp(exec_by_ibus).run()

def print_help(out, v = 0):
    print >> out, "-h, --help             show this message."
    print >> out, "-d, --daemonize        daemonize ibus"
    print >> out, "-i, --ibus             executed by ibus."
    sys.exit(v)

def main():
    #Reset our log file
    print("WaitZar initialized")
    print("Time: " + str(datetime.datetime.now()) )

    #Get command-line arguments
    exec_by_ibus = False
    daemonize = False
    shortopt = "hdi"
    longopt = ["help", "daemonize", "ibus"]
    try:
        opts, args = getopt.getopt(sys.argv[1:], shortopt, longopt)
    except getopt.GetoptError, err:
        print_help(sys.stderr, 1)

    for o, a in opts:
        if o in("-h", "--help"):
            print_help(sys.stdout)
        elif o in ("-d", "--daemonize"):
            daemonize = True
        elif o in ("-i", "--ibus"):
            exec_by_ibus = True
        else:
            print >> sys.stderr, "Unknown argument: %s" % o
            print_help(sys.stderr, 1)

    if daemonize:
        if os.fork():
            sys.exit()

    print("Launching with options: " + str(opts) )
    launch_engine(exec_by_ibus)

if __name__ == "__main__":
    main()
