
#ifndef _LINE_
#define _LINE_

#include "point.h"

struct line
{
  const point p1, p2;
};

typedef struct line line;

double slope (const line l);

double get_y_init (const line l);

double get_y (const line l, const double x);

double get_x (const line l, const double y);

#endif
