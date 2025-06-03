#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include "bmp.h"

// Prototypes
char *colorAllocation(float height, float min_height, float max_height);
uint32_t hex_to_uint32(const char* hex_color);

int bilinear_interpolation(char *datafile, int xscaling, int yscaling){
    // Open input file
    FILE *file = fopen(datafile, "r");
    if (file == NULL) {
        printf("Could not open file %s\n", datafile);
        return 1;
    }

    // Read data characteristics from CSV-data
    float xmin, xmax, xstep, ymin, ymax, ystep;
    fscanf(file, "%f, %f, %f", &xmin, &xmax, &xstep);
    fscanf(file, "%f, %f, %f", &ymin, &ymax, &ystep);
    // Test
    printf("%f, %f, %f\n", xmin, xmax, xstep);
    printf("%f, %f, %f\n", ymin, ymax, ystep);


    // Declare xlength and ylength
    int xlength = ((int)((xmax - xmin) + 1) / xstep);
    int ylength = ((int)((ymax - ymin) + 1) / ystep);

    // Allocate memory for data
    float* data = (float*)malloc(xlength * ylength * sizeof(float));

    // Read data from file
    float x, y, height;
    for (int i = 0; i < xlength * ylength; i++) {
        fscanf(file, "%f, %f, %f", &x, &y, &height);
        data[i] = height;
    }

    // Allocate memory for interpolated data
    unsigned long outputData;
    outputData = (unsigned long)xscaling * (unsigned long)yscaling;
    float *interpolatedData = (float *)malloc(outputData * sizeof(float));

    // Bilinear interpolation
    for (int i = 0; i < xscaling; i++) {
        for (int j = 0; j < yscaling; j++) {
            // Calculate the position in the original data
            float x = (float)i / (float)(xscaling - 1) * (xlength - 1);
            float y = (float)j / (float)(yscaling - 1) * (ylength - 1);

            // Find the surrounding points (x-verticle /y-verticle )
            int x1 = (int)x;
            int x2 = x1 < xlength - 1 ? x1 + 1 : x1;
            int y1 = (int)y;
            int y2 = y1 < ylength - 1 ? y1 + 1 : y1;

            // Calculate the fractions
            float x_frac = x - x1;
            float y_frac = y - y1;

            // Interpolate in x direction
            float height_x1 = data[x1 + y1 * xlength] * (1 - x_frac) + data[x2 + y1 * xlength] * x_frac;
            float height_x2 = data[x1 + y2 * xlength] * (1 - x_frac) + data[x2 + y2 * xlength] * x_frac;

            // Interpolate in y direction
            float height = height_x1 * (1 - y_frac) + height_x2 * y_frac;

            // Store the interpolated height using the array translation
            interpolatedData[i + j * xscaling] = height;
        }
    }

    // Calculate min and max height using 'linear search'
    float min_height = FLT_MAX;
    float max_height = FLT_MIN;
    for (unsigned long i = 0; i < outputData; i++) {
        float height = interpolatedData[i];
        if (height < min_height) min_height = height;
        if (height > max_height) max_height = height;
    }

    // Allocate memory for the color_map-array (type uint32_t*)
    uint32_t* color_map = (uint32_t*)malloc(outputData * sizeof(uint32_t));

    // Map the heights to colors
    for (unsigned long i = 0; i < outputData; i++) {
        char* color = colorAllocation(interpolatedData[i], min_height, max_height); // this uses the function that already assigned the heights to the colors to initialize the variable color
        color_map[i] = hex_to_uint32(color); // stores the assigned colors inside the color_map-array
    }

    // Create the BMP file
    bmp_create("heatmap_b.bmp", color_map, xscaling, yscaling);

    // Free the color map
    free(color_map);

    // Free input data array
    free(data);

    // free interpolated data array
    free(interpolatedData);

    // Close file
    fclose(file);
    return 0;
}
