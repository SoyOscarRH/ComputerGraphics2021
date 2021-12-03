#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "pixel.h"
#include "face.h"
#include "point.h"
#include "threadPool.h"

typedef struct InfoParallelLineDrawing{
    Point2DInt p1;
    Point2DInt p2;
    Point2DInt *points;
    int numPoints;
    PixelRGB *color;
    PixelRGB **image;
    ThreadPool *pool;
} InfoParallelLineDrawing;

Point2DInt *naiveLineDrawing(Point2DInt p1, Point2DInt p2, int *numElements);
void parallelNaiveLineDrawing(PixelRGB **image, PixelRGB *color, Point2DInt p1, Point2DInt p2, int numThreads, ThreadPool *pool);
void parallelNaive(void *lineInfo);

Point2DInt *DDALineDrawing(Point2DInt p1, Point2DInt p2, int *numElements);
void parallelDDALineDrawing(PixelRGB **image, PixelRGB color, Point2DInt p1, Point2DInt p2, int numThreads);
void *parallelDDA(void *lineInfo);

Point2DInt *BresenhamLineDrawing(Point2DInt p1, Point2DInt p2, int *numElements);
void parallelBresenhamLineDrawing(PixelRGB **image, PixelRGB color, Point2DInt p1, Point2DInt p2, int numThreads);
void *parallelBresenham(void *lineInfo);

void drawFace(PixelRGB **image, PixelRGB color, FaceNode face, Point2DInt *vertices, int lineAlgorithm);
void drawFaceParallel(PixelRGB **image, PixelRGB *color, FaceNode face, Point2DInt *vertices, int lineAlgorithm, ThreadPool *pool, int numThreads);

#endif