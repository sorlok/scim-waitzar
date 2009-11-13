# libwz 
# try this: 
#  import libwz
#  w = libwz.WordBuilder("whatever")
#  w.typeLetter('a')


# ####################
#Special information here
# ####################


# Make version info available
__ver_major__ = 1
__ver_minor__ = 0
__ver_patch__ = 0
__version__ = "%d.%d.%d" % (__ver_major__,__ver_minor__,__ver_patch__)


# Base error class for all exceptions our module throws.
class Error(Exception):
    """Base exception class for the libwz module."""
    pass
    

# ####################
#Normal class starts here
# ####################

import os
import unittest
import warnings
from libwz import _libwz as _lwz



#Base class for all libwz objects.
# Helps ensure that we are always accessing the c++ library consistently.
# Call _check_this() at the beginning of all sub-class methods referencing C++ code
class _LibWZObject(object):
    def __init__(self):
        self._this = None
        
    # Ensure that self._this points to something, else throw an error.
    def _check_this(self,msg=None):
        if self._this is None:
             if msg is None:
                msg = "%s unusable: the underlying C-library object has been freed."
                msg = msg % (self.__class__.__name__,)
        raise Error(msg)


        
# Mirror of WordBuilder.cpp, found in libwaitzar
class WordBuilder(_LibWZObject):

    #Constructor
    #    modelFile --> string
    #    userWordsFiles --> string[]
    #  Equivalent to: WordBuilder(const char* modelFile, std::vector<std::string> userWordsFiles)
    def __init__(self, modelFile, userWordsFiles):
        # Superclass initialisation
        _LibWZObject.__init__(self)
        self._this = _lwz.wordbuilder_init()


    #Destructor
    def __del__(self):
        if self._this is not None:
            _lwz.wordbuilder_free(self._this)
            self._this = None


    # Type a letter. Returns true if this caused a transition
    #    letter --> char
    # Equivalent to: bool typeLetter(char letter);
    def typeLetter(self, letter):
        self._check_this()

        #Marshell and call
        letter = LibWZChar(letter)
        val = _lwz.wordbuilder_typeletter(self._this, letter.encode())

        #Marshell return value
        if val == 0:
            return False
        else:
            return True


#    def suggest(self,word):
 #       """Suggest possible spellings for a word.
  #      
    #    This method tries to guess the correct spelling for a given
      #  word, returning the possibilities in a list.
     #   """
     #   self._check_this()
       # word = EnchantStr(word)
        #suggs = _e.dict_suggest(self._this,word.encode())
        #return [word.decode(w) for w in suggs]



##  Create a module-level default broker object, and make its important
##  methods available at the module level.
#_broker = Broker()
#request_dict = _broker.request_dict
#request_pwl_dict = _broker.request_pwl_dict
#dict_exists = _broker.dict_exists
#list_dicts = _broker.list_dicts
#list_languages = _broker.list_languages

