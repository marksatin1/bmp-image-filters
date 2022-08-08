#include <math.h>
#include <stdio.h>

#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]) {
  int avg;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      avg = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

      image[i][j].rgbtRed = avg;
      image[i][j].rgbtGreen = avg;
      image[i][j].rgbtBlue = avg;
    }
  }
  return;
}

// Reflect image horizontally
void reflect_horz(int height, int width, RGBTRIPLE image[height][width]) {
  RGBTRIPLE temp[height][width];

  for (int row = 0; row < height; row++) {
    for (int column = 0; column < width / 2; column++) {
      temp[row][column] = image[row][column];
      image[row][column] = image[row][width - column - 1];
      image[row][width - column - 1] = temp[row][column];
    }
  }
  return;
}

// Reflect image vertically
void reflect_vert(int height, int width, RGBTRIPLE image[height][width]) {
  RGBTRIPLE temp[height][width];

  for (int row = 0; row < height / 2; row++) {
    for (int column = 0; column < width; column++) {
      temp[row][column] = image[row][column];
      image[row][column] = image[height - row - 1][column];
      image[height - row - 1][column] = temp[row][column];
    }
  }
  return;
}

// Invert image
void invert(int height, int width, RGBTRIPLE image[height][width]) {
  for (int row = 0; row < height; row++) {
    for (int column = 0; column < width; column++) {
      image[row][column].rgbtRed = 255 - image[row][column].rgbtRed;
      image[row][column].rgbtGreen = 255 - image[row][column].rgbtGreen;
      image[row][column].rgbtBlue = 255 - image[row][column].rgbtBlue;
    }
  }
  return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width]) {
  RGBTRIPLE temp[height][width];

  for (int row = 0; row < height; row++) {
    for (int column = 0; column < width; column++) {
      temp[row][column] = image[row][column];
      int sumRed = 0, sumGreen = 0, sumBlue = 0;
      float counter = 0;

      // Build a 3x3 kernel to find box blur averages
      for (int i = row - 1; i < row + 2; i++) {
        for (int j = column - 1; j < column + 2; j++) {

          // Only consider values in image[i][j]
          if (i >= 0 && i < height && j >= 0 && j < width) {
            sumRed += image[i][j].rgbtRed;
            sumGreen += image[i][j].rgbtGreen;
            sumBlue += image[i][j].rgbtBlue;
            counter++;
          }
        }
      }

      // Set average values into temp array
      temp[row][column].rgbtRed = round(sumRed / counter);
      temp[row][column].rgbtGreen = round(sumGreen / counter);
      temp[row][column].rgbtBlue = round(sumBlue / counter);
    }
  }

  // Replace original image with blur values
  for (int row = 0; row < height; row++) {
    for (int column = 0; column < width; column++) {
      image[row][column] = temp[row][column];
    }
  }
  return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width]) {
  RGBTRIPLE temp[height][width];
  int Gx[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
  int Gy[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

  for (int row = 0; row < height; row++) {
    for (int column = 0; column < width; column++) {
      temp[row][column] = image[row][column];
      int SumGxRed = 0, SumGyRed = 0;
      int SumGxGreen = 0, SumGyGreen = 0;
      int SumGxBlue = 0, SumGyBlue = 0;

      float GRed = 0, GGreen = 0, GBlue = 0;
      int RRed = 0, RGreen = 0, RBlue = 0;

      int k = 0;

      // Create a 3x3 grid surrounding temp[row][column]
      for (int i = row - 1; i < row + 2; i++) {
        for (int j = column - 1; j < column + 2; j++) {

          // Only consider values in image[i][j]
          if (i >= 0 && i < height && j >= 0 && j < width) {
            SumGxRed += (Gx[k] * image[i][j].rgbtRed);
            SumGyRed += (Gy[k] * image[i][j].rgbtRed);

            SumGxGreen += (Gx[k] * image[i][j].rgbtGreen);
            SumGyGreen += (Gy[k] * image[i][j].rgbtGreen);

            SumGxBlue += (Gx[k] * image[i][j].rgbtBlue);
            SumGyBlue += (Gy[k] * image[i][j].rgbtBlue);

            k++;
          }
          else {
            k++;
          }
        }
      }

      GRed = sqrt(SumGxRed * SumGxRed + SumGyRed * SumGyRed);
      GGreen = sqrt(SumGxGreen * SumGxGreen + SumGyGreen * SumGyGreen);
      GBlue = sqrt(SumGxBlue * SumGxBlue + SumGyBlue * SumGyBlue);

      RRed = round(GRed);
      RGreen = round(GGreen);
      RBlue = round(GBlue);

      if (RRed >= 255) {
        temp[row][column].rgbtRed = 255;
      }
      else {
        temp[row][column].rgbtRed = RRed;
      }

      if (RGreen >= 255) {
        temp[row][column].rgbtGreen = 255;
      }
      else {
        temp[row][column].rgbtGreen = RGreen;
      }

      if (RBlue >= 255) {
        temp[row][column].rgbtBlue = 255;
      }
      else {
        temp[row][column].rgbtBlue = RBlue;
      }
    }
  }

  // Replace original image with G values
  for (int row = 0; row < height; row++) {
    for (int column = 0; column < width; column++) {
      image[row][column] = temp[row][column];
    }
  }
  return;
}