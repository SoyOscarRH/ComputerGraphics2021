#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "fileManagement.h"
#include "light.h"
#include "matrix.h"
#include "point.h"
#include "primitives.h"
#include "object3D.h"

Object3D readObjFile(const char *nameFile){

    FILE *fp;
    char *line = NULL;
    size_t len;
    ssize_t read;
    char *splitToken;

    Object3D *obj = (Object3D*)malloc(sizeof(Object3D));

    Point3DNode *firstVertex, *lastVertex;
    char isFirstVertexFound = 0;
    int numVertices = 0;

    int numEdges = 0;

    FaceNode *firstFace, *lastFace;
    int isFirstFaceFound = 0;
    int numFaces = 0;

    fp = fopen(nameFile, "r");
    if(fp == NULL){
        exit(EXIT_FAILURE);
    }

    while((read = getline(&line, &len, fp)) != -1){
        splitToken = strtok(line, " ");
        if(strcmp(splitToken, "v") == 0){
            numVertices++;
            if(isFirstVertexFound == 0){
                firstVertex = (Point3DNode*)malloc(sizeof(Point3DNode));
                splitToken = strtok(NULL, " ");
                firstVertex->point.x = atof(splitToken);
                splitToken = strtok(NULL, " ");
                firstVertex->point.y = atof(splitToken);
                splitToken = strtok(NULL, " ");
                firstVertex->point.z = atof(splitToken);

                lastVertex = firstVertex;
                isFirstVertexFound = 1;
            }else{
                lastVertex->next = (Point3DNode*)malloc(sizeof(Point3DNode));
                splitToken = strtok(NULL, " ");
                lastVertex->next->point.x = atof(splitToken);
                splitToken = strtok(NULL, " ");
                lastVertex->next->point.y = atof(splitToken);
                splitToken = strtok(NULL, " ");
                lastVertex->next->point.z = atof(splitToken);
                lastVertex = lastVertex->next;
            }
        }else if(strcmp(splitToken, "f") == 0){
            if(isFirstFaceFound == 0){
                firstFace = (FaceNode*)malloc(sizeof(FaceNode));
                intNode *aux = (intNode*)malloc(sizeof(intNode));
                int numVertex = 0;

                splitToken = strtok(NULL, " ");
                while(splitToken != NULL){
                    if(numVertex == 0){
                        aux->value = atoi(splitToken);
                        firstFace->faceList = aux;
                    }else{
                        aux->next = (intNode*)malloc(sizeof(intNode));
                        aux->next->value = atoi(splitToken);
                        aux = aux->next;
                    }
                    splitToken = strtok(NULL, " ");
                    numVertex++;
                }

                firstFace->numVertices = numVertex;
                lastFace = firstFace;
                isFirstFaceFound = 1;

                numFaces += numVertex - 2;
                numEdges += (numVertex - 2)*3;
            }else{
                lastFace->next = (FaceNode*)malloc(sizeof(FaceNode));
                intNode *aux = (intNode*)malloc(sizeof(intNode));
                int numVertex = 0;

                splitToken = strtok(NULL, " ");
                while(splitToken != NULL){
                    if(numVertex == 0){
                        aux->value = atoi(splitToken);
                        lastFace->next->faceList = aux;
                    }else{
                        aux->next = (intNode*)malloc(sizeof(intNode));
                        aux->next->value = atoi(splitToken);
                        aux = aux->next;
                    }
                    splitToken = strtok(NULL, " ");
                    numVertex++;
                }
                lastFace->next->numVertices = numVertex;
                lastFace = lastFace->next;

                numFaces += numVertex - 2;
                numEdges += (numVertex - 2)*3;                
            }
        }
    }

    printf("File read...\n");

    obj->numVertices = numVertices;
    obj->vertices = (Point3D*)malloc(numVertices*sizeof(Point3D));

    obj->numEdges = numEdges;
    obj->edges = (int**)malloc(numEdges*sizeof(int*));
    for(int i = 0; i < numEdges; i++){
        obj->edges[i] = (int*)malloc(2*sizeof(int));
    }

    obj->numFaces = numFaces;
    obj->faces = (int**)malloc(numFaces*sizeof(int*));
    for(int i = 0; i < numFaces; i++){
        obj->faces[i] = (int*)malloc(3*sizeof(int));
    }

    Point3DNode *aux = firstVertex;
    int i = 0;
    while(aux){
        obj->vertices[i].x = aux->point.x;
        obj->vertices[i].y = aux->point.y;
        obj->vertices[i].z = aux->point.z;

        i++;
        aux = aux->next;
    }

    FaceNode *auxFace = firstFace;
    int iEdge = 0;
    int iFace = 0;
    while(auxFace){
        int firstVertexFace = auxFace->faceList->value - 1;
        intNode *auxInt = auxFace->faceList->next;
        for(int j = 0; j < auxFace->numVertices - 2; j++){
            obj->faces[iFace][0] = iEdge;
            obj->edges[iEdge][0] = firstVertexFace;
            obj->edges[iEdge][1] = auxInt->value - 1;
            iEdge++;
            obj->faces[iFace][1] = iEdge;
            obj->edges[iEdge][0] = auxInt->value - 1;
            auxInt = auxInt->next;
            obj->edges[iEdge][1] = auxInt->value - 1;
            iEdge++;
            obj->faces[iFace][2] = iEdge;
            obj->edges[iEdge][0] = auxInt->value - 1;
            obj->edges[iEdge][1] = firstVertexFace;
            iEdge++;
            iFace++;
        }
        auxFace = auxFace->next;
    }

    obj->faceNormals = (Point3D*)malloc(numFaces*sizeof(Point3D));
    obj->vertexNormals = (Point3D*)malloc(numVertices*sizeof(Point3D));
    obj->facesColors = (Color*)calloc(numFaces, sizeof(Color));
    obj->diffuseF = (double*)calloc(numFaces, sizeof(double));
    obj->specularF = (double*)calloc(numFaces, sizeof(double));

    obj->vertexFacesAdjacency = (int**)calloc(numVertices, sizeof(int*));
    for(int i = 0; i < numVertices; i++){
        obj->vertexFacesAdjacency[i] = (int*)calloc(numFaces, sizeof(int));
    }

    for(int i = 0; i < numFaces; i++){
        int vertexA = obj->edges[obj->faces[i][0]][0];
        int vertexB = obj->edges[obj->faces[i][1]][0];
        int vertexC = obj->edges[obj->faces[i][2]][0];
        obj->vertexFacesAdjacency[vertexA][i] = 1;
        obj->vertexFacesAdjacency[vertexB][i] = 1;
        obj->vertexFacesAdjacency[vertexC][i] = 1;
    }

    obj->verticesH = (Point3DHomogeneous*)malloc(numVertices*sizeof(Point3DHomogeneous));
    obj->perspectiveVertices = (Point3D*)malloc(numVertices*sizeof(Point3D));
    obj->screenNDCVertices = (Point3D*)malloc(numVertices*sizeof(Point3D));
    obj->rasterVertices = (Point2DInt*)malloc(numVertices*sizeof(Point2DInt));

    obj->perspectiveFaceNormals = (Point3D*)malloc(numFaces*sizeof(Point3D));
    obj->visiblePerspectiveFaces = (int*)calloc(numFaces, sizeof(int));

    fclose(fp);
    
    //Freeing aux list of vertices.
    Point3DNode *auxV = firstVertex;
    Point3DNode *auxV2;
    while(aux){
        auxV2 = auxV;
        auxV = auxV->next;
        free(auxV2);
    }

    //Freeing aux list of faces.
    FaceNode *auxF = firstFace;
    FaceNode *aux2F;
    while(auxF){
        aux2F = auxF;
        auxF = auxF->next;
        intNode *auxI = aux2F->faceList;
        intNode *aux2I;
        while(auxI){
            aux2I = auxI;
            auxI = auxI->next;
            free(aux2I);
        }
        free(aux2F);
    }
    
    if(line){
        free(line);
    }

    return *obj;
}

void calculateFaceNormals(Object3D *obj){

    for(int i = 0; i < obj->numFaces; i++){
        Point3D vertexA = obj->vertices[obj->edges[obj->faces[i][0]][0]];
        Point3D vertexB = obj->vertices[obj->edges[obj->faces[i][0]][1]];
        Point3D vertexC = obj->vertices[obj->edges[obj->faces[i][2]][0]];
        
        Point3D vectorA;
        vectorA.x = vertexB.x - vertexA.x;
        vectorA.y = vertexB.y - vertexA.y;
        vectorA.z = vertexB.z - vertexA.z;

        Point3D vectorB;
        vectorB.x = vertexC.x - vertexA.x;
        vectorB.y = vertexC.y - vertexA.y;
        vectorB.z = vertexC.z - vertexA.z;

        Point3D normal = crossProduct(vectorA, vectorB);
        Point3D normalUnit = unitVector(normal);
        obj->faceNormals[i].x = normalUnit.x;
        obj->faceNormals[i].y = normalUnit.y;
        obj->faceNormals[i].z = normalUnit.z;
    }
}

void calculateVertexNormals(Object3D *obj){
    for(int i = 0; i < obj->numVertices; i++){
        obj->vertexNormals[i] = (Point3D){.x = 0, .y = 0, .z = 0};
        int facesCounter = 0;
        for(int j = 0; j < obj->numFaces; j++){
            if(obj->vertexFacesAdjacency[i][j]){
                facesCounter++;
                obj->vertexNormals[i].x += obj->faceNormals[j].x;
                obj->vertexNormals[i].y += obj->faceNormals[j].y;
                obj->vertexNormals[i].z += obj->faceNormals[j].z;
            }
        }
        obj->vertexNormals[i].x /= facesCounter;
        obj->vertexNormals[i].y /= facesCounter;
        obj->vertexNormals[i].z /= facesCounter;
        
        Point3D normalUnit = unitVector(obj->vertexNormals[i]);
        obj->vertexNormals[i].x = normalUnit.x;
        obj->vertexNormals[i].y = normalUnit.y;
        obj->vertexNormals[i].z = normalUnit.z;
    }
}

void calculatePoint3DHomogeneous(Object3D *obj){
    for(int i = 0; i < obj->numVertices; i++){
        obj->verticesH[i].x = obj->vertices[i].x;
        obj->verticesH[i].y = obj->vertices[i].y;
        obj->verticesH[i].z = obj->vertices[i].z;
        obj->verticesH[i].w = 1;
    }
}

void applyTransformation(Object3D *obj, double **transformationMatrix){

    calculatePoint3DHomogeneous(obj);

    for(int i = 0; i < obj->numVertices; i++){
        matrixPoint3DHMultiplicationInPlace(transformationMatrix, &obj->verticesH[i]);
    }

    Point3D min, max;
    returnFromHomogeneous(obj, &min, &max);
}

void returnFromHomogeneous(Object3D *obj, Point3D *minP, Point3D *maxP){

    minP->x = obj->verticesH[0].x/obj->verticesH[0].w;
    minP->y = obj->verticesH[0].y/obj->verticesH[0].w;
    minP->z = obj->verticesH[0].z/obj->verticesH[0].w;

    maxP->x = obj->verticesH[0].x/obj->verticesH[0].w;
    maxP->y = obj->verticesH[0].y/obj->verticesH[0].w;
    maxP->z = obj->verticesH[0].z/obj->verticesH[0].w;

    obj->vertices[0].x = obj->verticesH[0].x/obj->verticesH[0].w;
    obj->vertices[0].y = obj->verticesH[0].y/obj->verticesH[0].w;
    obj->vertices[0].z = obj->verticesH[0].z/obj->verticesH[0].w;

    for(int i = 1; i < obj->numVertices; i++){
        obj->vertices[i].x = obj->verticesH[i].x/obj->verticesH[i].w;
        obj->vertices[i].y = obj->verticesH[i].y/obj->verticesH[i].w;
        obj->vertices[i].z = obj->verticesH[i].z/obj->verticesH[i].w;
        
        if(obj->vertices[i].x < minP->x) { minP->x = obj->vertices[i].x; }
        if(obj->vertices[i].y < minP->y) { minP->y = obj->vertices[i].y; }
        if(obj->vertices[i].z < minP->z) { minP->z = obj->vertices[i].z; }

        if(obj->vertices[i].x > maxP->x) { maxP->x = obj->vertices[i].x; }
        if(obj->vertices[i].y > maxP->y) { maxP->y = obj->vertices[i].y; }
        if(obj->vertices[i].z > maxP->z) { maxP->z = obj->vertices[i].z; }        
    }
}

void checkVisibleFacesPerspective(Object3D *obj, Point3D *perspectiveVertices, Point3D cameraVector){

    for(int i = 0; i < obj->numFaces; i++){
        
        Point3D vertexA = obj->vertices[obj->edges[obj->faces[i][0]][0]];
        Point3D vertexB = obj->vertices[obj->edges[obj->faces[i][0]][1]];
        Point3D vertexC = obj->vertices[obj->edges[obj->faces[i][2]][0]];

        Point3D pVertexA = obj->perspectiveVertices[obj->edges[obj->faces[i][0]][0]];
        Point3D pVertexB = obj->perspectiveVertices[obj->edges[obj->faces[i][0]][1]];
        Point3D pVertexC = obj->perspectiveVertices[obj->edges[obj->faces[i][2]][0]];        

        Point3D vectorA;
        vectorA.x = pVertexB.x - pVertexA.x;
        vectorA.y = pVertexB.y - pVertexA.y;
        vectorA.z = vertexB.z - vertexA.z;

        Point3D vectorB;
        vectorB.x = pVertexC.x - pVertexA.x;
        vectorB.y = pVertexC.y - pVertexA.y;
        vectorB.z = vertexC.z - vertexA.z;

        Point3D normal = crossProduct(vectorA, vectorB);
        obj->perspectiveFaceNormals[i].x = normal.x;
        obj->perspectiveFaceNormals[i].y = normal.y;
        obj->perspectiveFaceNormals[i].z = normal.z;

        double angleNormalCamera = angleBetweenTwoVectors(normal, cameraVector);

        if(angleNormalCamera >= 90*M_PI/180.0 && angleNormalCamera <= 270*M_PI/180.0){
            obj->visiblePerspectiveFaces[i] = 1;
        }else{
            obj->visiblePerspectiveFaces[i] = 0;
        }
    }
}

void addSpecularReflection(Object3D *obj, int numFace, int numVertexTriangle, Point3D pixelPos, Color *pixelColor, LightNode *lightNode){
    
    SunlightInfo *sunlightInfo = (SunlightInfo*)lightNode->light->info;
    
    int n = 1;
    
    double distanceA = distanceBetween3DPoints(sunlightInfo->position, pixelPos);
    double attenuationA = attenuationInverseQuadratic(distanceA, sunlightInfo->a, sunlightInfo->b, sunlightInfo->c);
    Point3D lightVectorA = (Point3D){.x = pixelPos.x - sunlightInfo->position.x, .y = pixelPos.y - sunlightInfo->position.y, .z = pixelPos.z - sunlightInfo->position.z};
    Point3D pixelCameraV = (Point3D){.x = 0 - pixelPos.x, .y = 0 - pixelPos.y, .z = 0 - pixelPos.z};
    Color resultSpecularA = specularReflection(lightVectorA, obj->vertexNormals[obj->edges[obj->faces[numFace][numVertexTriangle]][0]], pixelCameraV, obj->specularF[numFace], n, attenuationA, lightNode->light->color);
    pixelColor->R += resultSpecularA.R;
    pixelColor->G += resultSpecularA.G;
    pixelColor->B += resultSpecularA.B;    
}


void drawFaces(Object3D *obj, Point2DInt *points2D, Image *image, Point3D cameraVector, int lineDrawingAlgorithm, int fillFaces, LightNode *lights){
    for(int i = 0; i < obj->numFaces; i++){

        if(obj->visiblePerspectiveFaces[i]){
            int sizeLine1, sizeLine2, sizeLine3;

            //Using Gouraud illumination model.

            Point3D vertexA = obj->vertices[obj->edges[obj->faces[i][0]][0]];
            Point3D vertexB = obj->vertices[obj->edges[obj->faces[i][1]][0]];
            Point3D vertexC = obj->vertices[obj->edges[obj->faces[i][2]][0]];

            Point3D triangleCenter;
            triangleCenter.x = (vertexA.x + vertexB.x + vertexC.x)/3;
            triangleCenter.y = (vertexA.y + vertexB.y + vertexC.y)/3;
            triangleCenter.z = (vertexA.z + vertexB.z + vertexC.z)/3;

            Color diffuseColor = {.R = 0, .G = 0, .B = 0};
            Color vertexAColor = {.R = 0, .G = 0, .B = 0};
            Color vertexBColor = {.R = 0, .G = 0, .B = 0};
            Color vertexCColor = {.R = 0, .G = 0, .B = 0};

            LightNode *auxLightNode = lights;
            while(auxLightNode){
                if(auxLightNode->light->type == 0){
                    vertexAColor.R += auxLightNode->light->color.R;
                    vertexAColor.G += auxLightNode->light->color.G;
                    vertexAColor.B += auxLightNode->light->color.B;

                    vertexBColor.R += auxLightNode->light->color.R;
                    vertexBColor.G += auxLightNode->light->color.G;
                    vertexBColor.B += auxLightNode->light->color.B;

                    vertexCColor.R += auxLightNode->light->color.R;
                    vertexCColor.G += auxLightNode->light->color.G;
                    vertexCColor.B += auxLightNode->light->color.B;                                        
                }else if(auxLightNode->light->type == 1){
                    //printf("Sunlight!\n");
                    
                    SunlightInfo *sunligthInfo = (SunlightInfo*)auxLightNode->light->info;
                    
                    //Calculating the diffuse light for a sunlight.
                    double distanceCenter = distanceBetween3DPoints(sunligthInfo->position, triangleCenter);
                    double attenuationCenter = attenuationInverseQuadratic(distanceCenter, sunligthInfo->a, sunligthInfo->b, sunligthInfo->c);
                    Point3D lightVectorCenter = (Point3D){.x = triangleCenter.x - sunligthInfo->position.x, .y = triangleCenter.y - sunligthInfo->position.y, .z = triangleCenter.z - sunligthInfo->position.z};
                    Color resultDiffuseCenter = diffuseReflection(lightVectorCenter, obj->faceNormals[i], obj->diffuseF[i], attenuationCenter, auxLightNode->light->color);
                    diffuseColor.R += resultDiffuseCenter.R;
                    diffuseColor.G += resultDiffuseCenter.G;
                    diffuseColor.B += resultDiffuseCenter.B;

                    addSpecularReflection(obj, i, 0, vertexA, &vertexAColor, auxLightNode);
                    addSpecularReflection(obj, i, 1, vertexB, &vertexBColor, auxLightNode);
                    addSpecularReflection(obj, i, 2, vertexC, &vertexCColor, auxLightNode);

                }else if(auxLightNode->light->type == 2){
                    SpotlightInfo *spotlightInfo = (SpotlightInfo*)auxLightNode->light->info;
                    Point3D lightVector;
                    lightVector.x = spotlightInfo->direction.x - spotlightInfo->position.x;
                    lightVector.y = spotlightInfo->direction.y - spotlightInfo->position.y;
                    lightVector.z = spotlightInfo->direction.z - spotlightInfo->position.z;

                    //Calculating the diffuse light for a spotlight.
                    double distanceCenter = distanceBetween3DPoints(spotlightInfo->position, triangleCenter);
                    double attenuationCenter = attenuationInverseQuadratic(distanceCenter, spotlightInfo->a, spotlightInfo->b, spotlightInfo->c);
                    Point3D positionToCenterVector = (Point3D){.x = triangleCenter.x - spotlightInfo->position.x, .y = triangleCenter.y - spotlightInfo->position.y, .z = triangleCenter.z - spotlightInfo->position.z};
                    double angleLightCenter = angleBetweenTwoVectors(lightVector, positionToCenterVector);
                    if(angleLightCenter <= spotlightInfo->angle){
                        Color resultDiffuseCenter = diffuseReflection(positionToCenterVector, obj->faceNormals[i], obj->diffuseF[i], attenuationCenter, auxLightNode->light->color);
                        diffuseColor.R += cos(angleLightCenter)*resultDiffuseCenter.R;
                        diffuseColor.G += cos(angleLightCenter)*resultDiffuseCenter.G;
                        diffuseColor.B += cos(angleLightCenter)*resultDiffuseCenter.B;
                    }

                    int n = 1;

                    //Calculating specular light for a spotlight.                    
                    double distanceA = distanceBetween3DPoints(spotlightInfo->position, vertexA);
                    double attenuationA = attenuationInverseQuadratic(distanceA, spotlightInfo->a, spotlightInfo->b, spotlightInfo->c);
                    Point3D lightVectorA = (Point3D){.x = vertexA.x - spotlightInfo->position.x, .y = vertexA.y - spotlightInfo->position.y, .z = vertexA.z - spotlightInfo->position.z};
                    double angleLightVertexA = angleBetweenTwoVectors(lightVector, lightVectorA);
                    if(angleLightVertexA <= spotlightInfo->angle){
                        Point3D vertexACameraV = (Point3D){.x = 0 - vertexA.x, .y = 0 - vertexA.y, .z = 0 - vertexA.z};
                        Color resultSpecularA = specularReflection(lightVectorA, obj->vertexNormals[obj->edges[obj->faces[i][0]][0]], vertexACameraV, obj->specularF[i], n, attenuationA, auxLightNode->light->color);
                        vertexAColor.R += cos(angleLightVertexA)*resultSpecularA.R;
                        vertexAColor.G += cos(angleLightVertexA)*resultSpecularA.G;
                        vertexAColor.B += cos(angleLightVertexA)*resultSpecularA.B;
                    }
                    
                    double distanceB = distanceBetween3DPoints(spotlightInfo->position, vertexB);
                    double attenuationB = attenuationInverseQuadratic(distanceB, spotlightInfo->a, spotlightInfo->b, spotlightInfo->c);
                    Point3D lightVectorB = (Point3D){.x = vertexB.x - spotlightInfo->position.x, .y = vertexB.y - spotlightInfo->position.y, .z = vertexB.z - spotlightInfo->position.z};
                    double angleLightVertexB = angleBetweenTwoVectors(lightVector, lightVectorB);
                    if(angleLightVertexB <= spotlightInfo->angle){
                        Point3D vertexBCameraV = (Point3D){.x = 0 - vertexB.x, .y = 0 - vertexB.y, .z = 0 - vertexB.z};
                        Color resultSpecularB = specularReflection(lightVectorB, obj->vertexNormals[obj->edges[obj->faces[i][1]][0]], vertexBCameraV, obj->specularF[i], n, attenuationB, auxLightNode->light->color);
                        vertexBColor.R += cos(angleLightVertexB)*resultSpecularB.R;
                        vertexBColor.G += cos(angleLightVertexB)*resultSpecularB.G;
                        vertexBColor.B += cos(angleLightVertexB)*resultSpecularB.B;
                    }

                    double distanceC = distanceBetween3DPoints(spotlightInfo->position, vertexC);
                    double attenuationC = attenuationInverseQuadratic(distanceC, spotlightInfo->a, spotlightInfo->b, spotlightInfo->c);
                    Point3D lightVectorC = (Point3D){.x = vertexC.x - spotlightInfo->position.x, .y = vertexC.y - spotlightInfo->position.y, .z = vertexC.z - spotlightInfo->position.z};
                    double angleLightVertexC = angleBetweenTwoVectors(lightVector, lightVectorC);
                    if(angleLightVertexC <= spotlightInfo->angle){
                        Point3D vertexCCameraV = (Point3D){.x = 0 - vertexC.x, .y = 0 - vertexC.y, .z = 0 - vertexC.z};
                        Color resultSpecularC = specularReflection(lightVectorC, obj->vertexNormals[obj->edges[obj->faces[i][1]][0]], vertexCCameraV, obj->specularF[i], n, attenuationC, auxLightNode->light->color);
                        vertexCColor.R += cos(angleLightVertexC)*resultSpecularC.R;
                        vertexCColor.G += cos(angleLightVertexC)*resultSpecularC.G;
                        vertexCColor.B += cos(angleLightVertexC)*resultSpecularC.B;
                    }
                }
                auxLightNode = auxLightNode->next;
            }

            vertexAColor.R += diffuseColor.R + obj->facesColors[i].R;
            vertexAColor.G += diffuseColor.G + obj->facesColors[i].G;
            vertexAColor.B += diffuseColor.B + obj->facesColors[i].B;
            checkMaxColor(&vertexAColor, 255);

            vertexBColor.R += diffuseColor.R + obj->facesColors[i].R;
            vertexBColor.G += diffuseColor.G + obj->facesColors[i].G;
            vertexBColor.B += diffuseColor.B + obj->facesColors[i].B;
            checkMaxColor(&vertexBColor, 255);

            vertexCColor.R += diffuseColor.R + obj->facesColors[i].R;
            vertexCColor.G += diffuseColor.G + obj->facesColors[i].G;
            vertexCColor.B += diffuseColor.B + obj->facesColors[i].B;
            checkMaxColor(&vertexCColor, 255);            

            Point2DInt *line1 = BresenhamLineDrawing(points2D[obj->edges[obj->faces[i][0]][0]], points2D[obj->edges[obj->faces[i][0]][1]], &sizeLine1);
            Point2DInt *line2 = BresenhamLineDrawing(points2D[obj->edges[obj->faces[i][1]][0]], points2D[obj->edges[obj->faces[i][1]][1]], &sizeLine2);
            Point2DInt *line3 = BresenhamLineDrawing(points2D[obj->edges[obj->faces[i][2]][0]], points2D[obj->edges[obj->faces[i][2]][1]], &sizeLine3);

            double z0line1 = obj->screenNDCVertices[obj->edges[obj->faces[i][0]][0]].z;
            double z1line1 = obj->screenNDCVertices[obj->edges[obj->faces[i][0]][1]].z;
            double *zValues1 = interpolate(z0line1, z1line1, sizeLine1);
            Color *line1Colors = interpolateColors(vertexAColor, vertexBColor, sizeLine1);

            double z0line2 = obj->screenNDCVertices[obj->edges[obj->faces[i][1]][0]].z;
            double z1line2 = obj->screenNDCVertices[obj->edges[obj->faces[i][1]][1]].z;
            double *zValues2 = interpolate(z0line2, z1line2, sizeLine2);
            Color *line2Colors = interpolateColors(vertexBColor, vertexCColor, sizeLine2);            

            double z0line3 = obj->screenNDCVertices[obj->edges[obj->faces[i][2]][0]].z;
            double z1line3 = obj->screenNDCVertices[obj->edges[obj->faces[i][2]][1]].z;
            double *zValues3 = interpolate(z0line3, z1line3, sizeLine3);
            Color *line3Colors = interpolateColors(vertexCColor, vertexAColor, sizeLine3);

            drawPoints(line1, zValues1, sizeLine1, image, obj->edgesColor);
            drawPoints(line2, zValues2, sizeLine2, image, obj->edgesColor);
            drawPoints(line3, zValues3, sizeLine3, image, obj->edgesColor);
                 

            if(fillFaces && (sizeLine1 + sizeLine2 + sizeLine3 != 0)){

                ScanlineColorsNode *internalPixelsInfo = fillTriangleColors(line1, zValues1, line1Colors, sizeLine1, line2, zValues2, line2Colors, sizeLine2, line3, zValues3, line3Colors, sizeLine3);

                ScanlineColorsNode *aux = internalPixelsInfo;
                ScanlineColorsNode *aux2;
                while(aux){

                    double *zValuesLine = interpolate(aux->zFirst, aux->zLast, aux->numPoints);
                    Color *colorsLine = interpolateColors(aux->firstColor, aux->lastColor, aux->numPoints);
                    
                    double *finalZ = (double*)malloc((aux->numPoints-2)*sizeof(double));
                    Color *finalColors = (Color*)malloc((aux->numPoints-2)*sizeof(Color));
                    Point2DInt *finalPoints = (Point2DInt*)malloc((aux->numPoints-2)*sizeof(Point2DInt));
                    for(int j = 1, k = 0; j < aux->numPoints-1; j++, k++){
                        finalZ[k] = zValuesLine[j];
                        finalColors[k] = (Color){.R = colorsLine[j].R, .G = colorsLine[j].G, .B = colorsLine[j].B};
                        finalPoints[k].x = aux->points[j].x;
                        finalPoints[k].y = aux->points[j].y;
                    }

                    drawPointsColors(finalPoints, zValuesLine, aux->numPoints-2, image, finalColors);
                                        
                    
                    free(zValuesLine);
                    free(colorsLine);
                    free(finalZ);
                    free(finalColors);
                    free(finalPoints);

                    aux2 = aux;
                    aux = aux->next;
                    
                    free(aux2->points);
                    free(aux2);
                }
            }
            
            free(line1);
            free(line2);
            free(line3);
            free(zValues1);
            free(zValues2);
            free(zValues3);
            free(line1Colors);
            free(line2Colors);
            free(line3Colors);
            
        }
    }
}

void drawFrame(const char *nameFrame, Object3D *obj, LightNode *lights, double **transformationMatrix, double **perspectiveMatrix, Point3D cameraVector, int height, int width){
        
    //Creating the image buffer.
    Image image;
    image.raster = (Color**)calloc(height, sizeof(Color*));
    for(int i = 0; i < height; i++){
        image.raster[i] = (Color*)calloc(width, sizeof(Color));
    }

    image.zBuffer = (double**)malloc(height*sizeof(double*));
    for(int i = 0; i < height; i++){
        image.zBuffer[i] = (double*)malloc(width*sizeof(double));
    }

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            image.zBuffer[i][j] = 1.0 / 0.0;
        }
    }

    applyTransformation(obj, transformationMatrix);
    calculateFaceNormals(obj);
    calculateVertexNormals(obj);
    
    calculatePerspectiveVertices(obj, perspectiveMatrix);
    calculateNDCVertices(obj, obj->minPerspective.x-0.1, obj->maxPerspective.x+0.1, obj->minPerspective.y-0.1, obj->maxPerspective.y+0.1);
    calculateRasterVertices(obj, width, height);
    
    checkVisibleFacesPerspective(obj, obj->perspectiveVertices, cameraVector);
    drawFaces(obj, obj->rasterVertices, &image, cameraVector, 0, 1, lights);
    writePPMP3T(nameFrame, &image, height, width);

    for(int i = 0; i < height; i++){
        free(image.zBuffer[i]);
        free(image.raster[i]);
    }
    free(image.zBuffer);
    free(image.raster);
}

void drawPoints(Point2DInt *points, double *zValues, int numPoints, Image *image, Color color){
    for(int i = 0; i < numPoints; i++){
        if(zValues[i] < image->zBuffer[points[i].y][points[i].x]){
            image->raster[points[i].y][points[i].x].R = color.R;
            image->raster[points[i].y][points[i].x].G = color.G;
            image->raster[points[i].y][points[i].x].B = color.B;
            image->zBuffer[points[i].y][points[i].x] = zValues[i];
        }
    }
}

void drawPointsColors(Point2DInt *points, double *zValues, int numPoints, Image *image, Color *colors){
    for(int i = 0; i < numPoints; i++){
        if(zValues[i] < image->zBuffer[points[i].y][points[i].x]){
            image->raster[points[i].y][points[i].x].R = colors[i].R;
            image->raster[points[i].y][points[i].x].G = colors[i].G;
            image->raster[points[i].y][points[i].x].B = colors[i].B;
            image->zBuffer[points[i].y][points[i].x] = zValues[i];
        }
    }    
}


void calculatePerspectiveVertices(Object3D *obj, double **perspectiveMatrix){
    
    calculatePoint3DHomogeneous(obj);
    for(int i = 0; i < obj->numVertices; i++){
        matrixPoint3DHMultiplicationInPlace(perspectiveMatrix, &obj->verticesH[i]);
    }

    obj->minPerspective.x = obj->verticesH[0].x/obj->verticesH[0].w;
    obj->minPerspective.y = obj->verticesH[0].y/obj->verticesH[0].w;
    obj->minPerspective.z = obj->verticesH[0].z/obj->verticesH[0].w;

    obj->maxPerspective.x = obj->verticesH[0].x/obj->verticesH[0].w;
    obj->maxPerspective.y = obj->verticesH[0].y/obj->verticesH[0].w;
    obj->maxPerspective.z = obj->verticesH[0].z/obj->verticesH[0].w;    

    obj->perspectiveVertices[0].x = obj->verticesH[0].x/obj->verticesH[0].w;
    obj->perspectiveVertices[0].y = obj->verticesH[0].y/obj->verticesH[0].w;
    obj->perspectiveVertices[0].z = obj->verticesH[0].z/obj->verticesH[0].w;

    for(int i = 1; i < obj->numVertices; i++){
        obj->perspectiveVertices[i].x = obj->verticesH[i].x/obj->verticesH[i].w;
        obj->perspectiveVertices[i].y = obj->verticesH[i].y/obj->verticesH[i].w;
        obj->perspectiveVertices[i].z = obj->verticesH[i].z/obj->verticesH[i].w;
        
        if(obj->perspectiveVertices[i].x < obj->minPerspective.x) { obj->minPerspective.x = obj->perspectiveVertices[i].x; }
        if(obj->perspectiveVertices[i].y < obj->minPerspective.y) { obj->minPerspective.y = obj->perspectiveVertices[i].y; }
        if(obj->perspectiveVertices[i].z < obj->minPerspective.z) { obj->minPerspective.z = obj->perspectiveVertices[i].z; }

        if(obj->perspectiveVertices[i].x > obj->maxPerspective.x) { obj->maxPerspective.x = obj->perspectiveVertices[i].x; }
        if(obj->perspectiveVertices[i].y > obj->maxPerspective.y) { obj->maxPerspective.y = obj->perspectiveVertices[i].y; }
        if(obj->perspectiveVertices[i].z > obj->maxPerspective.z) { obj->maxPerspective.z = obj->perspectiveVertices[i].z; }        
    }    
}

void calculateNDCVertices(Object3D *obj, double minCanvasWidth, double maxCanvasWidth, double minCanvasHeight, double maxCanvasHeight){
    for(int i = 0; i < obj->numVertices; i++){
        obj->screenNDCVertices[i].x = (obj->perspectiveVertices[i].x - minCanvasWidth)/(maxCanvasWidth - minCanvasWidth);
        obj->screenNDCVertices[i].y = (obj->perspectiveVertices[i].y - minCanvasHeight)/(maxCanvasHeight - minCanvasHeight);
        obj->screenNDCVertices[i].z = obj->vertices[i].z;
    }
}

void calculateRasterVertices(Object3D *obj, int width, int height){
    for(int i = 0; i < obj->numVertices; i++){
        obj->rasterVertices[i].x = obj->screenNDCVertices[i].x * width;
        obj->rasterVertices[i].y = obj->screenNDCVertices[i].y * height;
    }    
}
