// BMP-related data types adapted from Microsoft's BMP 4.0 //
#include <stdint.h>


/* Common Data Types - Aliases for C primitive data types */
typedef uint8_t  BYTE;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef uint16_t WORD;


/* The BITMAPFILEHEADER structure contains information about the type, size, 
and layout of a file that contains a DIB [device-independent bitmap]. */
typedef struct {
  WORD   bfType;
  DWORD  bfSize;
  WORD   bfReserved1;
  WORD   bfReserved2;
  DWORD  bfOffBits;
} __attribute__((__packed__))
BITMAPFILEHEADER;


/* The BITMAPINFOHEADER structure contains information about the 
dimensions and color format of a DIB [device-independent bitmap]. */
typedef struct {
  DWORD  biSize;
  LONG   biWidth;
  LONG   biHeight;
  WORD   biPlanes;
  WORD   biBitCount;
  DWORD  biCompression;
  DWORD  biSizeImage;
  LONG   biXPelsPerMeter;
  LONG   biYPelsPerMeter;
  DWORD  biClrUsed;
  DWORD  biClrImportant;
} __attribute__((__packed__))
BITMAPINFOHEADER;


/* RGBTRIPLE describes a color consisting of relative intensities 
of red, green, and blue. */
typedef struct {
  BYTE  rgbtBlue;
  BYTE  rgbtGreen;
  BYTE  rgbtRed;
} __attribute__((__packed__))
RGBTRIPLE;
