/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	//YOUR CODE HERE
	Image* image = (Image*) malloc(sizeof(Image));


	FILE* file = fopen(filename, "r");
	if (file == NULL) return NULL;

	int type, cols, rows;
	int max_color_value;

	fscanf(file, "P%d", &type);
	fscanf(file, "%d %d %d", &cols, &rows, &max_color_value);

	// 仅支持 PPM P3 格式读取
	if (type != 3) return NULL;

	Color** color_list = (Color**) malloc(cols * rows * sizeof(Color*));
	image->image = color_list;
	image->rows = rows;
	image->cols = cols;

	int color_index = 0;
	for (int i=0; i<rows; i++) {
		for (int j=0; j<cols; j++) {
			Color* color = (Color*) malloc(sizeof(Color));
			uint8_t r, g, b;
			
			// 无符号八位整数使用 hhu
			fscanf(file, "%hhu %hhu %hhu", &r, &g, &b);

			color->R = r;
			color->G = g;
			color->B = b;
			color_list[color_index++] = color;
		}
	}

	fclose(file);
	return image;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
	uint8_t cols = image->cols, rows = image->rows;

	printf("P3\n");
	printf("%hhu %hhu\n", cols, rows);
	printf("255\n");

	Color** color = image->image;
	for (int i=0; i<rows; i++) {
		for (int j=0; j<cols; j++) {
			printf("%3hhu %3hhu %3hhu", (*color)->R, (*color)->G, (*color)->B);
			color++;

			if (j != cols-1) printf("   ");
		}
		printf("\n");
	}
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
	// 不能使用 uint8_t ：作为长度，uint8_t太小
	uint32_t length = image->cols * image->rows;
	Color** colors = image->image;

	for (int i=0; i<length; i++) free(colors[i]);
	free(colors);
	free(image);
}