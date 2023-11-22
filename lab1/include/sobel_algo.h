#ifndef ALGO
#define ALGO

typedef struct {
    const uint8_t *source; 
    int16_t *dest;
    int width; 
    int height;
} sobel_args;

int16_t sobel_mac( const uint8_t *, int, int, int8_t const *, int );
void sobel_x ( const sobel_args *);
void sobel_y ( const sobel_args *);
void sobel_sum ( const int16_t *, const int16_t *, uint8_t *, int, int );


#endif