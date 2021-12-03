#ifndef RENDER_H
#define RENDER_H

#include "point.h"
#include "face.h"
#include "pixel.h"

typedef struct Object3D{
    Point3DNode *vertices;
    int numVertices;
    FaceNode *faces;
    Point3D minPoint;
    Point3D maxPoint;
    Point3D midPoint;
    float xMin;
    float xMax;
    float yMin;
    float yMax;
    float zMin;
    float zMax;
    float xMid;
    float yMid;
    float zMid;
} Object3D;

void readObjFile(Point3DNode **vertices, FaceNode **faces, const char *nameFile, int *numVertices, float *xMin, float *xMax, float *yMin, float *yMax, float *zMin, float *zMax);
Object3D *createObject3D(const char *nameFile);
Point2D *reduce3Dto2D(Point3DNode *vertices, int numVertices);
void scale2DPoints(Point2D *vertices, int numVertices, float scale);
void center2DPoints(Point2D *vertices, int numVertices, float xCenter, float yCenter, float xMin, float xMax, float yMin, float yMax);

void setLine(Point2DInt *points, int numPoints, PixelRGB **image, PixelRGB color);

void renderFrames(Object3D *obj, float **transformationMatrix, int firstFrame, int lastFrame, int typeProjection, int width, int height, int lineAlgorithm, PixelRGB color);

void objToRasterXY(Object3D *obj, const char *ppmFile, int width, int height, int lineDrawingAlgorithm, PixelRGB lineColor);
void objToRasterXYParallel(const char *objFile, const char *ppmFile, int width, int height, int lineDrawingAlgorithm, PixelRGB lineColor, int numThreads);

void objToRasterXYPerspective(Object3D *obj, const char *ppmFile, int width, int height, int lineDrawingAlgorithm, PixelRGB lineColor);

void pruebasLineas(PixelRGB **image, char *filename, int width, int height, int numLineas);

#endif