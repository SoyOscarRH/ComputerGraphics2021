#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "light.h"

void addLight(LightNode **lightList, Light *newLight){
    if(!*lightList){
        *lightList = (LightNode*)calloc(1, sizeof(LightNode));
        (*lightList)->light = newLight;
        (*lightList)->next = NULL;
    }else{
        LightNode *aux = (LightNode*)calloc(1, sizeof(LightNode));
        aux->light = newLight;
        aux->next = *lightList;
        *lightList = aux;
    }
}

double attenuationInverseQuadratic(double distance, double a, double b, double c){
    double attenuation = 1/(a*distance*distance + b*distance + c);
    return attenuation;
}

Color diffuseReflection(Point3D lightVector, Point3D faceNormal, double diffuseF, double attenuation, Color lightColor){

    Color diffuseColor;
    Point3D reflectedVector;

    Point3D unitNormal = unitVector(faceNormal);
    double reflectedVectorF = 2*dotProduct(lightVector, unitNormal);
    
    reflectedVector.x = lightVector.x - reflectedVectorF*unitNormal.x;
    reflectedVector.y = lightVector.y - reflectedVectorF*unitNormal.y;
    reflectedVector.z = lightVector.z - reflectedVectorF*unitNormal.z;

    double angle = angleBetweenTwoVectors(unitNormal, reflectedVector);
    double diffuseValue = (cos(angle) < 0) ? 0.0 : cos(angle);
    
    diffuseColor.R = (int)round(diffuseF*diffuseValue*attenuation*lightColor.R);
    diffuseColor.G = (int)round(diffuseF*diffuseValue*attenuation*lightColor.G);
    diffuseColor.B = (int)round(diffuseF*diffuseValue*attenuation*lightColor.B);

    return diffuseColor;
}

Color specularReflection(Point3D lightVector, Point3D surfaceNormal, Point3D cameraVector, double specularF, int n, double attenuation, Color lightColor){
    
    Color specularColor;
    Point3D reflectedVector;

    Point3D unitNormal = unitVector(surfaceNormal);
    double reflectedVectorF = 2*dotProduct(lightVector, unitNormal);
    
    reflectedVector.x = lightVector.x - reflectedVectorF*unitNormal.x;
    reflectedVector.y = lightVector.y - reflectedVectorF*unitNormal.y;
    reflectedVector.z = lightVector.z - reflectedVectorF*unitNormal.z;

    double angle = angleBetweenTwoVectors(cameraVector, reflectedVector);
    double specularValue = (cos(angle) < 0) ? 0.0 : pow(cos(angle), n);

    specularColor.R = (int)round(specularF*specularValue*attenuation*lightColor.R);
    specularColor.G = (int)round(specularF*specularValue*attenuation*lightColor.G);
    specularColor.B = (int)round(specularF*specularValue*attenuation*lightColor.B);

    return specularColor;    
}
