#ifndef PARAMETRIC_H
#define PARAMETRIC_H

#include "point.h"

double evaluatePolynomialDegree3(double t, double a, double b, double c, double d);
Point2D *get2DParametricValues(int n, double ax, double bx, double cx, double dx, double ay, double by, double cy, double dy);
Point3D *get3DParametricValues(int n, double **AMatrix);

double **getAMatrixHermite(Point3D p1, Point3D p2, Point3D v1, Point3D v2);
double **getAMatrixBezier(Point3D p1, Point3D p2, Point3D b1, Point3D b2);

int factorial(int n);
int combination(int n, int k);
int *findCombinationValuesForBernstein(int n);
double bernsteinPolynomial(double u, int n, int k, int *combinationValues);
Point3D calculateBezierSurfacePoint(double u, double v, Point3D **controlPoints, int m, int n);

double ***getAMatrixBezierSurface(Point3D **controlPoints);
Point3D getBezierSurfacePoint(double u, double v, double ***AMatrices);

void setPixelCurve(Color **image, int width, int height, Point2DInt *points, int numPoints, Color *curveColor);

#endif