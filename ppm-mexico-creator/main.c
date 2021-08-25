#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/pixel.h"

int
main (int argc, char **argv)
{
  if (argc != 3)
    {
      printf ("Usage: ./ppm-mexico-creator rows cols\n");
      return 1;
    }
  const int rows = atoi (argv[1]), cols = atoi (argv[2]);
  const int section2 = cols / 3;
  const int section3 = (2 * cols) / 3;

  printf("sec2 %d\n", section2);
  printf("sec3 %d\n", section3);

  const pixel red = { 255, 0, 0 };
  const pixel green = { 0, 255, 0 };
  const pixel white = { 255, 255, 255 };

  FILE *output_file = fopen ("mexico.ppm", "w+");
  fprintf (output_file, "P3\n%d %d\n255\n", cols, rows);

  for (int dx = 0; dx < rows; dx++)
    {
      for (int dy = 0; dy < cols; dy++)
        {
          const pixel color = dy < section2   ? green
                              : dy < section3 ? white
                                              : red;

          fprintf (output_file, "%d %d %d  ", color.r, color.g, color.b);
        }
      fprintf (output_file, "\n");
    }

  fclose (output_file);

  return 0;
}
