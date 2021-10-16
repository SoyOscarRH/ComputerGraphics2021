#include "face.h"
#include "fileManagement.h"
#include "pixel.h"
#include "primitives.h"
#include "render.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main (int argc, const char *argv[])
{
  int width = 1040;
  int height = 585;

  PixelRGB green;
  green.R = 0;
  green.G = 255;
  green.B = 0;

  if (argc == 2)
    {
      int algorithm;
      printf ("0: Naive approach\n");
      printf ("1: DDA\n");
      printf ("2: Bresenham\n");
      printf ("Line drawing algorithm: ");
      scanf ("%d", &algorithm);

      objToRasterXY (argv[1], "output.ppm", width, height, algorithm, green);
      printf ("Drawing completed.\n");
    }
  else
    {
      printf ("./main <modelName.obj>\n");
    }
}