#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "point.h"

Point2DInt transformPointToFirstOctant(Point2DInt p, float m){
    
    Point2DInt newP;

    //2nd octant.
    if(m > 1){
        newP.x = p.y;
        newP.y = p.x;
    //4th octant.
    }else if(m < -1){
        newP.x = p.y;
        newP.y = p.x;
        newP.y *= -1;
    //3rd octant.
    }else{
        newP.x = p.x;
        newP.y = p.y * -1;
    }

    return newP;
}

Point2DInt transformPointFromFirstOctant(Point2DInt p, float m){

    Point2DInt newP;

    //2nd octant.
    if(m > 1){
        newP.x = p.y;
        newP.y = p.x;
    //4th octant.
    }else if(m < -1){
        newP.x = p.y*-1;
        newP.y = p.x;
    //3rd octant.
    }else{
        newP.x = p.x;
        newP.y = p.y * -1;
    }

    return newP;

}

void swapPoints(Point2DInt *p1, Point2DInt *p2){    
    p1->x ^= p2->x;
    p2->x ^= p1->x;
    p1->x ^= p2->x;

    p1->y ^= p2->y;
    p2->y ^= p1->y;
    p1->y ^= p2->y;
}

void transformArrayPoint2DFloatToInt(Point2D *floatPoints, Point2DInt *intPoints, int numPoints){
    for(int i = 0; i < numPoints; i++){
        intPoints[i].x = (int)round(floatPoints[i].x);
        intPoints[i].y = (int)round(floatPoints[i].y);
    }
}


Point3DHomogeneous *convertToHomogeneous3D(Point3DNode *points, int numPoints){
    
    Point3DHomogeneous *homogeneousVertices = (Point3DHomogeneous*)calloc(numPoints, sizeof(Point3DHomogeneous));
    Point3DNode *aux = points;
    int i = 0;
    while(aux){
        homogeneousVertices[i].x = aux->point.x;
        homogeneousVertices[i].y = aux->point.y;
        homogeneousVertices[i].z = aux->point.z;
        homogeneousVertices[i].w = 1;
        i++;
        aux = aux->next;
    }
    
    /*for(int i = 0; i < numPoints; i++){
        homogeneousVertices[i].x = points[i].x;
        homogeneousVertices[i].y = points[i].y;
        homogeneousVertices[i].z = points[i].z;
        homogeneousVertices[i].w = 1;
    }*/
    return homogeneousVertices;
}

void returnFromHomogeneous3D(Point3DNode *points, Point3DHomogeneous *homogeneousPoints, int numPoints){
    
    Point3DNode *aux = points;
    int i = 0;
    while(aux){
        aux->point.x = homogeneousPoints[i].x / homogeneousPoints[i].w;
        aux->point.y = homogeneousPoints[i].y / homogeneousPoints[i].w;
        aux->point.z = homogeneousPoints[i].z / homogeneousPoints[i].w;
        i++;
        aux = aux->next;
    }    
    
    /*
    for(int i = 0; i < numPoints; i++){
        points[i].x = homogeneousPoints[i].x / homogeneousPoints[i].w;
        points[i].y = homogeneousPoints[i].y / homogeneousPoints[i].w;
        points[i].z = homogeneousPoints[i].z / homogeneousPoints[i].w;
    }
    */
}

Point3DNode *returnFromHomogeneous3DNew(Point3DHomogeneous *homogeneousPoints, int numPoints){
    
    Point3DNode *newList = (Point3DNode*)calloc(1, sizeof(Point3DNode));
    newList->point.x = homogeneousPoints[0].x / homogeneousPoints[0].w;
    newList->point.y = homogeneousPoints[0].y / homogeneousPoints[0].w;
    newList->point.z = homogeneousPoints[0].z / homogeneousPoints[0].w;
    newList->next = NULL;

    Point3DNode *first = newList;

    for(int i = 1; i < numPoints; i++){
        Point3DNode *aux = (Point3DNode*)calloc(1, sizeof(Point3DNode));
        aux->point.x = homogeneousPoints[i].x / homogeneousPoints[i].w;
        aux->point.y = homogeneousPoints[i].y / homogeneousPoints[i].w;
        aux->point.z = homogeneousPoints[i].z / homogeneousPoints[i].w;
        aux->next = NULL;
        newList->next = aux;
        newList = aux;
    }
    
    return first;
}
