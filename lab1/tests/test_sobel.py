
#!/usr/bin/env python3
import unittest

import cv2
import numpy as np

from ctypes import *
import ctypes

# ---- This file contains skeleton code for system, interface and unit test of the sobel algorithm
# ---- Its framed by the unittest python facility

# ---------- Data Structures ----
class SOBEL_ARGS( ctypes.Structure ):
    _fields_ = [("source_ptr", ctypes.POINTER(ctypes.c_uint8)),
                ("dest_ptr", ctypes.POINTER(ctypes.c_int16)),
                ("width", ctypes.c_int),
                ("height", ctypes.c_int)]

# ---------- Test Code ----------
class TestStringMethods(unittest.TestCase):
    

    # setup the test by opening a bpm file, converting it to grayscale and putting the result into a
    # sobel_algo compatible array
    def setUp( self ):
        im = cv2.imread('../data/image.png')
        im_gray = cv2.cvtColor( im, cv2.COLOR_RGB2GRAY)

        grad_x = cv2.Sobel(im_gray, cv2.CV_16S, 1, 0, ksize = 3, scale =1, delta=0, borderType=cv2.BORDER_DEFAULT)
        abs_grad_x = cv2.convertScaleAbs(grad_x)

        grad_y = cv2.Sobel(im_gray, cv2.CV_16S, 0, 1, ksize = 3, scale =1, delta=0, borderType=cv2.BORDER_DEFAULT)
        abs_grad_y = cv2.convertScaleAbs(grad_y)

        grad_sum = abs_grad_x + abs_grad_y
        grad_sum_abs = cv2.convertScaleAbs(grad_sum)

        self.im_gray = im_gray
        self.im_exp_x = abs_grad_x[1:-1,1:-1]
        self.im_exp_y = abs_grad_y[1:-1,1:-1]
        self.im_exp_sum = grad_sum_abs[1:-1,1:-1]
        self.c_lib = CDLL("../lib/sobel_algo.so")

        print(abs_grad_x)


    def test_sobel_x( self ):

        # Indent - print a test-identifying string
        print( "Test the sobel_x function" )

        image_height, image_width = self.im_gray.shape

        source = self.im_gray.copy().astype(np.uint8)
        source_p = source.ctypes.data_as(POINTER(c_uint8))

        dest = np.zeros([image_height, image_width], dtype='int16')
        dest_p = dest.ctypes.data_as(POINTER(c_int16))

        test_struct = SOBEL_ARGS( source_p, dest_p, image_width, image_height )
        test_struct_ptr = pointer( test_struct )
        self.c_lib.sobel_x( test_struct_ptr )

        ## Test
        np.testing.assert_array_equal(dest[1:-1, 1:-1], self.im_exp_x)

    def test_sobel_x_inline( self ):

        # Indent - print a test-identifying string
        print( "Test the sobel_x function" )

        image_height, image_width = self.im_gray.shape

        source = self.im_gray.copy().astype(np.uint8)
        source_p = source.ctypes.data_as(POINTER(c_uint8))

        dest = np.zeros([image_height, image_width], dtype='int16')
        dest_p = dest.ctypes.data_as(POINTER(c_int16))

        test_struct = SOBEL_ARGS( source_p, dest_p, image_width, image_height )
        test_struct_ptr = pointer( test_struct )
        self.c_lib.sobel_x_inline( test_struct_ptr )

        ## Test
        np.testing.assert_array_equal(dest[1:-1, 1:-1], self.im_exp_x)


    def test_sobel_y( self ):

        # Indent - print a test-identifying string
        print( "Test the sobel_y function" )

        image_height, image_width = self.im_gray.shape

        source = self.im_gray.copy().astype(np.uint8)
        source_p = source.ctypes.data_as(POINTER(c_uint8))

        dest = np.zeros([image_height, image_width], dtype='int16')
        dest_p = dest.ctypes.data_as(POINTER(c_int16))

        test_struct = SOBEL_ARGS( source_p, dest_p, image_width, image_height )
        test_struct_ptr = pointer( test_struct )
        self.c_lib.sobel_y( test_struct_ptr )

        # Test
        np.testing.assert_array_equal(dest[1:-1, 1:-1], self.im_exp_y)

    def test_sobel_y_inline( self ):

        # Indent - print a test-identifying string
        print( "Test the sobel_y function" )

        image_height, image_width = self.im_gray.shape

        source = self.im_gray.copy().astype(np.uint8)
        source_p = source.ctypes.data_as(POINTER(c_uint8))

        dest = np.zeros([image_height, image_width], dtype='int16')
        dest_p = dest.ctypes.data_as(POINTER(c_int16))

        test_struct = SOBEL_ARGS( source_p, dest_p, image_width, image_height )
        test_struct_ptr = pointer( test_struct )
        self.c_lib.sobel_y_inline( test_struct_ptr )

        # Test
        np.testing.assert_array_equal(dest[1:-1, 1:-1], self.im_exp_y)

    
    def test_sobel_mac_x_filter(self):
        print("Test the sobel_mac, x filter")
        
        image = np.ones((3, 3), dtype='uint8').astype(np.uint8)
        image_p = image.ctypes.data_as(POINTER(c_uint8))
        image_height, image_width = image.shape

        x_filter = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]], dtype='int8').astype(np.int8)
        filter_p = x_filter.ctypes.data_as(POINTER(c_int8))

        # Test
        ret = self.c_lib.sobel_mac(image_p, 1, 1, filter_p, image_width, image_height)
        self.assertEqual(ret, 0)

    
    def test_sobel_mac_y_filter(self):
        print("Test the sobel_mac, y filter")

        image = np.ones((3, 3), dtype='uint8').astype(np.uint8)
        image_p = image.ctypes.data_as(POINTER(c_uint8))
        image_height, image_width = image.shape

        y_filter = np.array([[-1, -2, -1], [0, 0, 0], [1, 2, 1]], dtype='int8').astype(np.int8)
        filter_p = y_filter.ctypes.data_as(POINTER(c_int8))

        # Test
        res = self.c_lib.sobel_mac(image_p, 1, 1, filter_p, image_width, image_height)
        self.assertEqual(res, 0)
        
    def test_sobel_sum(self):
        print("Test the sobel_sum")

        image_height, image_width = self.im_gray.shape

        # Sobel x
        source_x = self.im_gray.copy().astype(np.uint8)
        source_x_p = source_x.ctypes.data_as(POINTER(c_uint8))

        dest_x = np.zeros([image_height, image_width], dtype='int16')
        dest_x_p = dest_x.ctypes.data_as(POINTER(c_int16))

        test_struct_x = SOBEL_ARGS( source_x_p, dest_x_p, image_width, image_height )
        test_struct_x_ptr = pointer( test_struct_x )
        self.c_lib.sobel_x( test_struct_x_ptr )

        # Sobel y
        source_y = self.im_gray.copy().astype(np.uint8)
        source_y_p = source_y.ctypes.data_as(POINTER(c_uint8))

        dest_y = np.zeros([image_height, image_width], dtype='int16')
        dest_y_p = dest_y.ctypes.data_as(POINTER(c_int16))

        test_struct_y = SOBEL_ARGS( source_y_p, dest_y_p, image_width, image_height )
        test_struct_y_ptr = pointer( test_struct_y )
        self.c_lib.sobel_y( test_struct_y_ptr )

        # Sobel sum
        dest_sum = np.zeros([image_height, image_width, 3], dtype='uint8')
        dest_sum_p = dest_sum.ctypes.data_as(POINTER(c_uint8))

        self.c_lib.sobel_sum(dest_x_p, dest_y_p, dest_sum_p, image_height, image_width)

        print("\n")
        print(dest_sum[:,:,0])
        print("\n")

        np.testing.assert_array_equal(dest_sum[1:-1, 1:-1, 0], self.im_exp_sum)
    

if __name__ == '__main__':
    unittest.main()

