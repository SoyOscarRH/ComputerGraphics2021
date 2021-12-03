#ifndef PRIMITIVES_H
#define PRIMITIVES_H

Point2DInt *lineDrawingSpecialCases(Point2DInt p1, Point2DInt p2, int *numElements);

Point2DInt *BresenhamLineDrawing(Point2DInt p1, Point2DInt p2, int *numElements);

typedef struct HorizontalLineInfoNode{
    int yValue;
    int xMin;
    int xMax;
    struct HorizontalLineInfoNode *next;
} HorizontalLineInfoNode;

typedef struct ScanlineNode{
    Point2DInt *points;
    int numPoints;
    double zFirst;
    double zLast;
    struct ScanlineNode *next;
} ScanlineNode;

typedef struct ScanlineColorsNode{
    Point2DInt *points;
    int numPoints;
    double zFirst;
    double zLast;
    Color firstColor;
    Color lastColor;
    struct ScanlineColorsNode *next;
} ScanlineColorsNode;

Point2DInt *fillTriangle(Point2DInt *line1, int sizeP1, Point2DInt *line2, int sizeP2, Point2DInt *line3, int sizeP3, int *numElements);
ScanlineNode *fillTriangleT(Point2DInt *line1, double *zValues1, int sizeL1, Point2DInt *line2, double *zValues2, int sizeL2, Point2DInt *line3, double *zValues3, int sizeL3);
ScanlineColorsNode *fillTriangleColors(Point2DInt *line1, double *zValues1, Color *colorsL1, int sizeL1, Point2DInt *line2, double *zValues2, Color *colorsL2, int sizeL2, Point2DInt *line3, double *zValues3, Color *colorsL3, int sizeL3);


#endif