#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * C language program that gets as input two ordered pairs (x1,y1) and
 * (x2,y2). All coordinate values will be in the range 0 <= xi, yi <= 100. The
 * output will be a table (x,y) of integer values, where each pair represents a
 * pixel to “render” (represent, cover, resemble) a line segment.
 */

struct point {
  const double x, y;
};
typedef struct point point;

double slope(const point p1, const point p2) {
  const double dx = p2.x - p1.x;
  const double dy = p2.y - p1.y;

  if (dx == 0.0) {
    return 0.0;
  }

  return dy / dx;
}

double get_y_init(const point p1, const point p2) {
  return p1.y - slope(p1, p2) * p1.x;
}

double get_y(const point p1, const point p2, const double x) {
  const double m = slope(p1, p2);
  const double b = get_y_init(p1, p2);
  const double y = m * x + b;

  return y;
}

double get_x(const point p1, const point p2, const double y) {
  const double m = slope(p1, p2);
  const double b = get_y_init(p1, p2);
  const double x = (y - b) / m;

  return x;
}

int is_between_0_and_100(const int x) { return (x >= 0 && x <= 100) ? 1 : 0; }

int main(int argc, char** argv) {
  if (argc != 5) {
    printf("Usage: ./practice1 <x1> <y1> <x2> <y2>\n");
    return 1;
  }

  const int x1 = atoi(argv[1]), y1 = atoi(argv[2]);
  const int x2 = atoi(argv[3]), y2 = atoi(argv[4]);

  if (!is_between_0_and_100(x1) || !is_between_0_and_100(y1) ||
      !is_between_0_and_100(x2) || !is_between_0_and_100(y2)) {
    printf("Error: Coordinates must be in the range 0 <= xi, yi <= 100\n");
  }

  const int dx = abs(x2 - x1), dy = abs(y2 - y1);
  const point p1 = {x1, y1}, p2 = {x2, y2};

  if (dx > dy) {
    const int x_min = x1 < x2 ? x1 : x2;
    printf("x y\n");
    for (int i = 0; i <= dx; i++) {
      const int x = x_min + i;
      const int y = round(get_y(p1, p2, x));
      printf("%d %d\n", (int)x, (int)y);
    }
  } else {
    const int y_min = y1 < y2 ? y1 : y2;
    printf("x y\n");
    for (int i = 0; i <= dy; i++) {
      const int y = y_min + i;
      const int x = round(get_x(p1, p2, y));
      printf("%d %d\n", (int)x, (int)y);
    }
  }

  return 0;
}