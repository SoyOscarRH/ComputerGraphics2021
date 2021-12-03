#ifndef COLOR_H
#define COLOR_H

typedef struct Color{
    int R;
    int G;
    int B;
} Color;

void checkMaxColor(Color *color, int maxValue);

Color *interpolateColors(Color A, Color B, int numSteps);
void copyColorArray(Color *a, Color *b, int start, int end);

#endif