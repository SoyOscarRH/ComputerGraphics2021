#include <stdio.h>
#include <stdlib.h>

double **matrixMultiplication(double **A, int rA, int cA, double **B, int rB, int cB){

    if(cA != rB){
        return NULL;
    }

    double **C = (double**)malloc(rA*sizeof(double*));
    for(int i = 0; i < rA; i++){
        C[i] = (double*)malloc(cB*sizeof(double));
    }

    //printf("Haciendo mult...\n");
    int counter = 0;

    for(int i = 0; i < rA; i++){
        for(int j = 0; j < cB; j++){
            double sum = 0;
            for(int k = 0; k < rB; k++){
                sum += A[i][k] * B[k][j];
                counter++;
                //printf("Counter: %d : %f\n", counter, sum);
            }
            C[i][j] = sum;
        }
    }

    return C;
}

void printMatrix(double **M, int m, int n){
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            printf("%f ", M[i][j]);
        }
        printf("\n");
    }
}