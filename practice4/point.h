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

Point3D *homogeneous3Dto3D(Point3DHomogeneous *points3DH, int numPoints, Point3D *minP, Point3D *maxP);

double magnitude3DVector(Point3D A);
Point3D unitVector(Point3D v);
Point3D crossProduct(Point3D A, Point3D B);
double dotProduct(Point3D A, Point3D B);
double angleBetweenTwoVectors(Point3D A, Point3D B);

double distanceBetween3DPoints(Point3D A, Point3D B);

Point3D *screenSpacetoNDC(Point3D *screenPoints, int numPoints, double widthCanvas, double heightCanvas);

typedef struct Point3DNode{
    Point3D point;
    struct Point3DNode *next;
} Point3DNode;

typedef struct Point2D{
    double x;
    double y;
} Point2D;

Point2D *reduce3Dto2DXY(Point3D *points3D, int numPoints);
void scale2DPoints(Point2D *points, int numPoints, double scale);
void center2DPoints(Point2D *points, int numPoints, double xCenter, double yCenter, double xMin, double xMax, double yMin, double yMax);

typedef struct Point2DInt{
    int x;
    int y;
} Point2DInt;

Point2DInt *NDCtoRaster(Point3D *NDCPoints, int numPoints, int width, int height);

void transformArrayPoint2DDoubleToInt(Point2D *doublePoints, Point2DInt *intPoints, int numPoints);

Point2DInt transformPointToFirstOctant(Point2DInt p, float m);
Point2DInt transformPointFromFirstOctant(Point2DInt p, float m);
void swapPoints(Point2DInt *p1, Point2DInt *p2);

double *interpolate(double firstValue, double lastValue, int numElements);

void copyPoint2DIntArray(Point2DInt *a, Point2DInt *b, int start, int end);
void mergeSort2DIntByY(Point2DInt *a, int start, int size);

void copyPoint3DArray(Point3D *a, Point3D *b, int start, int end);
void mergeSort3DByY(Point3D *a, int start, int size);

void mergeSort3DColorByY(Point3D *a, Color* colorA, int start, int size);

#endif