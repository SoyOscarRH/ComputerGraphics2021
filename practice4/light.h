#ifndef LIGHT_H
#define LIGHT_H

#include "color.h"
#include "point.h"

typedef struct Light{
    Color color;
    int type; // 0 = Ambient, 1 = Sun, 2 = Spotlight
    void *info;
} Light;

typedef struct SunlightInfo{
    int a;
    int b;
    int c;
    Point3D position;
} SunlightInfo;

typedef struct SpotlightInfo{
    int a;
    int b;
    int c;
    Point3D position;
    Point3D direction;
    double angle;
} SpotlightInfo;

typedef struct LightNode{
    Light *light;
    struct LightNode *next;
} LightNode;

void addLight(LightNode **lightList, Light *newLight);

double attenuationInverseQuadratic(double distance, double a, double b, double c);

Color diffuseReflection(Point3D lightVector, Point3D faceNormal, double diffuseF, double attenuation, Color lightColor);
Color specularReflection(Point3D lightVector, Point3D surfaceNormal, Point3D cameraVector, double specularF, int n, double attenuation, Color lightColor);

#endif