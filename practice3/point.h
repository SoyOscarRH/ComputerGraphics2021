#ifndef POINT_H
#define POINT_H

typedef struct Point3DHomogeneous{
    float x;
    float y;
    float z;
    float w;
} Point3DHomogeneous;

typedef struct Point3D{
    float x;
    float y;
    float z;
} Point3D;

typedef struct Point2D{
    float x;
    float y;
} Point2D;

typedef struct Point2DInt{
    int x;
    int y;
} Point2DInt;

struct Point3DNode{
    Point3D point;
    struct Point3DNode* next;
};
typedef struct Point3DNode Point3DNode;

Point2DInt transformPointToFirstOctant(Point2DInt p, float m);
Point2DInt transformPointFromFirstOctant(Point2DInt p, float m);
void swapPoints(Point2DInt *p1, Point2DInt *p2);

void transformArrayPoint2DFloatToInt(Point2D *floatPoints, Point2DInt *intPoints, int numPoints);

Point3DHomogeneous *convertToHomogeneous3D(Point3DNode *points, int numPoints);
void returnFromHomogeneous3D(Point3DNode *points, Point3DHomogeneous *homogeneousPoints, int numPoints);
Point3DNode *returnFromHomogeneous3DNew(Point3DHomogeneous *homogeneousPoints, int numPoints);

#endif