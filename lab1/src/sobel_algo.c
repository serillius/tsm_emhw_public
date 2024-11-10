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

#include <emmintrin.h>
#include <tmmintrin.h>

#include "../include/sobel_algo.h"


uint8_t abs_max_uint8(const int16_t src)
{
    uint16_t src_abs = abs(src);
    return (uint8_t)((src_abs <= (uint32_t)255) ? src_abs : 255);
}

int16_t sobel_mac( const uint8_t *pixels, int x, int y, const int8_t *filter, 
                 int width ) {

    int dy, dx;
    short result = 0;

    // result += filter[0] * pixels[(y - 1) * width + (x - 1)];
    // result += filter[1] * pixels[(y - 1) * width + (x    )];
    // result += filter[2] * pixels[(y - 1) * width + (x + 1)];

    // result += filter[3] * pixels[(y    ) * width + (x - 1)];
    // result += filter[4] * pixels[(y    ) * width + (x    )];
    // result += filter[5] * pixels[(y    ) * width + (x + 1)];

    // result += filter[6] * pixels[(y + 1) * width + (x - 1)];
    // result += filter[7] * pixels[(y + 1) * width + (x    )];
    // result += filter[8] * pixels[(y + 1) * width + (x + 1)];
    
    for ( dy = -1; dy < 2; dy++) {
        for (dx = -1; dx < 2; dx++) {
            result += filter[(dy+1) * 3 + (dx+1)] * \
            pixels[(y+dy) * width + (x+dx)];         
        }  
    }
                     
    return result;             
}  

uint8_t sobel_mac_simd( const uint8_t *pixels, const int8_t *filter) {   
    __m128i result;

    __m128i vec_pixels = _mm_loadu_si128((const __m128i*)pixels);
    __m128i vec_filter = _mm_loadu_si128((const __m128i*)filter);

    __m128i mask = _mm_set_epi8(0, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
    
    vec_pixels = _mm_and_si128(vec_pixels, mask);
    vec_filter = _mm_and_si128(vec_filter, mask);

    result = _mm_maddubs_epi16(vec_pixels, vec_filter);

    result = _mm_hadd_epi16 (result, result);
    result = _mm_hadd_epi16 (result, result);
    result = _mm_hadd_epi16 (result, result);
    result = _mm_packus_epi16 (result, result);

    int16_t res;
    _mm_storeu_si16(&res, result);

    return (uint8_t) res;
}

int16_t sobel_mac_x( const uint8_t *pixels, int x, int y, const int8_t *filter, 
                 int width ) {

    int dy, dx;
    short result = 0;

    result += filter[0] * pixels[(y - 1) * width + (x - 1)];
    result += filter[2] * pixels[(y - 1) * width + (x + 1)];
    result += filter[3] * pixels[(y    ) * width + (x - 1)];
    result += filter[5] * pixels[(y    ) * width + (x + 1)];
    result += filter[6] * pixels[(y + 1) * width + (x - 1)];
    result += filter[8] * pixels[(y + 1) * width + (x + 1)];
                     
    return result;             
}         

int16_t sobel_mac_y( const uint8_t *pixels, int x, int y, const int8_t *filter, 
                 int width ) {

    int dy, dx;
    short result = 0;

    result += filter[0] * pixels[(y - 1) * width + (x - 1)];
    result += filter[1] * pixels[(y - 1) * width + (x    )];
    result += filter[2] * pixels[(y - 1) * width + (x + 1)];

    result += filter[6] * pixels[(y + 1) * width + (x - 1)];
    result += filter[7] * pixels[(y + 1) * width + (x    )];
    result += filter[8] * pixels[(y + 1) * width + (x + 1)];
                     
    return result;             
}         
                 
const int8_t gx_array[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};

void sobel_x ( const sobel_args *par )
{
    int x, y;

    for ( y= 1; y < (par->height -1) ; y++) {
        for (x = 1; x < (par->width-1) ; x ++) {
            par->dest[y*par->width + x] = abs_max_uint8(sobel_mac_x(par->source, x, y, gx_array, par->width));
             
        }
    }
    
}

void sobel_x_smid ( const sobel_args *par )
{
    int x, y;

    for ( y= 0; y < (par->height - 2) ; y++) {
        for (x = 0; x < (par->width - 2) ; x++) {

            uint8_t inter[9] = {par->source[par->width * (y    ) + (x    )],
                                par->source[par->width * (y    ) + (x + 1)],
                                par->source[par->width * (y    ) + (x + 2)],
                                
                                par->source[par->width * (y + 1) + (x    )],
                                par->source[par->width * (y + 1) + (x + 1)],
                                par->source[par->width * (y + 1) + (x + 2)],
                                
                                par->source[par->width * (y + 2) + (x    )],
                                par->source[par->width * (y + 2) + (x + 1)],
                                par->source[par->width * (y + 2) + (x + 2)]};
            
            par->dest[(y + 1) * par->width + x + 1] = sobel_mac_simd(inter, gx_array);
             
        }
    }
    
}

void sobel_x_inline ( const sobel_args *par )
{
    int x, y;

    for ( y= 1; y < (par->height -1) ; y++) {
        for (x = 1; x < (par->width-1) ; x ++) {
            par->dest[y*par->width + x] += -1 * par->source[(y - 1) * par->width + (x - 1)];
            par->dest[y*par->width + x] += 1  * par->source[(y - 1) * par->width + (x + 1)];

            par->dest[y*par->width + x] += -2 * par->source[(y    ) * par->width + (x - 1)];
            par->dest[y*par->width + x] += 2  * par->source[(y    ) * par->width + (x + 1)];

            par->dest[y*par->width + x] += -1 * par->source[(y + 1) * par->width + (x - 1)];
            par->dest[y*par->width + x] += 1  * par->source[(y + 1) * par->width + (x + 1)];
            
            par->dest[y*par->width + x] = abs_max_uint8(par->dest[y*par->width + x]);
             
        }
    }
    
}

const int8_t gy_array[] = {1, 2, 1, 0, 0, 0, -1, -2, -1};

void sobel_y_inline ( const sobel_args *par )
{
    int x, y;
     
    for ( y= 1; y < (par->height -1) ; y++) {
        for (x = 1; x < par->width-1; x ++) {
            par->dest[y*par->width + x] += 1 * par->source[(y - 1) * par->width + (x - 1)];
            par->dest[y*par->width + x] += 2 * par->source[(y - 1) * par->width + (x    )];
            par->dest[y*par->width + x] += 1 * par->source[(y - 1) * par->width + (x + 1)];

            par->dest[y*par->width + x] += -1 * par->source[(y + 1) * par->width + (x - 1)];
            par->dest[y*par->width + x] += -2 * par->source[(y + 1) * par->width + (x    )];
            par->dest[y*par->width + x] += -1 * par->source[(y + 1) * par->width + (x + 1)];

            par->dest[y*par->width + x] = abs_max_uint8(par->dest[y*par->width + x]); 
        }
    }
}

void sobel_y ( const sobel_args *par )
{
    int x, y;
     
    for ( y= 1; y < (par->height -1) ; y++) {
        for (x = 1; x < par->width-1; x ++) {
            par->dest[y*par->width + x] = abs_max_uint8(sobel_mac_y(par->source, x, y, gy_array, par->width));
        }
    }
}

void sobel_y_smid ( const sobel_args *par )
{
    int x, y;

    for ( y= 0; y < (par->height - 2) ; y++) {
        for (x = 0; x < (par->width - 2) ; x++) {

            uint8_t inter[9] = {par->source[par->width * (y    ) + (x    )],
                                par->source[par->width * (y    ) + (x + 1)],
                                par->source[par->width * (y    ) + (x + 2)],
                                
                                par->source[par->width * (y + 1) + (x    )],
                                par->source[par->width * (y + 1) + (x + 1)],
                                par->source[par->width * (y + 1) + (x + 2)],
                                
                                par->source[par->width * (y + 2) + (x    )],
                                par->source[par->width * (y + 2) + (x + 1)],
                                par->source[par->width * (y + 2) + (x + 2)]};
            
            par->dest[(y + 1) * par->width + x + 1] = sobel_mac_simd(inter, gy_array);
             
        }
    }
    
}


void sobel_sum ( const int16_t *source_x, const int16_t *source_y, uint8_t *dest, int height, int width) {

    int sobel_sum = 0;

    int i = 0, j = 0;
    
    for ( ; i < (height)*(width); i++) {
        sobel_sum = source_x[i] + source_y[i];

        // make it png ready
        dest[j++] = (uint8_t)(sobel_sum);
        dest[j++] = (uint8_t)(sobel_sum);
        dest[j++] = (uint8_t)(sobel_sum);
    }
}