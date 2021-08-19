#include "line.h"
#include "point.h"

double
slope (const line l)
{
  const point p1 = l.p1, p2 = l.p2;
  const double dx = p2.x - p1.x;
  const double dy = p2.y - p1.y;

  if (dx == 0.0)
    {
      return 0.0;
    }

  return dy / dx;
}

double
get_y_init (const line l)
{
  const point p1 = l.p1;
  return p1.y - slope (l) * p1.x;
}

double
get_y (const line l, const double x)
{
  const double m = slope (l);
  const double b = get_y_init (l);
  const double y = m * x + b;

  return y;
}

double
get_x (const line l, const double y)
{
  const double m = slope (l);
  const double b = get_y_init (l);
  const double x = (y - b) / m;

  return x;
}
