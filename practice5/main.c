#include <stdio.h>
#include <stdlib.h>
#include "color.h"
#include "fileManagement.h"
#include "parametric.h"
#include "point.h"
#include "primitives.h"
#include "matrix.h"

int main(){

    int m = 4;
    int n = 6;

    Point3D **controlPoints = (Point3D**)malloc(m*sizeof(Point3D*));
    for(int i = 0; i < m ; i++){
        controlPoints[i] = (Point3D*)malloc(n*sizeof(Point3D));
    }

    controlPoints[0][0] = (Point3D){.x = 50.0, .y = 50, .z = 50};
    controlPoints[0][1] = (Point3D){.x = 450.0, .y = 75, .z = 50};
    controlPoints[0][2] = (Point3D){.x = 700.0, .y = 0, .z = 50};
    controlPoints[0][3] = (Point3D){.x = 1100.0, .y = 200, .z = 50};
    controlPoints[0][4] = (Point3D){.x = 1500.0, .y = 100, .z = 50};
    controlPoints[0][5] = (Point3D){.x = 1870.0, .y = 20, .z = 50};

    controlPoints[1][0] = (Point3D){.x = 200.0, .y = 400, .z = 50};
    controlPoints[1][1] = (Point3D){.x = 125.0, .y = 120, .z = 500};
    controlPoints[1][2] = (Point3D){.x = 375.0, .y = 120, .z = 500};
    controlPoints[1][3] = (Point3D){.x = 1200.0, .y = 70, .z = 500};
    controlPoints[1][4] = (Point3D){.x = 225.0, .y = 0, .z = 500};
    controlPoints[1][5] = (Point3D){.x = 1500, .y = 450, .z = 50};

    controlPoints[2][0] = (Point3D){.x = 400.0, .y = 700, .z = 50};
    controlPoints[2][1] = (Point3D){.x = 125.0, .y = 140, .z = 500};
    controlPoints[2][2] = (Point3D){.x = 450.0, .y = 90, .z = 500};
    controlPoints[2][3] = (Point3D){.x = 450.0, .y = 90, .z = 500};
    controlPoints[2][4] = (Point3D){.x = 1200.0, .y = 800, .z = 500};
    controlPoints[2][5] = (Point3D){.x = 1400, .y = 900, .z = 50};

    controlPoints[3][0] = (Point3D){.x = 50.0, .y = 900, .z = 50};
    controlPoints[3][1] = (Point3D){.x = 500.0, .y = 700, .z = 50};
    controlPoints[3][2] = (Point3D){.x = 800.0, .y = 900, .z = 50};
    controlPoints[3][3] = (Point3D){.x = 1200.0, .y = 1080, .z = 50};
    controlPoints[3][4] = (Point3D){.x = 1500,0, .y = 900, .z = 50};
    controlPoints[3][5] = (Point3D){.x = 1870.0, .y = 1030, .z = 50};

    int width = 1920;
    int height = 1080;

    Color **image = (Color**)calloc(height, sizeof(Color*));
    for(int i = 0; i < height; i++){
        image[i] = (Color*)calloc(width, sizeof(Color));
    }

    Color curve3DColor = {.R = 185, .G = 191, .B = 15};
    //Color internalColor = {.R = 24, .G = 122, .B = 15}; 
    Color internalColor = {.R = 128, .G = 13, .B = 70}; 

    int numLinesU, numLinesV;
    printf("Num lines u: ");
    scanf("%d", &numLinesU);
    printf("Num lines v: ");
    scanf("%d", &numLinesV); 

    double uIncrement = 1.0/(numLinesU-1);
    double vIncrement = 1.0/(numLinesV-1);

    Point3D **surfacePoints = (Point3D**)malloc(numLinesU*sizeof(Point3D*));
    for(int i = 0; i < numLinesU; i++){
        surfacePoints[i] = (Point3D*)malloc(numLinesV*sizeof(Point3D));
    }

    double u = 0.0;
    for(int i = 0; i < numLinesU; i++, u+=uIncrement){
        double v = 0.0;
        for(int j = 0; j < numLinesV; j++, v+=vIncrement){
            surfacePoints[i][j] = calculateBezierSurfacePoint(u, v, controlPoints, m, n);
        }
    }

    /*
    for(int i = 0; i < numLinesU; i++){
        for(int j = 0; j < numLinesV - 1; j++){
            Point2DInt p2D1 = {.x = surfacePoints[i][j].x, .y = surfacePoints[i][j].y};
            Point2DInt p2D2 = {.x = surfacePoints[i][j+1].x, .y = surfacePoints[i][j+1].y};
            int numElements;
            Point2DInt *curvePoints = BresenhamLineDrawing(p2D1, p2D2, &numElements);
            setPixelCurve(image, width, height, curvePoints, numElements, &internalColor);
            free(curvePoints);
        }
    }*/

    /*
    for(int i = 0; i < numLinesV; i++){
        for(int j = 0; j < numLinesU - 1; j++){
            Point2DInt p2D1 = {.x = surfacePoints[j][i].x, .y = surfacePoints[j][i].y};
            Point2DInt p2D2 = {.x = surfacePoints[j+1][i].x, .y = surfacePoints[j+1][i].y};
            int numElements;
            Point2DInt *curvePoints = BresenhamLineDrawing(p2D1, p2D2, &numElements);
            setPixelCurve(image, width, height, curvePoints, numElements, &internalColor);
            free(curvePoints);
        }
    }*/

    for(int i = 0; i < numLinesU - 1; i++){
        for(int j = 0; j < numLinesV - 1; j++){
            Point2DInt p1 = {.x = surfacePoints[i][j].x, .y = surfacePoints[i][j].y};
            Point2DInt p2H = {.x = surfacePoints[i+1][j].x, .y = surfacePoints[i+1][j].y};
            int numElementsH;
            Point2DInt *curvePointsH = BresenhamLineDrawing(p1, p2H, &numElementsH);
            setPixelCurve(image, width, height, curvePointsH, numElementsH, &internalColor);
            free(curvePointsH);

            Point2DInt p2V = {.x = surfacePoints[i][j+1].x, .y = surfacePoints[i][j+1].y};
            int numElementsV;
            Point2DInt *curvePointsV = BresenhamLineDrawing(p1, p2V, &numElementsV);
            setPixelCurve(image, width, height, curvePointsV, numElementsV, &internalColor);
            free(curvePointsV);

            Point2DInt p2D = {.x = surfacePoints[i+1][j+1].x, .y = surfacePoints[i+1][j+1].y};
            int numElementsD;
            Point2DInt *curvePointsD = BresenhamLineDrawing(p1, p2D, &numElementsD);
            setPixelCurve(image, width, height, curvePointsD, numElementsD, &internalColor);
            free(curvePointsD);
        }
    }

    for(int i = 0; i < numLinesV - 1; i++){
        Point2DInt p1 = {.x = surfacePoints[numLinesU-1][i].x, .y = surfacePoints[numLinesU-1][i].y};
        Point2DInt p2 = {.x = surfacePoints[numLinesU-1][i+1].x, .y = surfacePoints[numLinesU-1][i+1].y};
        int numElements;
        Point2DInt *curvePoints = BresenhamLineDrawing(p1, p2, &numElements);
        setPixelCurve(image, width, height, curvePoints, numElements, &internalColor);
        free(curvePoints);
    }

    for(int i = 0; i < numLinesU - 1; i++){
        Point2DInt p1 = {.x = surfacePoints[i][numLinesV-1].x, .y = surfacePoints[i][numLinesV-1].y};
        Point2DInt p2 = {.x = surfacePoints[i+1][numLinesV-1].x, .y = surfacePoints[i+1][numLinesV-1].y};
        int numElements;
        Point2DInt *curvePoints = BresenhamLineDrawing(p1, p2, &numElements);
        setPixelCurve(image, width, height, curvePoints, numElements, &internalColor);
        free(curvePoints);
    }    


    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            Point2DInt p2D = {.x = controlPoints[i][j].x, .y = controlPoints[i][j].y };
            if( ( p2D.x >= 0 && p2D.x < width) && (p2D.y >= 0 && p2D.y < height) ){
                image[p2D.y][p2D.x] = (Color){.R = curve3DColor.R, .G = curve3DColor.G, .B = curve3DColor.B};
            }            
        }
    } 

    writePPMP3("testCurves.ppm", image, height, width);

    return 0;
}