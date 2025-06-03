#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "bmp.h"


/**
 * writes the value given with bytes with
 * N bytes to file bmp
 *
 * @param bmp pointer to bitmap-file
 * @param bytes bytes to be written to bitmap file
 * @param N number of bytes to be written (at most 8)
 *
 */
static void bmp_write_N_byte(FILE * bmp, uint64_t bytes, uint8_t N);



void bmp_create(const char * bmp_name, const uint32_t * data, uint16_t width, uint16_t height)
{

  if (data == NULL || width == 0 || height == 0) {
    fprintf(stderr, "Error: invalid parameters for bmp-file \n");
    exit(EXIT_FAILURE);
  }

  FILE * fpbmp = fopen(bmp_name, "wb");
  if (fpbmp == NULL) {
    fprintf(stderr, "Error: cannot open bmp-file \n");
    exit(EXIT_FAILURE);
  }

  /* Write to file as follows: 1.bmp-file-head :: 2.bmp-file-info :: 3.pixel data */

  /* BMP header */
  /* magic code: BM */
  fputc('B', fpbmp);
  fputc('M', fpbmp);
  /* (4 Byte, uint32_t) size of BMP file in bytes
   * = bmp_file_header + bmp_info_header + pixels 
   */
  bmp_write_N_byte(fpbmp, 14 + 40 + width * height * 4, 4);
  /* (4 Byte, uint32_t) reserved, default: 0 */
  bmp_write_N_byte(fpbmp, 0, 4);
  bmp_write_N_byte(fpbmp, 54, 4);

  /* BMP info */
  /*(4 Byte, uint32_t) size of bmp_info in byte */
  bmp_write_N_byte(fpbmp, 40, 4);
  /*(4Byte, int32_t) width of bitmap in #pixels */
  bmp_write_N_byte(fpbmp, width, 4);
  /* (4 Byte, int32_t) height of bitmap in #pixels. negative: top-bottom */
  bmp_write_N_byte(fpbmp, -height, 4);
  /* (2 Byte, uint16_t) unused (number of color layers) */
  bmp_write_N_byte(fpbmp, 1, 2);
  /* (2 Byte, uint16_t) color format in bpp; must be one of
   * 1, 4, 8, 16, 24 or 32, where colors are indexed for 1, 4 and 8 bpp 
   */
  bmp_write_N_byte(fpbmp, 32, 2);
  /* (4 Byte, uint32_t) compression: 0 = none */
  bmp_write_N_byte(fpbmp, 0, 4);
  /* (4 Byte, uint32_t) size for all pixels (including padding) in byte */
  bmp_write_N_byte(fpbmp, width * height * 4, 4);
  /* (4 Byte, int32_t) horizontal resolution, typically 0 for BMP */
  bmp_write_N_byte(fpbmp, 0, 4);
  /* (4 Byte, int32_t) vertical resolution, typically 0 for BMP */
  bmp_write_N_byte(fpbmp, 0, 4);
  /* (4 Byte, uint32_t) number of entries in color table: 0 = none */
  bmp_write_N_byte(fpbmp, 0, 4);
  /* (4 Byte, uint32_t) number of important colors; 0 = all are important */
  bmp_write_N_byte(fpbmp, 0, 4);

  /* write pixels */
  for (uint16_t y = 0; y < height; y++) {
    for (uint16_t x = 0; x < width; x++) {
      bmp_write_N_byte(fpbmp, data[y * width + x], 4);
    }
  }

  fclose(fpbmp);
}


static void bmp_write_N_byte(FILE * bmp, uint64_t bytes, uint8_t N)
{
    for(uint8_t i = 0; ((i < N) && (i < 8)); i++) {
        fputc(bytes % 256, bmp);
        bytes /= 256;
    }
}
