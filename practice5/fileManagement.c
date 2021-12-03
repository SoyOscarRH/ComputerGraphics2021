#include <stdio.h>
#include "color.h"
#include "fileManagement.h"

void writePPMP3(const char *fileName, Color **image, int height, int width){
    
    FILE *fileP;

    fileP = fopen(fileName, "w");

    //printf("Archivo %s abierto...\n", fileName);

    fprintf(fileP, "P3\n");
    fprintf(fileP, "%d %d\n", width, height);
    fprintf(fileP, "255\n");

    int pixelCount = 0;

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(pixelCount == 5){
                fprintf(fileP, "\n");
                pixelCount = 0;
            }
            fprintf(fileP, "%*d %*d %*d  ", 3, image[i][j].R, 3, image[i][j].G, 3, image[i][j].B);
            pixelCount++;
        }
    }

    fclose(fileP);
    printf("%s saved\n", fileName);

}
