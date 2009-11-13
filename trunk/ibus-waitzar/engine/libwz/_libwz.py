# libwz 
# Low-level ctypes interface to libwaitzar. 
# Keep it simple. (Based on pyenchant)


import sys, os, os.path
from ctypes import *
from ctypes.util import find_library

# Locate and load the libwaitzar shared object.
lwz_path = find_library("waitzar")
if not lwz_path or os.path.isdir(lwz_path):
    lwz_path = find_library("libwaitzar")
if not lwz_path or os.path.isdir(lwz_path):
    raise ImportError("waitzar C++ library not found")
lwz = cdll.LoadLibrary(lwz)


# Define various callback function types
#t_broker_desc_func = CFUNCTYPE(None,c_char_p,c_char_p,c_char_p,c_void_p)
t_wordbuilder_desc_func = CFUNCTYPE(None,c_char_p,c_char_p,c_char_p,c_char_p,c_void_p)


# Simple typedefs for readability
#t_broker = c_void_p
t_wordbuilder = c_void_p


# Now we can define the types of each function we are going to use
wordbuilder_init = lwz.waitzar_WordBuilder_init
wordbuilder_init.argtypes = [c_]
wordbuilder_init.restype = t_wordbuilder

wordbuilder_free = lwz.waitzar_WordBuilder_free
wordbuilder_free.argtypes = [t_wordbuilder, c_char_p, c_size_t, POINTER(c_char_p)]
wordbuilder_free.restype = None


wordbuilder_typeletter1 = lwz.waitzar_typeLetter
wordbuilder_typeletter1.argtypes = [t_wordbuilder, c_char]
wordbuilder_typeletter1.restype = c_int
def wordbuilder_typeletter(wordbuilder, letter):
    return wordbuilder_typeletter1(wordbuilder, letter)

