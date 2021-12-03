#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <time.h>

#include "matrix.h"
#include "point.h"
#include "face.h"
#include "pixel.h"
#include "primitives.h"
#include "fileManagement.h"
#include "threadPool.h"
#include "render.h"

void readObjFile(Point3DNode **vertices, FaceNode **faces, const char *nameFile, int *numVertices, float *xMin, float *xMax, float *yMin, float *yMax, float *zMin, float *zMax){
    
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    Point3DNode *lastVertex;
    Point3DNode *firstVertex;
    int isFirstVertexFound = 0;

    FaceNode *lastFace;
    FaceNode *firstFace;
    int isFirstFaceFound = 0;

    char *token;

    fp = fopen(nameFile, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        token = strtok(line, " ");
        if(strcmp(token, "v") == 0){
            *numVertices += 1;
            token = strtok(NULL, " ");
            if(isFirstVertexFound == 0){
                firstVertex = (Point3DNode*)malloc(sizeof(Point3DNode));
                firstVertex->point.x = atof(token);
                *xMin = firstVertex->point.x;
                *xMax = firstVertex->point.x;
                token = strtok(NULL, " ");
                firstVertex->point.y = atof(token);
                *yMin = firstVertex->point.y;
                *yMax = firstVertex->point.y;                
                token = strtok(NULL, " ");
                firstVertex->point.z = atof(token);
                *zMin = firstVertex->point.z;
                *zMax = firstVertex->point.z;

                lastVertex = firstVertex;
                isFirstVertexFound = 1;  
            }else{
                lastVertex->next = (Point3DNode*)malloc(sizeof(Point3DNode));
                lastVertex->next->point.x = atof(token);
                *xMin = (lastVertex->next->point.x < *xMin) ? lastVertex->next->point.x : *xMin;
                *xMax = (lastVertex->next->point.x > *xMax) ? lastVertex->next->point.x : *xMax;
                token = strtok(NULL, " ");
                lastVertex->next->point.y = atof(token);
                *yMin = (lastVertex->next->point.y < *yMin) ? lastVertex->next->point.y : *yMin;
                *yMax = (lastVertex->next->point.y > *yMax) ? lastVertex->next->point.y : *yMax;                
                token = strtok(NULL, " ");
                lastVertex->next->point.z = atof(token); 
                *zMin = (lastVertex->next->point.z < *zMin) ? lastVertex->next->point.z : *zMin;
                *zMax = (lastVertex->next->point.z > *zMax) ? lastVertex->next->point.z : *zMax;                
                lastVertex = lastVertex->next;               
            }           
        }else if(strcmp(token, "f") == 0){
            token = strtok(NULL, " ");
            if(isFirstFaceFound == 0){
                firstFace = (FaceNode*)malloc(sizeof(FaceNode));
                int isFirstVertex = 0;
                intNode *aux = (intNode*)malloc(sizeof(intNode));

                while(token != NULL){
                    if(isFirstVertex == 0){
                        aux->value = atoi(token);
                        firstFace->face = aux;
                        isFirstVertex = 1;
                    }else{
                        aux->next = (intNode*)malloc(sizeof(intNode));
                        aux->next->value = atoi(token);
                        aux = aux->next;
                    }
                    token = strtok(NULL, " ");
                } 
                lastFace = firstFace;
                isFirstFaceFound = 1;
            }else{
                lastFace->next = (FaceNode*)malloc(sizeof(FaceNode));
                int isFirstVertex = 0;
                intNode *aux = (intNode*)malloc(sizeof(intNode));

                while(token != NULL){
                    if(isFirstVertex == 0){
                        aux->value = atoi(token);
                        lastFace->next->face = aux;
                        isFirstVertex = 1;
                    }else{
                        aux->next = (intNode*)malloc(sizeof(intNode));
                        aux->next->value = atoi(token);
                        aux = aux->next;
                    }
                    token = strtok(NULL, " ");
                }
                lastFace = lastFace->next;
            }

        }
    }

    lastVertex->next = NULL;
    *vertices = firstVertex;

    lastFace->next = NULL;
    *faces = firstFace;

    fclose(fp);
    if (line)
        free(line);
}

Object3D *createObject3D(const char *nameFile){

    Object3D *object3D = (Object3D*)calloc(1, sizeof(Object3D));
    float xMin, xMax, yMin, yMax, zMin, zMax;

    readObjFile(&object3D->vertices, &object3D->faces, nameFile, &object3D->numVertices,  &xMin, &xMax, &yMin, &yMax, &zMin, &zMax);
    
    object3D->minPoint.x = xMin;
    object3D->minPoint.y = yMin;
    object3D->minPoint.z = zMin;

    object3D->maxPoint.x = xMax;
    object3D->maxPoint.y = yMax;
    object3D->maxPoint.z = zMax;

    object3D->midPoint.x = (xMax - xMin)/2;
    object3D->midPoint.y = (yMax - yMin)/2;
    object3D->midPoint.z = (zMax - zMin)/2;
    
    return object3D;

}

Point2D *reduce3Dto2D(Point3DNode *vertices, int numVertices){

    Point2D *vertices2D = (Point2D*)calloc(numVertices, sizeof(Point2D));

    Point3DNode *aux = vertices;

    for(int i = 0; i < numVertices; i++){
        vertices2D[i].x = aux->point.x;
        vertices2D[i].y = aux->point.y;
        aux = aux->next;
    }

    return vertices2D;

}

void scale2DPoints(Point2D *vertices, int numVertices, float scale){

    for(int i = 0; i < numVertices; i++){
        vertices[i].x *= scale;
        vertices[i].y *= scale;
    }

}

void center2DPoints(Point2D *vertices, int numVertices, float xCenter, float yCenter, float xMin, float xMax, float yMin, float yMax){
    
    float xMidpoint, yMidpoint;
    float differenceXMid, differenceYMid;

    xMidpoint = (xMin+xMax)/2;
    yMidpoint = (yMin+yMax)/2;

    differenceXMid = xCenter - xMidpoint;
    differenceYMid = yCenter - yMidpoint;

    for(int i = 0; i < numVertices; i++){
        vertices[i].x += differenceXMid;
        vertices[i].y += differenceYMid;
    }    

}

void setLine(Point2DInt *points, int numPoints, PixelRGB **image, PixelRGB color){
    //printf("Pintando con color: %d %d %d\n", color.R, color.G, color.B);
    for(int i = 0; i < numPoints; i++){
        //printf("Dibujando puntito... \n");
        image[points[i].y][points[i].x].R = color.R;
        image[points[i].y][points[i].x].G = color.G;
        image[points[i].y][points[i].x].B = color.B;
    }
}

void renderFrames(Object3D *obj, float **transformationMatrix, int firstFrame, int lastFrame, int typeProjection, int width, int height, int lineAlgorithm, PixelRGB color){
    
    //printf("Renderizando... \n");

    for(int i = firstFrame; i < lastFrame; i++){
        Point3DHomogeneous *homogeneousVertices = convertToHomogeneous3D(obj->vertices, obj->numVertices); 

        for(int i = 0; i < obj->numVertices; i++){
            matrixPoint3DHMultiplicationInPlace(transformationMatrix, &homogeneousVertices[i]);
        }
        /*matrixPoint3DHMultiplicationInPlace(transformationMatrix, minPoint);
        matrixPoint3DHMultiplicationInPlace(transformationMatrix, maxPoint);
        matrixPoint3DHMultiplicationInPlace(transformationMatrix, midPoint);
        */

        //printf("Transformaciones aplicadas... \n");    

        returnFromHomogeneous3D(obj->vertices, homogeneousVertices, obj->numVertices);
        
        char frameName[20];
        char frameNumber[10];
        memset(frameName, 0, 20);
        memset(frameNumber, 0, 10);
        strcat(frameName, "frames/");
        sprintf(frameNumber, "%d", i);
        strcat(frameName, frameNumber);
        strcat(frameName, ".ppm");

        if(typeProjection){
            //printf("Parallel\n");
            objToRasterXY(obj, frameName, width, height, lineAlgorithm, color);
        }else{
            //printf("Perspective\n");
            objToRasterXYPerspective(obj, frameName, width, height, lineAlgorithm, color);
        } 

        free(homogeneousVertices);

    }    
}

void objToRasterXY(Object3D *obj, const char *ppmFile, int width, int height, int lineDrawingAlgorithm, PixelRGB lineColor){

    float xMin = obj->xMin;
    float xMax = obj->xMax;
    float yMin = obj->yMin;
    float yMax = obj->yMax;

    float xScale = (width*0.9)/(fabs(xMin)+fabs(xMax));
    float yScale = (height*0.9)/(fabs(yMin)+fabs(yMax));

    Point2D *vertices2D = reduce3Dto2D(obj->vertices, obj->numVertices);

    float scale = (xScale < yScale) ? xScale : yScale;
    scale2DPoints(vertices2D, obj->numVertices, scale);

    center2DPoints(vertices2D, obj->numVertices, width/2.0, height/2.0, xMin*scale, xMax*scale, yMin*scale, yMax*scale);

    Point2DInt *verticesInt = (Point2DInt*)calloc(obj->numVertices, sizeof(Point2DInt));
    transformArrayPoint2DFloatToInt(vertices2D, verticesInt, obj->numVertices);

    PixelRGB **image = (PixelRGB**)calloc(height, sizeof(PixelRGB*));
    for(int i = 0; i < height; i++){
        image[i] = (PixelRGB*)calloc(width, sizeof(PixelRGB));
    }
    
    printf("Datos leidos...\n");

    FaceNode *siguiente = obj->faces;
    while(siguiente){
        //printf("Dibujando cara... \n");
        drawFace(image, lineColor, *siguiente, verticesInt, lineDrawingAlgorithm); 
        siguiente = siguiente->next;     
    }

    writePPMP3(ppmFile, image, height, width);

    for(int i = 0; i < height; i++){
        free(image[i]);
    }
    free(image);

}


void objToRasterXYParallel(const char *objFile, const char *ppmFile, int width, int height, int lineDrawingAlgorithm, PixelRGB lineColor, int numThreads){
    Point3DNode *vertices;
    int numVertices = 0;
    
    FaceNode *faces;
    float xMin, xMax, yMin, yMax, zMin, zMax;

    readObjFile(&vertices, &faces, objFile, &numVertices,  &xMin, &xMax, &yMin, &yMax, &zMin, &zMax);

    float xScale = (width*0.9)/(fabs(xMin)+fabs(xMax));
    float yScale = (height*0.9)/(fabs(yMin)+fabs(yMax));

    Point2D *vertices2D = reduce3Dto2D(vertices, numVertices);

    float scale = (xScale < yScale) ? xScale : yScale;
    scale2DPoints(vertices2D, numVertices, scale);

    center2DPoints(vertices2D, numVertices, width/2.0, height/2.0, xMin*scale, xMax*scale, yMin*scale, yMax*scale);

    Point2DInt *verticesInt = (Point2DInt*)calloc(numVertices, sizeof(Point2DInt));
    transformArrayPoint2DFloatToInt(vertices2D, verticesInt, numVertices);

    PixelRGB **image = (PixelRGB**)calloc(height, sizeof(PixelRGB*));
    for(int i = 0; i < height; i++){
        image[i] = (PixelRGB*)calloc(width, sizeof(PixelRGB));
    }

    /*for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            image[i][j].R = 255;
            image[i][j].G = 255;
            image[i][j].B = 255;
        }
    }*/   
    
    printf("Lectura de datos completada...\n");

    //Thread configuration...
    ThreadPool *pool = (ThreadPool*)malloc(sizeof(ThreadPool));
    pthread_t *th = (pthread_t*)calloc(numThreads, sizeof(pthread_t));
    pthread_t masterThreadId;

    pthread_mutex_init(&pool->mutexQueue, NULL);
    pthread_mutex_init(&pool->mutexImage, NULL);
    pthread_mutex_init(&pool->mutexTWorking, NULL);
    pthread_mutex_init(&pool->mutexDone, NULL);
    pthread_mutex_init(&pool->mutexTotal, NULL);
    pthread_cond_init(&pool->condQueue, NULL);
    
    pool->totalTasks = 0;
    pool->doneTasks = 0;

    pool->threadsWorking=0;
    pool->stop = 0;
    pool->first = NULL;
    pool->last = NULL;
    
    for (int i = 0; i < numThreads; i++) {
        if (pthread_create(&th[i], NULL, &runThread, (void*)pool) != 0) {
            perror("Failed to create the thread");
        }
    } 

    printf("Hilos creados...\n");

    FaceNode *siguiente = faces;
    while(siguiente){
        drawFaceParallel(image, &lineColor, *siguiente, verticesInt, lineDrawingAlgorithm, pool, numThreads); 
        siguiente = siguiente->next;     
    }

    pthread_create(&masterThreadId, NULL, &masterThread, (void*)pool);

    if(pthread_join(masterThreadId, NULL) != 0){
        perror("Failed to join master thread\n");
    }

    for (int i = 0; i < numThreads; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join the thread");
        }
    }

    printf("Hilos terminados...\n");

    pthread_mutex_destroy(&pool->mutexQueue);
    pthread_mutex_destroy(&pool->mutexImage);
    pthread_mutex_destroy(&pool->mutexTWorking);
    pthread_cond_destroy(&pool->condQueue);    

    printf("Guardando imagen...\n");
    writePPMP3(ppmFile, image, height, width);

    for(int i = 0; i < height; i++){
        free(image[i]);
    }
    free(image);    
}

void objToRasterXYPerspective(Object3D *obj, const char *ppmFile, int width, int height, int lineDrawingAlgorithm, PixelRGB lineColor){


    float **perspectiveMatrix = perspectiveMatrixZAxis(1);
    float **translationZ = translationMatrix(0, 0, 50);
    float **finalMatrix = matrix4x4Multiplication(perspectiveMatrix, translationZ);

    float xMin, xMax, yMin, yMax;
    float xAux, yAux;

    Point3DHomogeneous *homogeneousVertices = convertToHomogeneous3D(obj->vertices, obj->numVertices);

    matrixPoint3DHMultiplicationInPlace(finalMatrix, &homogeneousVertices[0]);
    xMin = homogeneousVertices[0].x / homogeneousVertices[0].w;
    xMax = xMin;
    yMin = homogeneousVertices[0].y / homogeneousVertices[0].w;
    yMax = yMin;    

    for(int i = 1; i < obj->numVertices; i++){
        matrixPoint3DHMultiplicationInPlace(finalMatrix, &homogeneousVertices[i]);
        xAux = homogeneousVertices[i].x / homogeneousVertices[i].w;
        yAux = homogeneousVertices[i].y / homogeneousVertices[i].w;
        if(xAux < xMin){
            xMin = xAux;
        }
        if(xAux > xMax){
            xMax = xAux;
        }
        if(yAux < yMin){
            yMin = yAux;
        }
        if(yAux > yMax){
            yMax = yAux;
        }        
    }

    Point3DNode *projectedVertices = returnFromHomogeneous3DNew(homogeneousVertices, obj->numVertices);    

    float xScale = (width*0.9)/(xMax-xMin);
    float yScale = (height*0.9)/(yMax-yMin);

    Point2D *vertices2D = reduce3Dto2D(projectedVertices, obj->numVertices);

    float scale = (xScale < yScale) ? xScale : yScale;

    /*
    printf("Scale x: %f    Scale y: %f\n", xScale, yScale);
    printf("x: [%f, %f], y: [%f, %f]\n", xMin, xMax, yMin, yMax);
    printf("scale: %f, x: [%f, %f], y: [%f, %f]\n", scale, xMin*scale, xMax*scale, yMin*scale, yMax*scale);
    */
    /*if(scale > 1){
        scale = 1;
    }*/

    scale2DPoints(vertices2D, obj->numVertices, scale);

    center2DPoints(vertices2D, obj->numVertices, width/2.0, height/2.0, xMin*scale, xMax*scale, yMin*scale, yMax*scale);

    Point2DInt *verticesInt = (Point2DInt*)calloc(obj->numVertices, sizeof(Point2DInt));
    transformArrayPoint2DFloatToInt(vertices2D, verticesInt, obj->numVertices);

    PixelRGB **image = (PixelRGB**)calloc(height, sizeof(PixelRGB*));
    for(int i = 0; i < height; i++){
        image[i] = (PixelRGB*)calloc(width, sizeof(PixelRGB));
    }
    

    FaceNode *siguiente = obj->faces;
    while(siguiente){
        //printf("Dibujando cara... \n");
        drawFace(image, lineColor, *siguiente, verticesInt, lineDrawingAlgorithm); 
        siguiente = siguiente->next;     
    }

    writePPMP3(ppmFile, image, height, width);

    free(homogeneousVertices);

    for(int i = 0; i < height; i++){
        free(image[i]);
    }
    free(image);

}


/*void pruebasLineas(PixelRGB **image, char *filename, int width, int height, int numLineas){
    
    PixelRGB color;
    color.R = 255;
    color.G = 0;
    color.B = 0;    

    printf("Numero hilos: ");
    int hilos;
    scanf("%d", &hilos);

    float promedio = 0;

    srand(time(NULL));
    Point2DInt p1, p2;
    for(int i = 0; i < numLineas; i++){
        p1.x = rand() % width;
        p1.y = rand() % height;
        p2.x = rand() % width;
        p2.y = rand() % height;

        clock_t begin, end;
        begin = clock();

        //DDALineDrawing(image, color, p1, p2);
        //naiveLineDrawing(image, color, p1, p2);
        naiveLineDrawing(image, color, p1, p2);
        //parallelNaiveLineDrawing(image, color, p1, p2, hilos);
        
        end = clock();
        float time_spent = (double)(end - begin) / CLOCKS_PER_SEC; 
        promedio += time_spent;
    }

    printf("Promedio: %lf\n", promedio/numLineas);

    
    Point p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16;
    p1.x = 1;
    p1.y = 1;
    p2.x = 100;
    p2.y = 133;

    p3.x = 1;
    p3.y = 1;
    p4.x = 200;
    p4.y = 450;
    
    p5.x = 1;
    p5.y = 400;
    p6.x = 100;
    p6.y = 175;

    p7.x = 1;
    p7.y = 10;
    p8.x = 5;
    p8.y = 1;

    p9.x = 100;
    p9.y = 450;
    p10.x = 100;
    p10.y = 300;

    p11.x = 400;
    p11.y = 300;
    p12.x = 100;
    p12.y = 300;    

    p13.x = 1;
    p13.y = 1;
    p14.x = 300;
    p14.y = 300;

    p15.x = 1;
    p15.y = 300;
    p16.x = 300;
    p16.y = 1;    

    naiveLineDrawing(image, blue, p1, p2);
    naiveLineDrawing(image, blue, p3, p4);
    naiveLineDrawing(image, blue, p5, p6);
    naiveLineDrawing(image, blue, p7, p8);
    naiveLineDrawing(image, blue, p9, p10); 
    naiveLineDrawing(image, blue, p11, p12);
    naiveLineDrawing(image, blue, p13, p14);
    naiveLineDrawing(image, blue, p15, p16);
     
}*/