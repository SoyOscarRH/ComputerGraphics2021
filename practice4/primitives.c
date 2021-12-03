#include <stdio.h>
#include <stdlib.h>
#include "point.h"
#include "primitives.h"

Point2DInt *lineDrawingSpecialCases(Point2DInt p1, Point2DInt p2, int *numElements){

    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    Point2DInt *points;

    if(dx == 0){
        if(p1.y > p2.y){
            swapPoints(&p1, &p2);
        }

        *numElements = p2.y - p1.y + 1;
        points = (Point2DInt*)malloc((*numElements)*sizeof(Point2DInt));

        int i, y;
        for(i = 0, y = p1.y; y < p2.y + 1; i++, y++){
            points[i].x = p1.x;
            points[i].y = y;           
        }
    }else if(dy == 0){
        if(p1.x > p2.x){
            swapPoints(&p1, &p2);
        }

        *numElements = p2.x - p1.x + 1;
        points = (Point2DInt*)malloc((*numElements)*sizeof(Point2DInt));

        int i, x;
        for(i = 0, x = p1.x; x < p2.x + 1; i++, x++){
            points[i].x = x;
            points[i].y = p1.y;               
        }
    }else if((float)dy/dx == 1){
        if(p1.x > p2.x){
            swapPoints(&p1, &p2);
        }
        
        *numElements = p2.x - p1.x + 1;
        points = (Point2DInt*)malloc((*numElements)*sizeof(Point2DInt));

        int i, x, y;
        for(i = 0, x = p1.x, y = p1.y; x < p2.x + 1; i++, x++, y++){
            points[i].x = x;
            points[i].y = y;
        }
    }else if((float)dy/dx == -1){
        if(p1.x > p2.x){
            swapPoints(&p1, &p2);
        }
        
        *numElements = p2.x - p1.x + 1;
        points = (Point2DInt*)malloc((*numElements)*sizeof(Point2DInt));

        int i, x, y;
        for(i = 0, x = p1.x, y = p1.y; x < p2.x + 1; i++, x++, y--){
            points[i].x = x;
            points[i].y = y;
        }
    }else{
        points = NULL;
    }

    return points;
}

Point2DInt *BresenhamLineDrawing(Point2DInt p1, Point2DInt p2, int *numElements){
    

    Point2DInt *points = lineDrawingSpecialCases(p1, p2, numElements);

    if(!points){

        int dx = p2.x - p1.x;
        int dy = p2.y - p1.y;

        float m = (float)dy/dx;

        if(m > 0 && m < 1){
            if(p1.x > p2.x){
                swapPoints(&p1, &p2);
                dx *= -1;
                dy *= -1;
            }

            *numElements = 1+p2.x - p1.x;
            points = (Point2DInt*)malloc((*numElements)*sizeof(Point2DInt));

            int d = 2*dy - dx;
            int y = p1.y;
            int i = 0;
            int x;
            points[i].x = p1.x;
            points[i].y = y;
            for(i = 1, x = p1.x+1; x <= p2.x; i++, x++){
                if(d < 0){
                    d += 2*dy;
                }else{
                    d += 2*(dy-dx);
                    y++;
                }
                points[i].x = x;
                points[i].y = y;
            } 
        }else{
            Point2DInt p1t = transformPointToFirstOctant(p1, m);
            Point2DInt p2t = transformPointToFirstOctant(p2, m);

            if(p1t.x > p2t.x){
                swapPoints(&p1t, &p2t);
            }

            Point2DInt *transformedPoints = (Point2DInt*)malloc((1+p2t.x - p1t.x)*sizeof(Point2DInt));
            int counter = 1;
            int dxT = p2t.x - p1t.x;
            int dyT = p2t.y - p1t.y;
            int y = p1t.y;
            
            int d = 2*dyT - dxT;
            transformedPoints[0].x = p1t.x;
            transformedPoints[0].y = p1t.y;

            for(int i = p1t.x + 1; i <= p2t.x; i++){
                if(d < 0){
                    d += 2*dyT;
                }else{
                    d += 2*(dyT - dxT);
                    y++;
                }
                transformedPoints[counter].x = i;
                transformedPoints[counter].y = y;
                counter++;
            }

            *numElements = 1+p2t.x - p1t.x;
            points = (Point2DInt*)malloc((*numElements)*sizeof(Point2DInt));

            for(int i = 0; i <= p2t.x - p1t.x; i++){
                points[i] = transformPointFromFirstOctant(transformedPoints[i], m);
            }
            free(transformedPoints);
        }
    }
    return points;   
}

Point2DInt *fillTriangle(Point2DInt *line1, int sizeP1, Point2DInt *line2, int sizeP2, Point2DInt *line3, int sizeP3, int *numElements){

    int totalSize = sizeP1 + sizeP2 + sizeP3;
    Point2DInt *finalList = (Point2DInt*)calloc(totalSize, sizeof(Point2DInt));

    //printf("%d, %d, %d\n", sizeP1, sizeP2, sizeP3);

    int totalCounter = 0;
    for(int i = 0; i < sizeP1; i++){
        finalList[totalCounter].x = line1[i].x;
        finalList[totalCounter].y = line1[i].y;
        totalCounter++;
    }

    for(int i = 0; i < sizeP2; i++){
        finalList[totalCounter].x = line2[i].x;
        finalList[totalCounter].y = line2[i].y;
        totalCounter++;
    }

    for(int i = 0; i < sizeP3; i++){
        finalList[totalCounter].x = line3[i].x;
        finalList[totalCounter].y = line3[i].y;
        totalCounter++;
    }

    mergeSort2DIntByY(finalList, 0, totalSize);

    int currentY = finalList[0].y;
    int lastX = finalList[0].x;

    HorizontalLineInfoNode *first = NULL;
    HorizontalLineInfoNode *aux;
    int internalPixels = 0;

    for(int i = 1; i < totalSize; i++){
        if(finalList[i].y == currentY){
            if(finalList[i].x - lastX > 1){
                if(!first){
                    first = (HorizontalLineInfoNode*)calloc(1, sizeof(HorizontalLineInfoNode));
                    first->yValue = currentY;
                    first->xMin = lastX + 1;
                    first->xMax = finalList[i].x - 1;
                    first->next = NULL;
                }else{
                    aux = (HorizontalLineInfoNode*)calloc(1, sizeof(HorizontalLineInfoNode));
                    aux->yValue = currentY;
                    aux->xMin = lastX + 1;
                    aux->xMax = finalList[i].x - 1;
                    aux->next = first;
                    first = aux;
                }
                internalPixels += finalList[i].x - 1 - lastX;
            }
        }else{
            currentY = finalList[i].y;
        }
        lastX = finalList[i].x;
    }

    free(finalList);

    Point2DInt *internalPoints = (Point2DInt*)calloc(internalPixels, sizeof(Point2DInt));

    HorizontalLineInfoNode *aux2;
    aux = first;
    int counter = 0;
    while(aux){
        for(int i = aux->xMin; i <= aux->xMax; i++){
            internalPoints[counter].x = i;
            internalPoints[counter].y = aux->yValue;
            counter++;
        }
        aux2 = aux;
        aux = aux->next;
        free(aux2);
    }
    
    *numElements = internalPixels;
    return internalPoints;
}

ScanlineNode *fillTriangleT(Point2DInt *line1, double *zValues1, int sizeL1, Point2DInt *line2, double *zValues2, int sizeL2, Point2DInt *line3, double *zValues3, int sizeL3){
    
    int totalSize = sizeL1 + sizeL2 + sizeL3;
    Point3D *finalList = (Point3D*)calloc(totalSize, sizeof(Point3D));

    //printf("%d, %d, %d\n", sizeP1, sizeP2, sizeP3);

    int totalCounter = 0;
    for(int i = 0; i < sizeL1; i++){
        finalList[totalCounter].x = line1[i].x;
        finalList[totalCounter].y = line1[i].y;
        finalList[totalCounter].z = zValues1[i];
        totalCounter++;
    }

    for(int i = 0; i < sizeL2; i++){
        finalList[totalCounter].x = line2[i].x;
        finalList[totalCounter].y = line2[i].y;
        finalList[totalCounter].z = zValues2[i];
        totalCounter++;
    }

    for(int i = 0; i < sizeL3; i++){
        finalList[totalCounter].x = line3[i].x;
        finalList[totalCounter].y = line3[i].y;
        finalList[totalCounter].z = zValues3[i];
        totalCounter++;
    }

    mergeSort3DByY(finalList, 0, totalSize);

    int currentY = (int)finalList[0].y;
    int lastX = (int)finalList[0].x;

    ScanlineNode *first = NULL;
    ScanlineNode *aux;
    int internalPixels = 0;

    for(int i = 1; i < totalSize; i++){
        if(finalList[i].y == currentY){
            if(finalList[i].x - lastX > 1){
                if(!first){
                    first = (ScanlineNode*)calloc(1, sizeof(ScanlineNode));
                    first->points = (Point2DInt*)calloc(finalList[i].x - lastX + 1, sizeof(Point2DInt));
                    first->numPoints = finalList[i].x - lastX + 1;
                    int xVal = lastX;
                    for(int j = 0; j < finalList[i].x - lastX + 1; j++){
                        first->points[j].x = xVal;
                        first->points[j].y = currentY;
                        xVal++;
                    }
                    first->zFirst = finalList[i-1].z;
                    first->zLast = finalList[i].z;
                    first->next = NULL;
                }else{
                    aux = (ScanlineNode*)calloc(1, sizeof(ScanlineNode));
                    aux->points = (Point2DInt*)calloc(finalList[i].x - lastX + 1, sizeof(Point2DInt));
                    aux->numPoints = finalList[i].x - lastX + 1;
                    int xVal = lastX;
                    for(int j = 0; j < finalList[i].x - lastX + 1; j++){
                        aux->points[j].x = xVal;
                        aux->points[j].y = currentY;
                        xVal++;
                    }
                    aux->zFirst = finalList[i-1].z;
                    aux->zLast = finalList[i].z;
                    aux->next = first;
                    first = aux;
                }
                internalPixels += finalList[i].x - 1 - lastX;
            }
        }else{
            currentY = finalList[i].y;
        }
        lastX = finalList[i].x;
    }
    free(finalList);
    return first;
}

ScanlineColorsNode *fillTriangleColors(Point2DInt *line1, double *zValues1, Color *colorsL1, int sizeL1, Point2DInt *line2, double *zValues2, Color *colorsL2, int sizeL2, Point2DInt *line3, double *zValues3, Color *colorsL3, int sizeL3){
    
    int totalSize = sizeL1 + sizeL2 + sizeL3;
    
    Point3D *finalList = (Point3D*)calloc(totalSize, sizeof(Point3D));
    Color *finalListColors = (Color*)calloc(totalSize, sizeof(Color));

    int totalCounter = 0;
    for(int i = 0; i < sizeL1; i++){
        finalList[totalCounter].x = line1[i].x;
        finalList[totalCounter].y = line1[i].y;
        finalList[totalCounter].z = zValues1[i];
        finalListColors[totalCounter] = (Color){.R = colorsL1[i].R, .G = colorsL1[i].G, .B = colorsL1[i].B};
        totalCounter++;
    }

    for(int i = 0; i < sizeL2; i++){
        finalList[totalCounter].x = line2[i].x;
        finalList[totalCounter].y = line2[i].y;
        finalList[totalCounter].z = zValues2[i];
        finalListColors[totalCounter] = (Color){.R = colorsL2[i].R, .G = colorsL2[i].G, .B = colorsL2[i].B};
        totalCounter++;
    }

    for(int i = 0; i < sizeL3; i++){
        finalList[totalCounter].x = line3[i].x;
        finalList[totalCounter].y = line3[i].y;
        finalList[totalCounter].z = zValues3[i];
        finalListColors[totalCounter] = (Color){.R = colorsL3[i].R, .G = colorsL3[i].G, .B = colorsL3[i].B};
        totalCounter++;
    }

    mergeSort3DColorByY(finalList, finalListColors, 0, totalSize);

    int currentY = (int)finalList[0].y;
    int lastX = (int)finalList[0].x;

    ScanlineColorsNode *first = NULL;
    ScanlineColorsNode *aux;
    int internalPixels = 0;

    for(int i = 1; i < totalSize; i++){
        if(finalList[i].y == currentY){
            if(finalList[i].x - lastX > 1){
                if(!first){
                    first = (ScanlineColorsNode*)calloc(1, sizeof(ScanlineColorsNode));
                    first->points = (Point2DInt*)calloc(finalList[i].x - lastX + 1, sizeof(Point2DInt));
                    first->numPoints = finalList[i].x - lastX + 1;
                    int xVal = lastX;
                    for(int j = 0; j < finalList[i].x - lastX + 1; j++){
                        first->points[j].x = xVal;
                        first->points[j].y = currentY;
                        xVal++;
                    }
                    first->zFirst = finalList[i-1].z;
                    first->zLast = finalList[i].z;
                    first->firstColor = (Color){.R = finalListColors[i-1].R, .G = finalListColors[i-1].G, .B = finalListColors[i-1].B};
                    first->lastColor = (Color){.R = finalListColors[i].R, .G = finalListColors[i].G, .B = finalListColors[i].B};
                    first->next = NULL;
                }else{
                    aux = (ScanlineColorsNode*)calloc(1, sizeof(ScanlineColorsNode));
                    aux->points = (Point2DInt*)calloc(finalList[i].x - lastX + 1, sizeof(Point2DInt));
                    aux->numPoints = finalList[i].x - lastX + 1;
                    int xVal = lastX;
                    for(int j = 0; j < finalList[i].x - lastX + 1; j++){
                        aux->points[j].x = xVal;
                        aux->points[j].y = currentY;
                        xVal++;
                    }
                    aux->zFirst = finalList[i-1].z;
                    aux->zLast = finalList[i].z;
                    aux->firstColor = (Color){.R = finalListColors[i-1].R, .G = finalListColors[i-1].G, .B = finalListColors[i-1].B};
                    aux->lastColor = (Color){.R = finalListColors[i].R, .G = finalListColors[i].G, .B = finalListColors[i].B};                    
                    aux->next = first;
                    first = aux;
                }
                internalPixels += finalList[i].x - 1 - lastX;
            }
        }else{
            currentY = finalList[i].y;
        }
        lastX = finalList[i].x;
    }
    free(finalList);
    free(finalListColors);
    return first;
}
