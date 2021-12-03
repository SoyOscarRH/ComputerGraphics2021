#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "point.h"

double**translationMatrix(double tx, double ty, double tz){
    double**M = (double**)calloc(4, sizeof(double*));
    for(int i = 0; i < 4; i++){
        M[i] = (double*)calloc(4, sizeof(double));
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

double **scaleMatrix(double sx, double sy, double sz){
    double **M = (double**)calloc(4, sizeof(double*));
    for(int i = 0; i < 4; i++){
        M[i] = (double*)calloc(4, sizeof(double));
    }
    M[0][0] = sx;
    M[1][1] = sy;
    M[2][2] = sz;
    M[3][3] = 1;

    return M;
}

double **rotationXMatrix(double angleX){
    double **M = (double**)calloc(4, sizeof(double*));
    for(int i = 0; i < 4; i++){
        M[i] = (double*)calloc(4, sizeof(double));
    }

    double radiansAngleX = angleX * M_PI / 180.0;

    M[0][0] = 1;
    M[1][1] = cos(radiansAngleX);
    M[1][2] = -sin(radiansAngleX);
    M[2][1] = sin(radiansAngleX);
    M[2][2] = cos(radiansAngleX);
    M[3][3] = 1;

    return M;
}

double **rotationYMatrix(double angleY){

    double **M = (double**)calloc(4, sizeof(double*));
    for(int i = 0; i < 4; i++){
        M[i] = (double*)calloc(4, sizeof(double));
    }

    double radiansAngleY = angleY * M_PI / 180.0;

    M[0][0] = cos(radiansAngleY);
    M[0][2] = -sin(radiansAngleY);
    M[1][1] = 1;
    M[2][0] = sin(radiansAngleY);
    M[2][2] = cos(radiansAngleY);
    M[3][3] = 1;

    return M;
}

double **rotationZMatrix(double angleZ){

    double **M = (double**)calloc(4, sizeof(double*));
    for(int i = 0; i < 4; i++){
        M[i] = (double*)calloc(4, sizeof(double));
    }

    double radiansAngleZ = angleZ * M_PI / 180.0;

    M[0][0] = cos(radiansAngleZ);
    M[0][1] = -sin(radiansAngleZ);
    M[1][0] = sin(radiansAngleZ);
    M[1][1] = cos(radiansAngleZ);
    M[2][2] = 1;
    M[3][3] = 1;

    return M;

}

double **perspectiveMatrixZAxis(double focalDistance){
    
    double **M = (double**)calloc(4, sizeof(double*));
    for(int i = 0; i < 4; i++){
        M[i] = (double*)calloc(4, sizeof(double));
    }

    M[0][0] = focalDistance;
    M[1][1] = focalDistance;
    M[2][2] = focalDistance;
    M[3][2] = 1;

    return M;    
}

double **perspectiveMatrixFOV(double fov, double far, double near){
    double **M = (double**)calloc(4, sizeof(double*));
    
    double S = ((fov/2)*(M_PI/180));

    for(int i = 0; i < 4; i++){
        M[i] = (double*)calloc(4, sizeof(double));
    }

    M[0][0] = S;
    M[1][1] = S;
    M[2][2] = -far/(far-near);;
    M[2][3] = -(far*near)/(far-near);;
    M[3][2] = -1;

    return M;        
}

double**matrix4x4Multiplication(double**A, double**B){
    
    double**C = (double**)calloc(4, sizeof(double*));
    for(int i = 0; i < 4; i++){
        C[i] = (double*)calloc(4, sizeof(double));
    }
    
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            double sum = 0;
            for(int k = 0; k < 4; k++){
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

    return C;
}

Point3DHomogeneous matrixPoint3DHMultiplication(double**A, Point3DHomogeneous point){
    Point3DHomogeneous res;

    res.x = A[0][0]*point.x + A[0][1]*point.y + A[0][2]*point.z + A[0][3]*point.w;
    res.y = A[1][0]*point.x + A[1][1]*point.y + A[1][2]*point.z + A[1][3]*point.w;
    res.z = A[2][0]*point.x + A[2][1]*point.y + A[2][2]*point.z + A[2][3]*point.w;
    res.w = A[3][0]*point.x + A[3][1]*point.y + A[3][2]*point.z + A[3][3]*point.w;

    return res;

}

void matrixPoint3DHMultiplicationInPlace(double**A, Point3DHomogeneous *point){
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


void printMatrix(double **M){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            printf("%f ", M[i][j]);
        }
        printf("\n");
    }      
}