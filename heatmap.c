#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include "bmp.h"
#include "n_interpolation.h"
#include "b_interpolation.h"

// Define color scale
#define NR_COLORS  10
char *color_scale[NR_COLORS] = {"#00008F", "#0000FF", "#006FFF", "#00DFFF", "#4FFFAF", "#A9FF3F", "#FFDF00", "#FF6F00", "#FF0000", "#7F0000"};

// Prototypes
char *colorAllocation(float height, float min_height, float max_height);
uint32_t hex_to_uint32(const char* hex_color);


int main(int argc, char *argv[]) 
{
    if (argc != 5) 
    {
        printf("Usage: %s <datafile> <interpolation intital (n/b)> <xscaling> <yscaling>\n", argv[0]);
        return 1;
    }

    char *datafile = argv[1];
    char interpolation = *argv[2];
    int xscaling = atoi(argv[3]);
    int yscaling = atoi(argv[4]);
    int error = 0;

    if (interpolation == 'n')
    {
        error = nearest_neighbor(datafile, xscaling, yscaling);
    }
    else if (interpolation == 'b')
    {
        error = bilinear_interpolation(datafile, xscaling, yscaling);
    }
    if (error != 0) 
    {
        printf("%c\nerror %i",interpolation, error);
    }

    return error;
}

// Function to convert hex color to uint32_t
uint32_t hex_to_uint32(const char* hex_color) 
{
    uint32_t r, g, b;
    sscanf(hex_color, "#%02x%02x%02x", &r, &g, &b);
    return (r << 16) | (g << 8) | b;
}

// Grouping
char *colorAllocation(float height, float min_height, float max_height) 
{
    int index = (int)(((height - min_height) / (max_height - min_height)) * NR_COLORS); 
    if (index < 0) index = 0;  
    if (index >= NR_COLORS) index = NR_COLORS - 1; 
    return (char *)color_scale[index]; 
}
