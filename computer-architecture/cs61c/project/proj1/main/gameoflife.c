/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	//YOUR CODE HERE
	// 从左上角的细胞开始，顺时针存储邻居细胞的颜色
	uint32_t flated_neighbor_colors[8];
	int index = 0;
	for (int dy=-1; dy<=1; dy++) {
		for (int dx=-1; dx<=1; dx++) {
			if (dy == 0 && dx == 0) continue;

			// 神奇的公式，以 image->cols 为周期将所有的 x 映射到 [0, image->cols-1] 内
			// 也就是实现了使最开始和最末尾的列相邻
			// y 的计算同理
			int x = ((col+dx) % image->cols + image->cols) % image->cols;
			int y = ((row+dy) % image->rows + image->rows) % image->rows;
			Color* neighbor_color = image->image[y*image->cols + x];
			flated_neighbor_colors[index++] = (neighbor_color->R << 16) + (neighbor_color->G << 8) + neighbor_color->B;
		}
	}
	
	// 计算每个 bit 的存活邻居数
	int live_neighbors[24] = {};
	for (int i=0; i<8; i++) {
		for (int j=0; j<24; j++) live_neighbors[j] += flated_neighbor_colors[i] >> j & 0b1;
	}

	Color* current_color = image->image[image->cols*row + col];
	uint32_t flated_current_color = (current_color->R << 16) + (current_color->G << 8) + current_color->B;
	uint32_t flated_next_generation_color = 0;
	for (int i=0; i<24; i++) {
		int bit = flated_current_color >> i & 0b1;

		// 如果该 bit 为 1 且存活邻居数达标
		if (bit && (1 << (live_neighbors[i]+9) & rule)) flated_next_generation_color |= 0b1 << i;
		// 如果该 bit 为 0 且存活邻居数达标
		else if (!bit && (1 << live_neighbors[i] & rule)) flated_next_generation_color |= 0b1 << i;
	}

	Color* next_generation_color = (Color*) malloc(sizeof(Color));
	next_generation_color->R = flated_next_generation_color >> 16 & 0b11111111;
	next_generation_color->G = flated_next_generation_color >> 8 & 0b11111111;
	next_generation_color->B = flated_next_generation_color & 0b11111111;

	return next_generation_color;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	//YOUR CODE HERE
	Image* next_generation_image = (Image*) malloc(sizeof(Image));
	Color** next_generation_colors = (Color**) malloc(image->rows * image->cols * sizeof(Color*));
	next_generation_image->rows = image->rows;
	next_generation_image->cols = image->cols;
	next_generation_image->image = next_generation_colors;

	for (int i=0; i<image->rows; i++) {
		for (int j=0; j<image->cols; j++) {
			next_generation_colors[i*image->cols + j] = evaluateOneCell(image, i, j, rule);
		}
	}

	return next_generation_image;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc != 3) {
		printf("Error: invalid arguments' count");
		return -1;
	}

	Image* image = readData(argv[1]);
	if (image == NULL) {
		printf("Error: failed to load image %s", argv[1]);
		return -1;
	}

	uint32_t rule; sscanf(argv[2], "%x", &rule);
	Image* next_image = life(image, rule);
	writeData(next_image);

	freeImage(next_image);
	freeImage(image);

	return 0;
}
