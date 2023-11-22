import ctypes
from ctypes import *

# setup the library file
so_file = "../lib/contour.so"
cont = CDLL(so_file)

#define the expected argument types for the c-function
img_ptr = Im.ctypes.data_as(ctypes.POINTER(ctypes.c_uint8))
xx_ptr = xx.ctypes.data_as(ctypes.POINTER(ctypes.c_uint8))
yy_ptr = yy.ctypes.data_as(ctypes.POINTER(ctypes.c_uint8))

# call the c-code
cont.get_contour( img_ptr, xx_ptr, yy_ptr )