#ifndef RENDER_H
#define RENDER_H

#include "face.h"
#include "pixel.h"
#include "point.h"

void readObjFile (Point3DNode **vertices, FaceNode **faces,
                  const char *nameFile, int *numVertices, float *xMin,
                  float *xMax, float *yMin, float *yMax, float *zMin,
                  float *zMax);
Point2D *reduce3Dto2D (Point3DNode *vertices, int numVertices);
void scale2DPoints (Point2D *vertices, int numVertices, float scale);
void center2DPoints (Point2D *vertices, int numVertices, float xCenter,
                     float yCenter, float xMin, float xMax, float yMin,
                     float yMax);

void setLine (Point2DInt *points, int numPoints, PixelRGB **image,
              PixelRGB color);

void objToRasterXY (const char *objFile, const char *ppmFile, int width,
                    int height, int lineDrawingAlgorithm, PixelRGB lineColor);
void objToRasterXYParallel (const char *objFile, const char *ppmFile,
                            int width, int height, int lineDrawingAlgorithm,
                            PixelRGB lineColor, int numThreads);

void pruebasLineas (PixelRGB **image, char *filename, int width, int height,
                    int numLineas);

#endif