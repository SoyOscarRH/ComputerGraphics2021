#include "point.h"

void swapPoints(Point2DInt *p1, Point2DInt *p2){    
    p1->x ^= p2->x;
    p2->x ^= p1->x;
    p1->x ^= p2->x;

    p1->y ^= p2->y;
    p2->y ^= p1->y;
    p1->y ^= p2->y;
}

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
