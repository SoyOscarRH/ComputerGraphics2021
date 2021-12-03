#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"

float **matrix4x4Multiplication(float **A, float **B){
    
    float **C = (float**)calloc(4, sizeof(float*));
    for(int i = 0; i < 4; i++){
        C[i] = (float*)calloc(4, sizeof(float));
    }
    
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            float sum = 0;
            for(int k = 0; k < 4; k++){
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

    return C;
}

Point3DHomogeneous matrixPoint3DHMultiplication(float **A, Point3DHomogeneous point){
    Point3DHomogeneous res;

    res.x = A[0][0]*point.x + A[0][1]*point.y + A[0][2]*point.z + A[0][3]*point.w;
    res.y = A[1][0]*point.x + A[1][1]*point.y + A[1][2]*point.z + A[1][3]*point.w;
    res.z = A[2][0]*point.x + A[2][1]*point.y + A[2][2]*point.z + A[2][3]*point.w;
    res.w = A[3][0]*point.x + A[3][1]*point.y + A[3][2]*point.z + A[3][3]*point.w;

    return res;

}

void matrixPoint3DHMultiplicationInPlace(float **A, Point3DHomogeneous *point){
    Point3DHomogeneous res;
    res.x = A[0][0]*point->x + A[0][1]*point->y + A[0][2]*point->z + A[0][3]*point->w;
    res.y = A[1][0]*point->x + A[1][1]*point->y + A[1][2]*point->z + A[1][3]*point->w;
    res.z = A[2][0]*point->x + A[2][1]*point->y + A[2][2]*point->z + A[2][3]*point->w;
    res.w = A[3][0]*point->x + A[3][1]*point->y + A[3][2]*point->z + A[3][3]*point->w;    
    
    point->x = res.x;
    point->y = res.y;
    point->z = res.z;
    point->w = res.w;
}


float **translationMatrix(float tx, float ty, float tz){
    float **M = (float**)calloc(4, sizeof(float*));
    for(int i = 0; i < 4; i++){
        M[i] = (float*)calloc(4, sizeof(float));
    }
    M[0][0] = 1;
    M[0][3] = tx;
    M[1][1] = 1;
    M[1][3] = ty;
    M[2][2] = 1;
    M[2][3] = tz;
    M[3][3] = 1;

    return M;
}

float **scaleMatrix(float sx, float sy, float sz){
    float **M = (float**)calloc(4, sizeof(float*));
    for(int i = 0; i < 4; i++){
        M[i] = (float*)calloc(4, sizeof(float));
    }
    M[0][0] = sx;
    M[1][1] = sy;
    M[2][2] = sz;
    M[3][3] = 1;

    return M;
}

float **rotationXMatrix(float angleX){
    float **M = (float**)calloc(4, sizeof(float*));
    for(int i = 0; i < 4; i++){
        M[i] = (float*)calloc(4, sizeof(float));
    }

    float radiansAngleX = angleX * M_PI / 180.0;

    M[0][0] = 1;
    M[1][1] = cos(radiansAngleX);
    M[1][2] = -sin(radiansAngleX);
    M[2][1] = sin(radiansAngleX);
    M[2][2] = cos(radiansAngleX);
    M[3][3] = 1;

    return M;
}

float **rotationYMatrix(float angleY){

    float **M = (float**)calloc(4, sizeof(float*));
    for(int i = 0; i < 4; i++){
        M[i] = (float*)calloc(4, sizeof(float));
    }

    float radiansAngleY = angleY * M_PI / 180.0;

    M[0][0] = cos(radiansAngleY);
    M[0][2] = -sin(radiansAngleY);
    M[1][1] = 1;
    M[2][0] = sin(radiansAngleY);
    M[2][2] = cos(radiansAngleY);
    M[3][3] = 1;

    return M;
}

float **rotationZMatrix(float angleZ){

    float **M = (float**)calloc(4, sizeof(float*));
    for(int i = 0; i < 4; i++){
        M[i] = (float*)calloc(4, sizeof(float));
    }

    float radiansAngleZ = angleZ * M_PI / 180.0;

    M[0][0] = cos(radiansAngleZ);
    M[0][1] = -sin(radiansAngleZ);
    M[1][0] = sin(radiansAngleZ);
    M[1][1] = cos(radiansAngleZ);
    M[2][2] = 1;
    M[3][3] = 1;

    return M;

}

float **perspectiveMatrixZAxis(float focalDistance){
    
    float **M = (float**)calloc(4, sizeof(float*));
    for(int i = 0; i < 4; i++){
        M[i] = (float*)calloc(4, sizeof(float));
    }

    M[0][0] = focalDistance;
    M[1][1] = focalDistance;
    M[2][2] = focalDistance;
    M[3][2] = 1;

    return M;    
}

void printMatrix(float **M){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            printf("%f ", M[i][j]);
        }
        printf("\n");
    }      
}