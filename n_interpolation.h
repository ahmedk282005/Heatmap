#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include "bmp.h"

// Prototypes
char *colorAllocation(float height, float min_height, float max_height);
uint32_t hex_to_uint32(const char* hex_color);

int nearest_neighbor(char *datafile, int xscaling, int yscaling)
{
    // Open input file
    FILE *file = fopen(datafile, "r");
    // Test
    if (file == NULL)
    {
        printf("Could not open file %s\n", datafile);
        return 2;
    }

    // Read data characteristics from input file
    float xmin, xmax, xstep, ymin, ymax, ystep;
    fscanf(file, "%f, %f, %f", &xmin, &xmax, &xstep);
    fscanf(file, "%f, %f, %f", &ymin, &ymax, &ystep);
    // Test
    printf("%f, %f, %f\n", xmin, xmax, xstep);
    printf("%f, %f, %f\n", ymin, ymax, ystep);


    // Declare the 'scale' of the input data
    int xlength = ((int)((xmax - xmin) + 1) / xstep);
    int ylength = ((int)((ymax - ymin) + 1) / ystep);

    // Allocate memory dynamically for total input data (for a 2D array {inputData[i][j] = inputData[xlength][ylength]})
    /* float* inputdata = (float*)malloc(xlength * ylength * sizeof(float)); */
    float** inputData = (float**)calloc((xlength), sizeof(float*));
    for (int i = 0; i < xlength; i++)
    {
        inputData[i] = (float*)calloc((ylength), sizeof(float));
    }

    // Read sampled data from file + assign height-sample to inputData[i][j] 
    // i.e. Store height points from the original data to the intital scale (like in a coordinate system)
    /* Assuming all x- and y-sample data are sorted, there is no need to store x and y */
    float x, y, height;
    for (int i = 0; i < xlength; i++) 
    {
        for (int j = 0; j < ylength; j++) 
        {
            fscanf(file, "%f, %f, %f", &x, &y, &height);
            inputData[i][j] = height;
        }   
    }   
    fclose(file);

    // Allocate memory dynamically for interpolated data (for a 2D array {interpolatedData[i][j] = interpolatedData[xscaling][yscaling]})
    /* float* interpolatedData = (float*)malloc(xlength * ylength * sizeof(float)); */
    float **interpolatedData = (float **)calloc(xscaling, sizeof(float *));

    for (int i = 0; i < xscaling; i++)
    {
        interpolatedData[i] = (float *)calloc(yscaling, sizeof(float));
    }

    // Nearest neighbor interpolation
    for (int i = 0; i < xscaling; i++) 
    {
        for (int j = 0; j < yscaling; j++) 
        {
            // Calculate the initial position
            float x = (float)i / (float)(xscaling - 1) * (xlength - 1);
            float y = (float)j / (float)(yscaling - 1) * (ylength - 1);

            // Calculate the nearest neighbor (x1,y1)
            int x1 = (int)x;
            float x_frac = x - x1;
            if (x_frac >= 0.5) 
            {
                x1 += 1;
            }
            int y1 = (int)y;
            float y_frac = y - y1;
            if (y_frac >= 0.5) 
            {
                y1 += 1;
            }

            // Assigning height to the interpolated data 
            interpolatedData[i][j] = inputData[x1][y1]; 
        }
    }

    // Find the min_- and max_height using 'linear search'
    float min_height = interpolatedData[0][0];
    float max_height = interpolatedData[0][0]; 

    for (int i = 0; i < xscaling; i++) 
    {
        for (int j = 0; j < yscaling; j++) 
        {
            float height = interpolatedData[i][j];
            if (height < min_height) 
            {
                min_height = height;
            }
            else if (height > max_height) 
            {
                max_height = height;
            }
        }
    }

    // Allocate memory dynamically for the color_map-array (type uint32_t*)
    uint32_t* color_map = (uint32_t*)calloc(((float)xscaling * (float)yscaling), sizeof(uint32_t));

    // Map the heights to colors
    for (int i = 0; i < xscaling; i++) 
    {
        for (int j = 0; j < yscaling; j++) 
        {
        char* color = colorAllocation(interpolatedData[i][j], min_height, max_height); // this uses the function that already assigned the heights to the colors to initialize the variable color
        color_map[j + i * xscaling] = hex_to_uint32(color); // stores the assigned colors inside the color_map-array using the array translation
        }
    }

    // Create the BMP file
    bmp_create("heatmap_n.bmp", color_map, xscaling, yscaling);

    // Free memory for inputData
    for (int i = 0; i < xlength; i++) 
    {
        free(inputData[i]);
    }
    free(inputData);

    // Free memory for interpolatedData
    for (int i = 0; i < xscaling; i++) 
    {
        free(interpolatedData[i]);
    }
    free(interpolatedData);

    // Free the color map
    free(color_map);

    return 0;
}