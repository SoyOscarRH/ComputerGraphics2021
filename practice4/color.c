#include <stdlib.h>
#include "color.h"

void checkMaxColor(Color *color, int maxValue){
    
    if(color->R > maxValue){
        color->R = maxValue;
    }

    if(color->G > maxValue){
        color->G = maxValue;
    }

    if(color->B > maxValue){
        color->B = maxValue;
    }

}

Color *interpolateColors(Color A, Color B, int n){
    Color *interpolated = (Color*)malloc(n*sizeof(Color));
    double incrementR = (B.R - A.R)/(n - 1.0);
    double incrementG = (B.G - A.G)/(n - 1.0);
    double incrementB = (B.B - A.B)/(n - 1.0);

    double currentR = A.R;
    double currentG = A.G;
    double currentB = A.B;
    for(int i = 0; i < n - 1; i++){
        interpolated[i].R = (int)currentR;
        interpolated[i].G = (int)currentG;
        interpolated[i].B = (int)currentB;
        currentR += incrementR;
        currentG += incrementG;
        currentB += incrementB;
    }
    interpolated[n - 1].R = B.R;
    interpolated[n - 1].G = B.G;
    interpolated[n - 1].B = B.B;

    return interpolated;
}

void copyColorArray(Color *a, Color *b, int start, int end){
	for(int i = 0; i < end-start; i++){
		b[i].R = a[start+i].R;
        b[i].G = a[start+i].G;
        b[i].B = a[start+i].B;
	}
}
