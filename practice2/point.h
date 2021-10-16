#ifndef POINT_H
#define POINT_H

typedef struct Point3D
{
  float x;
  float y;
  float z;
} Point3D;

typedef struct Point2D
{
  float x;
  float y;
} Point2D;

typedef struct Point2DInt
{
  int x;
  int y;
} Point2DInt;

struct Point3DNode
{
  Point3D point;
  struct Point3DNode *next;
};
typedef struct Point3DNode Point3DNode;

Point2DInt transformPointToFirstOctant (Point2DInt p, float m);
Point2DInt transformPointFromFirstOctant (Point2DInt p, float m);
void swapPoints (Point2DInt *p1, Point2DInt *p2);

void transformArrayPoint2DFloatToInt (Point2D *floatPoints,
                                      Point2DInt *intPoints, int numPoints);

#endif