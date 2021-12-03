#ifndef MATRIX_H
#define MATRIX_H

#include "point.h"

float **matrix4x4Multiplication(float **A, float **B);
Point3DHomogeneous matrixPoint3DHMultiplication(float **A, Point3DHomogeneous point);
void matrixPoint3DHMultiplicationInPlace(float **A, Point3DHomogeneous *point);

float **translationMatrix(float tx, float ty, float tz);
float **scaleMatrix(float sx, float sy, float sz);
float **rotationXMatrix(float angleX);
float **rotationYMatrix(float angleY);
float **rotationZMatrix(float angleZ);

float **perspectiveMatrixZAxis(float focalDistance);

void printMatrix(float **M);

#endif