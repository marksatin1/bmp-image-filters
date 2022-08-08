#include "bmp.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]);

// Reflect image horizontally
void reflect_horz(int height, int width, RGBTRIPLE image[height][width]);

// Reflect image vertically
void reflect_vert(int height, int width, RGBTRIPLE image[height][width]);

// Invert image colors
void invert(int height, int width, RGBTRIPLE image[height][width]);

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width]);

// Detect image edges
void edges(int height, int width, RGBTRIPLE image[height][width]);
