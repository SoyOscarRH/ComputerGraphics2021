#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"
#include "point.h"


double evaluatePolynomialDegree3(double t, double a, double b, double c, double d){
    double finalValue = a*t*t*t + b*t*t + c*t + d;
    return finalValue;
}

Point2D *get2DParametricValues(int n, double ax, double bx, double cx, double dx, double ay, double by, double cy, double dy){
    Point2D *linesValues = (Point2D*)malloc((n+1)*sizeof(Point2D));

    double increment = 1.0 / n;

    double currentT = 0.0;
    for(int i = 0; i < n+1; i++){
        linesValues[i].x =  evaluatePolynomialDegree3(currentT, ax, bx, cx, dy);
        linesValues[i].y =  evaluatePolynomialDegree3(currentT, ay, by, cy, dy);
        currentT += increment;
    }

    return linesValues;
}

Point3D *get3DParametricValues(int n, double **AMatrix){

    Point3D *linesValues = (Point3D*)malloc((n+1)*sizeof(Point3D));

    double increment = 1.0 / n;

    double currentT = 0.0;
    for(int i = 0; i < n+1; i++){
        Point3DHomogeneous TVector = {.x = currentT*currentT*currentT, .y = currentT*currentT, .z = currentT, .w = 1};
        linesValues[i].x = TVector.x*AMatrix[0][0] + TVector.y*AMatrix[1][0] + TVector.z*AMatrix[2][0] + TVector.w*AMatrix[3][0];
        linesValues[i].y = TVector.x*AMatrix[0][1] + TVector.y*AMatrix[1][1] + TVector.z*AMatrix[2][1] + TVector.w*AMatrix[3][1];
        linesValues[i].z = TVector.x*AMatrix[0][2] + TVector.y*AMatrix[1][2] + TVector.z*AMatrix[2][2] + TVector.w*AMatrix[3][2];

        currentT += increment;
    }

    return linesValues;
}

double **getAMatrixHermite(Point3D p1, Point3D p2, Point3D v1, Point3D v2){
    
    double **A = (double**)malloc(4*sizeof(double*));
    for(int i = 0; i < 4; i++){
        A[i] = (double*)malloc(3*sizeof(double));
    }

    double H[4][4] = {
        { 2.0, -2.0,  1.0,  1.0},
        {-3.0,  3.0, -2.0, -1.0},
        { 0.0,  0.0,  1.0,  0.0},
        { 1.0,  0.0,  0.0,  0.0}
    };

    double G[4][3] = {
        {p1.x, p1.y, p1.z},
        {p2.x, p2.y, p2.z},
        {v1.x, v1.y, v1.z},
        {v2.x, v2.y, v2.z}
    };

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 3; j++){
            A[i][j] = 0;
            for(int k = 0; k < 4; k++){
                A[i][j] += H[i][k] * G[k][j];
            }
        }
    }

    return A;
}

double **getAMatrixBezier(Point3D p1, Point3D p2, Point3D b1, Point3D b2){
    double **A = (double**)malloc(4*sizeof(double*));
    for(int i = 0; i < 4; i++){
        A[i] = (double*)malloc(3*sizeof(double));
    }
    
    double B[4][4] = {
        { -1.0,  3.0, -3.0, 1.0},
        {  3.0, -6.0,  3.0, 0.0},
        { -3.0,  3.0,  0.0, 0.0},
        {  1.0,  0.0,  0.0, 0.0}
    };

    double G[4][3] = {
        {p1.x, p1.y, p1.z},
        {b1.x, b1.y, b1.z},
        {b2.x, b2.y, b2.z},        
        {p2.x, p2.y, p2.z}
    };

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 3; j++){
            A[i][j] = 0;
            for(int k = 0; k < 4; k++){
                A[i][j] += B[i][k] * G[k][j];
            }
        }
    }

    return A;    
}

int factorial(int n){
    if(n == 0){
        return 1;
    }else{
        return(n*factorial(n-1));
    }
}

int combination(int n, int k){
    int value = factorial(n)/(factorial(k)*factorial(n-k));
    return value;
}

int *findCombinationValuesForBernstein(int n){
    int *combinatories = (int*)malloc(n*sizeof(int));

    for(int i = 0; i < n; i++){
        combinatories[i] = combination(n-1, i);
    }
    
    return combinatories;
}

double bernsteinPolynomial(double u, int n, int k, int *combinationValues){
    double bernsteinValue = combinationValues[k] * pow(u, k) * pow((1-u), (n-k));
    return bernsteinValue;
}

double ***getAMatrixBezierSurface(Point3D **controlPoints){

    double ***Amatrices = (double***)malloc(3*sizeof(double**));

    double **Gx = (double**)malloc(4*sizeof(double*));
    for(int i = 0; i < 4; i++){
        Gx[i] = (double*)malloc(4*sizeof(double));
    }

    double **Gy = (double**)malloc(4*sizeof(double*));
    for(int i = 0; i < 4; i++){
        Gy[i] = (double*)malloc(4*sizeof(double));
    }
    
    double **Gz = (double**)malloc(4*sizeof(double*));
    for(int i = 0; i < 4; i++){
        Gz[i] = (double*)malloc(4*sizeof(double));
    }

    double **bezierMatrix = (double**)malloc(4*sizeof(double*));
    for(int i = 0; i < 4; i++){
        bezierMatrix[i] = (double*)malloc(4*sizeof(double));
    }

    bezierMatrix[0][0] = -1.0;
    bezierMatrix[0][1] = 3.0;
    bezierMatrix[0][2] = -3.0;
    bezierMatrix[0][3] = 1.0;
    
    bezierMatrix[1][0] = 3.0;
    bezierMatrix[1][1] = -6.0;
    bezierMatrix[1][2] = 3.0;
    bezierMatrix[1][3] = 0.0;
    
    bezierMatrix[2][0] = -3.0;
    bezierMatrix[2][1] = 3.0;
    bezierMatrix[2][2] = 0.0;
    bezierMatrix[2][3] = 0.0;
    
    bezierMatrix[3][0] = 1.0;
    bezierMatrix[3][1] = 0.0;
    bezierMatrix[3][2] = 0.0;
    bezierMatrix[3][3] = 0.0;

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            Gx[i][j] = controlPoints[i][j].x;
            Gy[i][j] = controlPoints[i][j].y;
            Gz[i][j] = controlPoints[i][j].z;
        }   
    }

    double **auxAx = matrixMultiplication(bezierMatrix, 4, 4, Gx, 4, 4);
    double **auxAy = matrixMultiplication(bezierMatrix, 4, 4, Gy, 4, 4);
    double **auxAz = matrixMultiplication(bezierMatrix, 4, 4, Gz, 4, 4);

    Amatrices[0] = matrixMultiplication(auxAx, 4, 4, bezierMatrix, 4, 4);
    Amatrices[1] = matrixMultiplication(auxAy, 4, 4, bezierMatrix, 4, 4);
    Amatrices[2] = matrixMultiplication(auxAz, 4, 4, bezierMatrix, 4, 4);

    for(int i = 0; i < 4; i++){
        free(bezierMatrix[i]);
        free(Gx[i]);
        free(Gy[i]);
        free(Gz[i]);
        free(auxAx[i]);
        free(auxAy[i]);
        free(auxAz[i]);
    }
    free(bezierMatrix);
    free(Gx);
    free(Gy);
    free(Gz);
    free(auxAx);
    free(auxAy);
    free(auxAz);

    return Amatrices;
}

Point3D getBezierSurfacePoint(double u, double v, double ***AMatrices){
    
    double **uV = (double**)malloc(sizeof(double*));
    for(int i = 0; i < 1; i++){
        uV[i] = (double*)malloc(4*sizeof(double));
    }

    uV[0][0] = u*u*u;
    uV[0][1] = u*u;
    uV[0][2] = u;
    uV[0][3] = 1;

    double **vV = (double**)malloc(4*sizeof(double*));
    for(int i = 0; i < 4; i++){
        vV[i] = (double*)malloc(sizeof(double));
    }

    vV[0][0] = v*v*v;
    vV[1][0] = v*v;
    vV[2][0] = v;
    vV[3][0] = 1;


    double **x_aux = matrixMultiplication(uV, 1, 4, AMatrices[0], 4, 4);
    double **x = matrixMultiplication(x_aux, 1, 4, vV, 4, 1);

    double **y_aux = matrixMultiplication(uV, 1, 4, AMatrices[1], 4, 4);
    double **y = matrixMultiplication(y_aux, 1, 4, vV, 4, 1);

    double **z_aux = matrixMultiplication(uV, 1, 4, AMatrices[2], 4, 4);
    double **z = matrixMultiplication(z_aux, 1, 4, vV, 4, 1);

    Point3D point = {.x = x[0][0], .y = y[0][0], .z = z[0][0]};

    //Free memory.
    for(int i = 0; i < 1; i++){
        free(uV[i]);
        free(x_aux[i]);
        free(y_aux[i]);
        free(z_aux[i]);
    }
    free(uV);
    free(x_aux);
    free(y_aux);
    free(z_aux);

    free(x[0]);
    free(x);
    free(y[0]);
    free(y);
    free(z[0]);
    free(z);    

    for(int i = 0; i < 4; i++){
        free(vV[i]);
    }
    free(vV);

    return point;
}

Point3D calculateBezierSurfacePoint(double u, double v, Point3D **controlPoints, int m, int n){
    
    int *mCombinations = findCombinationValuesForBernstein(m);
    int *nCombinations = findCombinationValuesForBernstein(n);

    double **bersteinU = (double**)malloc(1*sizeof(double*));
    for(int i = 0; i < 1; i++){
        bersteinU[i] = (double*)malloc(m*sizeof(double));
    }

    double **bersteinV = (double**)malloc(n*sizeof(double*));
    for(int i = 0; i < n; i++){
        bersteinV[i] = (double*)malloc(1*sizeof(double));
    }  

    for(int i = 0; i < m; i++){
        bersteinU[0][i] = bernsteinPolynomial(u, m-1, i, mCombinations);
    }
    
    for(int i = 0; i < n; i++){
        bersteinV[i][0] = bernsteinPolynomial(v, n-1, i, nCombinations);
    }      

    double **xValues = (double**)malloc(m*sizeof(double*));
    double **yValues = (double**)malloc(m*sizeof(double*));
    double **zValues = (double**)malloc(m*sizeof(double*));
    for(int i = 0; i < m; i++){
        xValues[i] = (double*)malloc(n*sizeof(double));
        yValues[i] = (double*)malloc(n*sizeof(double));
        zValues[i] = (double*)malloc(n*sizeof(double));
    }

    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            xValues[i][j] = controlPoints[i][j].x;
            yValues[i][j] = controlPoints[i][j].y;
            zValues[i][j] = controlPoints[i][j].z;
        }
    }

    double **xAux = matrixMultiplication(bersteinU, 1, m, xValues, m, n);
    double **yAux = matrixMultiplication(bersteinU, 1, m, yValues, m, n);
    double **zAux = matrixMultiplication(bersteinU, 1, m, zValues, m, n);

    double **finalX = matrixMultiplication(xAux, 1, n, bersteinV, n, 1);
    double **finalY = matrixMultiplication(yAux, 1, n, bersteinV, n, 1);
    double **finalZ = matrixMultiplication(zAux, 1, n, bersteinV, n, 1);

    Point3D finalP = {.x = finalX[0][0], .y = finalY[0][0], .z = finalZ[0][0]};

    free(mCombinations);
    free(nCombinations);
    for(int i = 0; i < 1; i++){
        free(bersteinU[i]);
    }
    free(bersteinU);

    for(int i = 0; i < n; i++){
        free(bersteinV[i]);
    }
    free(bersteinV);

    for(int i = 0; i < m; i++){
        free(xValues[i]);
        free(yValues[i]);
        free(zValues[i]);
    }
    free(xValues);
    free(yValues);
    free(zValues);

    for(int i = 0; i < 1; i++){
        free(xAux[i]);
        free(yAux[i]);
        free(zAux[i]);
    }
    free(xAux);
    free(yAux);
    free(zAux);

    free(finalX[0]);
    free(finalX);
    free(finalY[0]);
    free(finalY);
    free(finalZ[0]);
    free(finalZ);        

    return finalP;
}

void setPixelCurve(Color **image, int width, int height, Point2DInt *points, int numPoints, Color *curveColor){

    for(int i = 0; i < numPoints; i++){
        if( (points[i].x >= 0 && points[i].x < width) && (points[i].y >= 0 && points[i].y < height) ){
            image[points[i].y][points[i].x] = (Color){.R = (*curveColor).R, .G = (*curveColor).G, .B = (*curveColor).B};
        }
    }

}
