#!/usr/bin/env python3
import unittest

import cv2

from ctypes import *
import ctypes

# ---- This file contains skeleton code for system, interface and unit test of the sobel algorithm
# ---- Its framed by the unittest python facility

# ---------- Data Structures ----
class SOBEL_ARGS( ctypes.Structure ):
    _fields_ = [("source_ptr", ctypes.POINTER(ctypes.c_uint8)),
                ("dest_ptr", ctypes.POINTER(ctypes.c_int)),
                ("width", ctypes.c_int),
                ("height", ctypes.c_int)]

# ---------- Test Code ----------
class TestStringMethods(unittest.TestCase):

    # setup the test by opening a bpm file, converting it to grayscale and putting the result into a
    # sobel_algo compatible array
    def setUp( self ):
        im = cv2.imread('../data/image.png')
        gray = cv2.cvtColor( im, cv2.COLOR_RGB2GRAY)
        grad_x = cv2.Sobel(gray, cv2.CV_16S, 0, 1, ksize = 3, scale =1, delta=0, borderType=cv2.BORDER_DEFAULT)
        abs_grad_x = cv2.convertScaleAbs(grad_x)
        print(abs_grad_x)


    def test_sobel_x( self ):

        # Indent - print a test-identifying string
        print( "Test the sobel_x function" )

        # Prep - identify the library file with the code that needs testing
        so_file = "../lib/sobel_algo.so"
        sobel = CDLL(so_file)

        # Test - fill the structure with values
        #      - instantiate a pointer to the struct
        #      - pass the pointer to and call the sobel_x function
        test_struct = SOBEL_ARGS( None, None, 0, 0 )
        test_struct_ptr = pointer( test_struct )
        sobel.sobel_x( test_struct_ptr )

if __name__ == '__main__':
    unittest.main()

