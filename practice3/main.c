#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include "matrix.h"
#include "render.h"
#include "face.h"
#include "pixel.h"
#include "fileManagement.h"
#include "primitives.h"

// ffmpeg [global options] [input options] -i input [output options] output
// ffmpeg -i "%d.ppm" -r 24 -q:v 1 <nombre>.avi
// ffmpeg -i "%d.ppm" -ss <startTime> -t <duration> -i <audio>.mp3 -r <fps> -q:v <qualityValue> <output>.avi
// ffmpeg -i "%d.ppm" -ss 281 -t 11 -i audio.mp3 -r 24 -q:v 1 -filter:a "volume=0.25" animationFullHDAudio.avi

int main(int argc, const char *argv[]){

    clock_t begin, end;
    float time_spent;

    int width = 1920;
    int height = 1080;
    
    PixelRGB red;
    red.R = 255;
    red.G = 0;
    red.B = 0;
    
    if(argc == 2){
        int algorithm;
        printf("Line drawing algorithm: ");
        scanf("%d", &algorithm);

        begin = clock();
        Object3D *obj = createObject3D(argv[1]);
        int tipo;

        //printf("Rotaciones: ");
        //scanf("%f %f %f", &rotX, &rotY, &rotZ);

        //printf("Tipo: ");
        //scanf("%d", &tipo);
        tipo = 0;


        Point3DHomogeneous *minPoint = (Point3DHomogeneous*)calloc(1, sizeof(Point3DHomogeneous));
        Point3DHomogeneous *maxPoint = (Point3DHomogeneous*)calloc(1, sizeof(Point3DHomogeneous));
        Point3DHomogeneous *midPoint = (Point3DHomogeneous*)calloc(1, sizeof(Point3DHomogeneous));

        minPoint->x = obj->minPoint.x;
        minPoint->y = obj->minPoint.y;
        minPoint->z = obj->minPoint.z;
        minPoint->w = 1;

        maxPoint->x = obj->maxPoint.x;
        maxPoint->y = obj->maxPoint.y;
        maxPoint->z = obj->maxPoint.z;
        maxPoint->w = 1;

        midPoint->x = obj->midPoint.x;
        midPoint->y = obj->midPoint.y;
        midPoint->z = obj->midPoint.z;
        midPoint->w = 1; 

        float **translation1 = translationMatrix(-1.0, 0.0, 0.0);
        float **rotationX1 = rotationXMatrix(7.5);
        float **transformationMatrix1 = matrix4x4Multiplication(rotationX1, translation1);

        float **rotationY2 = rotationYMatrix(-7.5);

        float **translation3 = translationMatrix(1.0, 0.0, 0.0);
        float **rotationX3 = rotationXMatrix(-7.5);
        float **transformationMatrix3 = matrix4x4Multiplication(rotationX3, translation3);        

        float **rotationY4 = rotationYMatrix(3.75);

        float **rotationZ5 = rotationZMatrix(-5);
        
        float **rotationZ6 = rotationZMatrix(7);
        
        int fps = 24;

        int frameAE = 2*fps;
        int frameBE = frameAE + fps;
        int frameCE = frameBE + 2*fps;
        int frameDE = frameCE + fps;
        int frameEE = frameDE + 2*fps;
        int frameFE = frameEE + 3*fps + 1;
        

        renderFrames(obj, transformationMatrix1, 0,        frameAE, tipo, width, height, algorithm, red);
        renderFrames(obj, rotationY2,            frameAE,  frameBE, tipo, width, height, algorithm, red);
        renderFrames(obj, transformationMatrix3, frameBE,  frameCE, 0, width, height, algorithm, red);
        renderFrames(obj, rotationY4, frameCE,  frameDE, tipo, width, height, algorithm, red);
        renderFrames(obj, rotationZ5, frameDE,  frameEE, tipo, width, height, algorithm, red);
        renderFrames(obj, rotationZ6, frameEE,  frameFE, tipo, width, height, algorithm, red);

        end = clock();
        time_spent = (double)(end - begin) / CLOCKS_PER_SEC; 
        printf("%f\n\n", time_spent);
    }else{
        printf("./main <objectFilename>.obj\n");
    }            

    return 0;
}