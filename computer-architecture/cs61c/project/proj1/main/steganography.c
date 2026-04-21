/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	//YOUR CODE HERE
	Color* color = (Color*) malloc(sizeof(Color));
	// 获取像素的B位的字节，并获取最低位的值
	int bit = image->image[image->cols*row+col]->B & 0b1;
	// -0 = 0b00000000
	// -1 = 0b11111111
	color->R = color->G = color->B = -bit;

	return color;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	//YOUR CODE HERE
	Image* new_image = (Image*) malloc(sizeof(Image));
	Color** new_colors = (Color**) malloc(image->cols * image->rows * sizeof(Color*));
	// 获取隐藏颜色
	for (int i=0; i<image->rows; i++) {
		for (int j=0; j<image->cols; j++) {
			new_colors[i*image->cols+ j] = evaluateOnePixel(image, i, j);
		}
	}

	new_image->cols = image->cols;
	new_image->rows = image->rows;
	new_image->image = new_colors;

	return new_image;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc != 2)  {
		printf("Error: invalid arguments' count");
		return -1;
	}

	Image* orig_image = readData(argv[1]);
	if (orig_image == NULL) {
		printf("Error: failed to open file %s", argv[1]);
		return -1;
	}

	Image* secret_image = steganography(orig_image);
	writeData(secret_image);

	freeImage(secret_image);
	freeImage(orig_image);

	return 0;
}
