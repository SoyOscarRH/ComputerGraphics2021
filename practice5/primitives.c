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
