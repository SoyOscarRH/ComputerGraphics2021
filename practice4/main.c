#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "color.h"
#include "fileManagement.h"
#include "light.h"
#include "matrix.h"
#include "object3D.h"
#include "point.h"
#include "primitives.h"


int main(int argc, const char *argv[]){

    int width = 500;
    int height = 500;

    if(argc == 2){

        //Loading .obj and giving it materials properties
        Object3D obj = readObjFile(argv[1]);
        obj.edgesColor = (Color){.R = 255, .G = 255, .B = 255};
        for(int i = 0; i < obj.numFaces; i++){
            obj.diffuseF[i] = 1;
            obj.specularF[i] = 5;
        }

        //Defining lights of the scene.
        LightNode *lightList = NULL;

        Light *spotlight2 = (Light*)malloc(sizeof(Light));
        spotlight2->color = (Color){.R = 0, .G = 100, .B = 0};
        spotlight2->type = 2;

        SpotlightInfo *spotlightInfo2 = (SpotlightInfo*)malloc(sizeof(SpotlightInfo));
        spotlightInfo2->a = 0.01;
        spotlightInfo2->b = 0.01;
        spotlightInfo2->c = 2;
        spotlightInfo2->position = (Point3D){.x = -10, .y = 0, .z = 10};
        spotlightInfo2->direction = (Point3D){.x = 0, .y = 0, .z = 10};
        spotlightInfo2->angle = 10;

        spotlight2->info = (void*)spotlightInfo2;
        addLight(&lightList, spotlight2);

        Light *light2 = (Light*)calloc(1, sizeof(Light));
        light2->color = (Color){.R = 75, .G = 0, .B = 0};
        light2->type = 1;

        SunlightInfo *sunlight2Info = (SunlightInfo*)calloc(1, sizeof(SunlightInfo));
        sunlight2Info->a = 0.1;
        sunlight2Info->b = 0.01;
        sunlight2Info->c = 1;
        sunlight2Info->position = (Point3D){.x = 100, .y = 100, .z = -100};

        light2->info = (void*)sunlight2Info;
        addLight(&lightList, light2);

        //Ambient light.
        Light *ambient1 = (Light*)malloc(sizeof(Light));
        ambient1->color = (Color){.R = 0, .G = 0, .B = 180};
        ambient1->type = 0;
        ambient1->info = NULL;
        addLight(&lightList, ambient1);

        double **perspectiveMatrix = perspectiveMatrixZAxis(2.0);
        Point3D cameraVector = {.x = 0, .y = 0, .z = 1.0};

        double **initialTranslation = translationMatrix(0, 0, 10);
        double **initialRotationZ = rotationZMatrix(90);
        double **initialTransformation = matrix4x4Multiplication(initialRotationZ, initialTranslation);

        drawFrame("ppms/1.ppm", &obj, lightList, initialTransformation, perspectiveMatrix, cameraVector, height, width);


        for(int i = 2; i < 241; i++){

            Point3D minPoint = (Point3D){.x = obj.vertices[0].x, .y = obj.vertices[0].y, .z = obj.vertices[0].z};
            Point3D maxPoint = (Point3D){.x = obj.vertices[0].x, .y = obj.vertices[0].y, .z = obj.vertices[0].z};
            for(int i = 1; i < obj.numVertices; i++){
                minPoint.x = (obj.vertices[i].x < minPoint.x) ? obj.vertices[i].x : minPoint.x;
                minPoint.y = (obj.vertices[i].y < minPoint.y) ? obj.vertices[i].y : minPoint.y;
                minPoint.z = (obj.vertices[i].z < minPoint.z) ? obj.vertices[i].z : minPoint.z;

                maxPoint.x = (obj.vertices[i].x < maxPoint.x) ? obj.vertices[i].x : maxPoint.x;
                maxPoint.y = (obj.vertices[i].y < maxPoint.y) ? obj.vertices[i].y : maxPoint.y;
                maxPoint.z = (obj.vertices[i].z < maxPoint.z) ? obj.vertices[i].z : maxPoint.z;
            }
            Point3D midPoint = (Point3D){.x = (minPoint.x+maxPoint.x)/2, .y = (minPoint.y+maxPoint.y)/2, .z = (minPoint.z+maxPoint.z)/2};
            double **translateToOrigin = translationMatrix(-midPoint.x, -midPoint.y, -midPoint.z);
            double **secondTransformation = rotationYMatrix(1.5);
            double **translateOriginal = translationMatrix(midPoint.x, midPoint.y, midPoint.z);
            double **A = matrix4x4Multiplication(secondTransformation, translateToOrigin);
            double **finalMatrix = matrix4x4Multiplication(translateOriginal, A);

            char frameName[20];
            char frameNumber[10];
            memset(frameName, 0, 20);
            memset(frameNumber, 0, 10);
            strcat(frameName, "ppms/");
            sprintf(frameNumber, "%d", i);
            strcat(frameName, frameNumber);
            strcat(frameName, ".ppm");
            drawFrame(frameName, &obj, lightList, finalMatrix, perspectiveMatrix, cameraVector, height, width);

            free(translateToOrigin);
            free(secondTransformation);
            free(translateOriginal);
            free(A);
            free(finalMatrix);
        }

    }

}
