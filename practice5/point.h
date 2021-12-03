#ifndef POINT_H
#define POINT_H

#include "color.h"

typedef struct Point3DHomogeneous{
    double x;
    double y;
    double z;
    double w;
} Point3DHomogeneous;

typedef struct Point3D{
    double x;
    double y;
    double z;
} Point3D;

typedef struct Point2D{
    double x;
    double y;
} Point2D;

typedef struct Point2DInt{
    int x;
    int y;
} Point2DInt;

void swapPoints(Point2DInt *p1, Point2DInt *p2);
Point2DInt transformPointToFirstOctant(Point2DInt p, float m);
Point2DInt transformPointFromFirstOctant(Point2DInt p, float m);

#endif