#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

int main(int argc, char *argv[]) {
  // Define allowable filters
  char *filters = "ghvibe";

  // Get filter flag and check validity
  char filter = getopt(argc, argv, filters);
  if (filter == '?') {
    fprintf(stderr, "ERROR: Invalid filter. Must choose from the following:\n  -g  convert to grayscale\n  -h  reflect horizontally\n  -v  reflect vertically\n  -i  invert colors\n  -b  blur\n  -e  detect edges");
    return 1;
  }

  // Ensure only one filter
  if (getopt(argc, argv, filters) != -1) {
    fprintf(stderr, "ERROR: Only one filter allowed.\n");
    return 2;
  }

  // Ensure proper syntax
  if (argc != optind + 2) {
    fprintf(stderr, "ERROR: Must use corect syntax:\nfilter [flag] infile outfile\n");
    return 3;
  }

  char *infile = argv[optind];
  char *outfile = argv[optind + 1];

  FILE *inptr = fopen(infile, "r");
  if (inptr == NULL) {
    fprintf(stderr, "ERROR: Could not open %s.\n", infile);
    return 4;
  }

  FILE *outptr = fopen(outfile, "w");
  if (outptr == NULL) {
    fclose(inptr);
    fprintf(stderr, "ERROR: Could not write to %s.\n", outfile);
    return 5;
  }

  BITMAPFILEHEADER bf;
  fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

  BITMAPINFOHEADER bi;
  fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

  // Ensure infile is (likely) a 24-bit BMP 4.0
  if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || bi.biBitCount != 24) {
    fclose(outptr);
    fclose(inptr);
    fprintf(stderr, "ERROR: Unsupported file format.\n");
    return 6;
  }

  int height = abs(bi.biHeight);
  int width = bi.biWidth;

  // Allocate memory for image
  RGBTRIPLE(*image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
  if (image == NULL) {
    fprintf(stderr, "ERROR: Not enough memory to store image.\n");
    fclose(outptr);
    fclose(inptr);
    return 7;
  }

  // Determine padding for scanlines
  int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

  // Iterate over infile's scanlines
  for (int i = 0; i < height; i++) {
    fread(image[i], sizeof(RGBTRIPLE), width, inptr);
    // Skip over padding
    fseek(inptr, padding, SEEK_CUR);
  }

  // Handle filter options
  switch (filter) {
    case 'g':
      grayscale(height, width, image);
      break;

    case 'h':
      reflect_horz(height, width, image);
      break;

    case 'v':
      reflect_vert(height, width, image);
      break;
    
    case 'i':
      invert(height, width, image);
      break;
      
    case 'b':
      blur(height, width, image);
      break;

    case 'e':
      edges(height, width, image);
      break;
  }

  fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
  fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

  // Write new pixels to outfile
  for (int i = 0; i < height; i++) {
    fwrite(image[i], sizeof(RGBTRIPLE), width, outptr);
    // Write padding at end of row
    for (int k = 0; k < padding; k++) {
      fputc(0x00, outptr);
    }
  }
  printf("Your filtered image is ready to view!");

  free(image);
  fclose(inptr);
  fclose(outptr);

  return 0;
}