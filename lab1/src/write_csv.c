/* ------------------------------------------------------------
                    ____     ________
                   /  _/__  / __/ __/    
                  _/ // _ \/ _/_\ \        
                 /___/_//_/___/___/            

    Institute of Embedded Systems
    Zurich University of Applied Sciences
    8401 Winterthur, Switzerland


    Project:  MPC Labs
    File:     $RCSfile$

    Purpose:  donn example answer for the MPC task decomposition/threads exercise 
              
    Remarks:  MPC lab 

    Author:   donn

    Date:     KW9 2021
    Version:  0.9 FS21

    $Id$

   ------------------------------------------------------------ */
#include <stdio.h>  
#include <string.h> 

#include "write_csv.h"
   
#define OUTPUT_FILE "timing.csv"
#define CHAR_ARRAY 1024
#define INT_ARRAY 255

// check if the file is empty if not include header for R
void write_csv ( int const *in_array, int num_measurement ) {
    // reserrve memory for an error string
    char char_string[CHAR_ARRAY];
    char temp_string[CHAR_ARRAY];
    
    char *fn = OUTPUT_FILE;
    FILE *fp;
    
    // open the file
    if ( NULL == ( fp = fopen( fn, "a+" ) ) ) {
        (void) sprintf( char_string, "%s Error opening output file ", __func__ );
        perror ( char_string ); 
    }

    // else start filling up the lines
    // opening for append means writing at the end of tzhe file.
    // each data set to end with a new line means that each subsequent starts
    // at the beginning of the next line
    
    // first expand the function __FILE__ and get rid of the path
    (void)sprintf( temp_string, "%s", __FILE__ );
 
    // search for the last / character and if found the copy the remaining 
    // string - the file name - into the output string 
    char *result = strrchr( temp_string, (int) '/' );
    if ( NULL == result ) {
        result = temp_string;
        (void) fprintf( fp, "%s,", result );
    } else {
        // and copy string intodestination file
        // pre increment pointer to point to first character of file name  
        result = temp_string + 1;
        (void) fprintf( fp, "%s,", result );
    }    
    
    // copy data into file
    for ( int i=0; i < num_measurement; i++ ) {
        (void) fprintf( fp, "%d,", in_array[i]);   
    }
    // write it out to file
    (void) fprintf ( fp, "\n");
    (void) fclose(fp);
}
