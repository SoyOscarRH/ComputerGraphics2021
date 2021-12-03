#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "point.h"

Point2DInt *lineDrawingSpecialCases(Point2DInt p1, Point2DInt p2, int *numElements);

Point2DInt *BresenhamLineDrawing(Point2DInt p1, Point2DInt p2, int *numElements);


#endif