#ifndef FILEMANAGEMENT_H
#define FILEMANAGEMENT_H

#include "object3D.h"

void writePPMP3(const char *fileName, Color **image, int height, int width);
void writePPMP3T(const char *fileName, Image *image, int height, int width);

#endif