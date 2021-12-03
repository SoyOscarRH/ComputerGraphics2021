#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "color.h"
#include "light.h"
#include "point.h"


typedef struct Image{
    Color **raster;
    double **zBuffer;
} Image;

typedef struct Object3D{
    Point3D *vertices;
    int **edges;
    int **faces;
    int numVertices;
    int numEdges;
    int numFaces;
    Point3D *faceNormals;
    Point3D *vertexNormals;
    Color edgesColor;
    Color *facesColors;
    double *diffuseF;
    double *specularF;

    int **vertexFacesAdjacency;
    
    Point3DHomogeneous *verticesH;
    Point3D *perspectiveVertices;
    Point3D *screenNDCVertices;
    Point2DInt *rasterVertices;

    Point3D minPerspective;
    Point3D maxPerspective;

    Point3D *perspectiveFaceNormals;
    int *visiblePerspectiveFaces;

} Object3D;

Object3D readObjFile(const char *nameFile);

void calculateFaceNormals(Object3D *obj);
void calculateVertexNormals(Object3D *obj);

void calculatePoint3DHomogeneous(Object3D *obj);
void applyTransformation(Object3D *obj, double **transformationMatrix);
void returnFromHomogeneous(Object3D *obj, Point3D *minP, Point3D *maxP);

void calculatePerspectiveVertices(Object3D *obj, double **perspectiveMatrix);
void calculateNDCVertices(Object3D *obj, double minCanvasWidth, double maxCanvasWidth, double minCanvasHeight, double maxCanvasHeight);
void calculateRasterVertices(Object3D *obj, int width, int height);

void checkVisibleFacesPerspective(Object3D *obj, Point3D *perspectiveVertices, Point3D cameraVector);

void addSpecularReflection(Object3D *obj, int numFace, int numVertexTriangle, Point3D pixelPos, Color *pixelColor, LightNode *lightNode);

void drawFaces(Object3D *obj, Point2DInt *points2D, Image *image, Point3D cameraVector, int lineDrawingAlgorithm, int fillFaces, LightNode *lights);

void drawFrame(const char *nameFrame, Object3D *obj, LightNode *lights, double **transformationMatrix, double **perspectiveMatrix, Point3D cameraVector, int height, int width);

typedef struct intNode{
    int value;
    struct intNode *next;
} intNode;

typedef struct FaceNode{
    intNode *faceList;
    int numVertices;
    struct FaceNode *next;
} FaceNode;


void drawPoints(Point2DInt *points, double *zValues, int numPoints, Image *image, Color color);
void drawPointsColors(Point2DInt *points, double *zValues, int numPoints, Image *image, Color *colors);

#endif