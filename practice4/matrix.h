#ifndef MATRIX_H
#define MATRIX_H

#include "point.h"

double **translationMatrix(double tx, double ty, double tz);
double **scaleMatrix(double sx, double sy, double sz);
double **rotationXMatrix(double angleX);
double **rotationYMatrix(double angleY);
double **rotationZMatrix(double angleZ);

double **perspectiveMatrixZAxis(double focalDistance);
double **perspectiveMatrixFOV(double fov, double far, double near);

double **matrix4x4Multiplication(double **A, double **B);
Point3DHomogeneous matrixPoint3DHMultiplication(double **A, Point3DHomogeneous point);
void matrixPoint3DHMultiplicationInPlace(double **A, Point3DHomogeneous *point);

void printMatrix(double **M);

#endif