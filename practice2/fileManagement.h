#ifndef FILEMANAGEMENT_H
#define FILEMANAGEMENT_H

#include "pixel.h"

void writePPMP3 (const char *fileName, PixelRGB **image, int height,
                 int width);

#endif