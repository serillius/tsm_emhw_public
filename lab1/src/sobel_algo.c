/* ------------------------------------------------------------
                    ____     ________
                   /  _/__  / __/ __/    
                  _/ // _ \/ _/_\ \        
                 /___/_//_/___/___/            

    Institute of Embedded Systems
    Zurich University of Applied Sciences
    8401 Winterthur, Switzerland


    Project:  TSM_EmbHardw
    File:     sobel_algo.c

    Purpose:  example code for the labs
              
    Remarks:

    Author:   donn

    Date:     KW9 2023
    Version:  0.9 FS23

    $Id$

   ------------------------------------------------------------ */ 
   
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <sched.h>
#include <errno.h>
#include <sys/sysinfo.h>
#include <sys/syscall.h>

#include <assert.h>

#include "../include/sobel_algo.h"

     

int16_t sobel_mac( const uint8_t *pixels, int x, int y, const int8_t *filter, 
                 int width ) {

    int dy, dx;
    short result = 0;
    
    for ( dy = -1; dy < 2; dy++) {
        for (dx = -1; dx < 2; dx++) {
            result += filter[(dy+1) * 3 + (dx+1)] * \
            pixels[(y+dy) * width + (x+dx)];         
        }
    }
                     
    return result;             
}         
                 
const int8_t gx_array[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};

void sobel_x ( const sobel_args *par )
{
    int x, y;

    for ( y= 1; y < (par->height -1) ; y++) {
        for (x = 1; x < (par->width-1) ; x ++) {
            par->dest[y*par->width + x] = abs(sobel_mac(par->source, x, y, gx_array, par->width));
             
        }
    }
    
}


const int8_t gy_array[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};

void sobel_y ( const sobel_args *par )
{
    int x, y;
     
    for ( y= 1; y < (par->height -1) ; y++) {
        for (x = 1; x < par->width-1; x ++) {
            par->dest[y*par->width + x] = abs(sobel_mac(par->source, x, y, gy_array, par->width));
        }
    }
}


void sobel_sum ( const int16_t *source_x, const int16_t *source_y, uint8_t *dest, int height, int width) {

    int sobel_sum = 0;

    int i = 0, j = 0;
    
    for ( ; i < (height-2)*(width-2); i++) {
        sobel_sum = source_x[i] + source_y[i];

        // make it png ready
        dest[j++] = (uint8_t)sobel_sum;
        dest[j++] = (uint8_t)sobel_sum;
        dest[j++] = (uint8_t)sobel_sum;
    }
}
