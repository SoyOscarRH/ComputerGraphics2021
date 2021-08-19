#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/line.h"

/*
 * C language program that gets as input two ordered pairs (x1,y1) and
 * (x2,y2). All coordinate values will be in the range 0 <= xi, yi <= 100.
 * The output will be a table (x,y) of integer values, where each pair
 * represents a pixel to “render” (represent, cover, resemble) a line
 * segment.
 */

int
is_between_0_and_100 (const int x)
{
  return (x >= 0 && x <= 100) ? 1 : 0;
}

int
main (int argc, char **argv)
{
  if (argc != 5)
    {
      printf ("Usage: ./practice1 <x1> <y1> <x2> <y2>\n");
      return 1;
    }
  const int x1 = atoi (argv[1]), y1 = atoi (argv[2]);
  const int x2 = atoi (argv[3]), y2 = atoi (argv[4]);

  if (!is_between_0_and_100 (x1) || !is_between_0_and_100 (y1)
      || !is_between_0_and_100 (x2) || !is_between_0_and_100 (y2))
    {
      printf ("Error: Coordinates must be in the range 0 <= xi, yi <= 100\n");
    }
  const int dx = abs (x2 - x1), dy = abs (y2 - y1);
  const point p1 = { x1, y1 }, p2 = { x2, y2 };
  const line l = { p1, p2 };

  if (dx > dy)
    {
      const int x_min = x1 < x2 ? x1 : x2;
      printf ("x y\n");
      for (int i = 0; i <= dx; i++)
        {
          const int x = x_min + i;
          const int y = round (get_y (l, x));
          printf ("%d %d\n", (int)x, (int)y);
        }
    }
  else
    {
      const int y_min = y1 < y2 ? y1 : y2;
      printf ("x y\n");
      for (int i = 0; i <= dy; i++)
        {
          const int y = y_min + i;
          const int x = round (get_x (l, y));
          printf ("%d %d\n", (int)x, (int)y);
        }
    }

  return 0;
}
