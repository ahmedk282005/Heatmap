#ifndef BMP_H
#define BMP_H

#include <stdint.h>

/**
 * writes a file in bmp-format
 *
 * @param bmp_name name of .bmp-file (null-terminated string)
 * @param data array with width*height pixels -> use index data[y * BMP_WIDTH + x] for coordinate (x,y)
 * @param width width of bitmap
 * @param height height of bitmap
 *
 */
void bmp_create(const char * bmp_name, const uint32_t * data, uint16_t width, uint16_t height);


#endif /* BMP_H */
