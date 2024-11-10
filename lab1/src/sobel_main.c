/* ------------------------------------------------------------
                    ____     ________
                   /  _/__  / __/ __/    
                  _/ // _ \/ _/_\ \        
                 /___/_//_/___/___/            

    Institute of Embedded Systems
    Zurich University of Applied Sciences
    8401 Winterthur, Switzerland


    Project:  TSM_EmbHard Labs

    Purpose:  donn example answer for the TSM_EmbHard labs
              
    Remarks:

    Author:   donn

    Date:     KW45 2023
    Version:  0.9 FS23

    $Id$

   ------------------------------------------------------------ */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <sched.h>
#include <errno.h>
#include <sys/sysinfo.h>
#include <sys/syscall.h>

#include <time.h>

#include <pthread.h>
#include <math.h>

#include "../include/sobel_algo.h"
#include "../include/sobel_rgb2g.h"

// image library stuff
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define NUM_MEASUREMENT 100

#ifndef NUM_MEASUREMENT
#   define NUM_MEASUREMENT 20
#endif
#include "write_csv.h"

// non portable - change line numbers so __LINE__ reports actual line of C code
#line 62

int main()
{
   int i = NUM_MEASUREMENT;
   double out_array[NUM_MEASUREMENT];
   
   do {
       
       // threads variables
       double result;

       // timer variables
       struct timespec start, stop;
       	
       // pointers for the partial image arrays 
        uint8_t *rgb_image;
        uint8_t *gry_image;
        int16_t *sobel_x_array;
        int16_t *sobel_y_array;
        uint8_t *sobel_final_array;
    
        // define the necessary variables and load an image
        // I don't like declaring/defining variables in the middle of the code but do it here for clarity
        // the library call will simply open a png file - called image.png - and read the contents into memory it reserves within
        // the function call. 
        int width=0, height=0, bpp=0;
        rgb_image = stbi_load("./data/image.png", &width, &height, &bpp, 3);
        
        // in case of issues
        if (rgb_image == NULL) {
            perror("File read failed\n"); 
            exit(1);
        }
        
        // printout the dimensions of the image
        //(void) printf("width %d height %d bpp %d\n", width, height, bpp);
    
    	// reserve memory for the greyscale image
    	// the rgb png comes in 8-bit format
    	gry_image = malloc ( width * height * sizeof(uint8_t) );
    		
    	// reserve memory for sobel x and sobel y
    	// sobel_x_array = (int16_t *) malloc( (width -2 ) * (height - 2 ) * sizeof(int16_t) );
    	// sobel_y_array = (int16_t *) malloc( (width -2 ) * (height - 2 ) * sizeof(int16_t) );
        sobel_x_array = (int16_t *) malloc( (width) * (height) * sizeof(int16_t) );
    	sobel_y_array = (int16_t *) malloc( (width) * (height) * sizeof(int16_t) );
    	
    	// reserve memory for the final output array - png compatible
    	// sobel_final_array = (uint8_t *) malloc( (width-2) * (height-2) * 3 * sizeof(uint8_t) );
        sobel_final_array = (uint8_t *) malloc( (width) * (height) * 3 * sizeof(uint8_t) );

    	// convert image to to greyscale
    	rgb2grayscale(width, height, rgb_image, gry_image);
    	
    	// initalise the parameters for the sobel functions 
    	// structure defined in sobel_algo.h
    	sobel_args x_par, y_par;
    	
    	// fill them in with values
    	x_par.source = gry_image;
    	x_par.dest = sobel_x_array;
    	x_par.width = width;
    	x_par.height = height;
    	
    	// copy the values for x into y and change the y destination
    	y_par = x_par;
    	y_par.dest = sobel_y_array;
    
        clock_gettime(CLOCK_REALTIME, &start);
    	
    	// call sobel_x and sobel_y
    	// sobel_x_inline(&x_par);
        // sobel_y_inline(&y_par);
        sobel_x_smid(&x_par);
        sobel_y_smid(&y_par);
        // sobel_x(&x_par);
        // sobel_y(&y_par);

        // add the results of sobel_x and sobel_y together
        sobel_sum(sobel_x_array, sobel_y_array, sobel_final_array, height, width);
        
        // get and calculate time -AFTER the join/barier point 
        int retval = clock_gettime(CLOCK_REALTIME, &stop);
        result = ( (stop.tv_sec - start.tv_sec) + ( (stop.tv_nsec - start.tv_nsec)  / 1e9 ) ) ;    // in seconds
        out_array[--i] = result;
        
        (void)printf("Line %d: Time elapsed is: %f\n", __LINE__, result);
    
        // write the image into a file edge_image.png
        stbi_write_png("edge_image.png", (width -2), (height - 2), 3, sobel_final_array, width * 3);

    } while ( i > 0 );
    
    // write int array into comma separated values file
    write_csv ( out_array, NUM_MEASUREMENT );

    // everything OK leave
    // note: we haven't freed the memory reserved by the image library or by us 
    return(0);

}