#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "color.h"
#include "point.h"

Point3D *homogeneous3Dto3D(Point3DHomogeneous *points3DH, int numPoints, Point3D *minP, Point3D *maxP){
    
    Point3D *newPoints = (Point3D*)calloc(numPoints, sizeof(Point3D));
    minP->x = points3DH[0].x/points3DH[0].w;
    minP->y = points3DH[0].y/points3DH[0].w;
    minP->z = points3DH[0].z/points3DH[0].w;

    maxP->x = points3DH[0].x/points3DH[0].w;
    maxP->y = points3DH[0].y/points3DH[0].w;
    maxP->z = points3DH[0].z/points3DH[0].w;    

    for(int i = 1; i < numPoints; i++){
        newPoints[i].x = points3DH[i].x/points3DH[i].w;
        newPoints[i].y = points3DH[i].y/points3DH[i].w;
        newPoints[i].z = points3DH[i].z/points3DH[i].w;
        
        if(newPoints[i].x < minP->x) { minP->x = newPoints[i].x; }
        if(newPoints[i].y < minP->y) { minP->y = newPoints[i].y; }
        if(newPoints[i].z < minP->z) { minP->z = newPoints[i].z; }

        if(newPoints[i].x > maxP->x) { maxP->x = newPoints[i].x; }
        if(newPoints[i].y > maxP->y) { maxP->y = newPoints[i].y; }
        if(newPoints[i].z > maxP->z) { maxP->z = newPoints[i].z; }        
    }

    return newPoints;
}

double magnitude3DVector(Point3D A){
    return sqrt(A.x*A.x + A.y*A.y + A.z*A.z);
}

Point3D unitVector(Point3D v){    
    double magnitude = magnitude3DVector(v);
    Point3D unitV = {.x = v.x/magnitude, .y = v.y/magnitude, .z = v.z/magnitude};
    return unitV;
}

Point3D crossProduct(Point3D A, Point3D B){
    Point3D res;
    res.x = A.y*B.z - A.z*B.y;
    res.y = -(A.x*B.z - A.z*B.x);
    res.z = A.x*B.y - A.y*B.x;
    return res;
}

double dotProduct(Point3D A, Point3D B){
    return A.x*B.x + A.y*B.y + A.z*B.z;
}

double angleBetweenTwoVectors(Point3D A, Point3D B){
    return acos(dotProduct(A, B)/(magnitude3DVector(A)*magnitude3DVector(B)));
}

double distanceBetween3DPoints(Point3D A, Point3D B){
    double distance = sqrt((B.x - A.x)*(B.x - A.x) + (B.y - A.y)*(B.y - A.y) + (B.z - A.z)*(B.z - A.z));
    return distance;
}

Point3D *screenSpacetoNDC(Point3D *screenPoints, int numPoints, double widthCanvas, double heightCanvas){
    Point3D *NDCPoints = (Point3D*)calloc(numPoints, sizeof(Point3D));
    for(int i = 0; i < numPoints; i++){
        NDCPoints[i].x = (screenPoints[i].x + (widthCanvas/2))/widthCanvas;
        NDCPoints[i].y = (screenPoints[i].y + (heightCanvas/2))/heightCanvas;
        NDCPoints[i].z = screenPoints[i].z;
    }
    return NDCPoints;
}

Point2DInt *NDCtoRaster(Point3D *NDCPoints, int numPoints, int width, int height){
    Point2DInt *points2D = (Point2DInt*)calloc(numPoints, sizeof(Point2DInt));
    for(int i = 0; i < numPoints; i++){
        points2D[i].x = NDCPoints[i].x * width;
        points2D[i].y = NDCPoints[i].y * height;
    }
    return points2D;
}

Point2D *reduce3Dto2DXY(Point3D *points3D, int numPoints){
    
    Point2D *points2D = (Point2D*)calloc(numPoints, sizeof(Point2D));
    for(int i = 0; i < numPoints; i++){
        points2D[i].x = points3D[i].x;
        points2D[i].y = points3D[i].y;
    }
    return points2D;
}

void scale2DPoints(Point2D *points, int numPoints, double scale){

    for(int i = 0; i < numPoints; i++){
        points[i].x *= scale;
        points[i].y *= scale;
    }

}

void center2DPoints(Point2D *points, int numPoints, double xCenter, double yCenter, double xMin, double xMax, double yMin, double yMax){
    
    double xMidpoint, yMidpoint;
    double differenceXMid, differenceYMid;

    xMidpoint = (xMin+xMax)/2;
    yMidpoint = (yMin+yMax)/2;

    differenceXMid = xCenter - xMidpoint;
    differenceYMid = yCenter - yMidpoint;

    for(int i = 0; i < numPoints; i++){
        points[i].x += differenceXMid;
        points[i].y += differenceYMid;
    }    

}

void transformArrayPoint2DDoubleToInt(Point2D *doublePoints, Point2DInt *intPoints, int numPoints){
    for(int i = 0; i < numPoints; i++){
        intPoints[i].x = (int)round(doublePoints[i].x);
        intPoints[i].y = (int)round(doublePoints[i].y);
    }    
}

Point2DInt transformPointToFirstOctant(Point2DInt p, float m){
    
    Point2DInt newP;

    //2nd octant.
    if(m > 1){
        newP.x = p.y;
        newP.y = p.x;
    //4th octant.
    }else if(m < -1){
        newP.x = p.y;
        newP.y = p.x;
        newP.y *= -1;
    //3rd octant.
    }else{
        newP.x = p.x;
        newP.y = p.y * -1;
    }

    return newP;
}

Point2DInt transformPointFromFirstOctant(Point2DInt p, float m){

    Point2DInt newP;

    //2nd octant.
    if(m > 1){
        newP.x = p.y;
        newP.y = p.x;
    //4th octant.
    }else if(m < -1){
        newP.x = p.y*-1;
        newP.y = p.x;
    //3rd octant.
    }else{
        newP.x = p.x;
        newP.y = p.y * -1;
    }

    return newP;

}

void swapPoints(Point2DInt *p1, Point2DInt *p2){    
    p1->x ^= p2->x;
    p2->x ^= p1->x;
    p1->x ^= p2->x;

    p1->y ^= p2->y;
    p2->y ^= p1->y;
    p1->y ^= p2->y;
}

double *interpolate(double firstValue, double lastValue, int numElements){

    double *interpolated = (double*)malloc(numElements*sizeof(double));
    double increment = (lastValue - firstValue)/(numElements - 1.0);
    
    double currentValue = firstValue;
    for(int i = 0; i < numElements - 1; i++){
        interpolated[i] = currentValue;
        currentValue += increment;
    }
    interpolated[numElements-1] = lastValue;

    return interpolated;
}

void copyPoint2DIntArray(Point2DInt *a, Point2DInt *b, int start, int end){
	for(int i = 0; i < end-start; i++){
		b[i].x = a[start+i].x;
        b[i].y = a[start+i].y;
	}    
}

void mergeSort2DIntByY(Point2DInt *a, int start, int size){
	
	int i, j, k;
	int m;
	
	if(size%2 == 0){
		m=start+size/2;
	}else{
		m=start+size/2+1;
	}

	if(size!=1){
			
		Point2DInt *left;
		Point2DInt *right;
		
		if(size%2 == 0){
		 	mergeSort2DIntByY(a, start, size/2);
		 	mergeSort2DIntByY(a, m, size/2);
	 		
	 		left = (Point2DInt*)calloc(size/2, sizeof(Point2DInt));
	 		right = (Point2DInt*)calloc(size/2, sizeof(Point2DInt));
	 		
	 		copyPoint2DIntArray(a, left, start, start+size/2);
	 		copyPoint2DIntArray(a, right, m, m+size/2);
	 		
	 		//printf("\n start: %d final: %d", start, size+start);
	 		k=start;
	 		i=0;
	 		j=0;
	 		while(k<size+start){		
				if(i==size/2){
                    a[k].x = right[j].x;
                    a[k].y = right[j].y;
					j++;
					k++;
				}else if(j==size/2){
                    a[k].x = left[i].x;
                    a[k].y = left[i].y;
					i++;
					k++;
				}else{
					if(left[i].y < right[j].y){
                        a[k].x = left[i].x;
                        a[k].y = left[i].y;
                        i++;
                        k++;
					}else{
                        if(left[i].y == right[j].y){
                            if(left[i].x < right[j].x){
                                a[k].x = left[i].x;
                                a[k].y = left[i].y;
                                i++;
                                k++;
                            }else{
                                a[k].x = right[j].x;
                                a[k].y = right[j].y;
                                j++;
                                k++;
                            }
                        }else{
                            a[k].x = right[j].x;
                            a[k].y = right[j].y;
                            j++;
                            k++;
                        }
					}
				}	
			}
		}else{
			mergeSort2DIntByY(a, start, size/2+1);		 	
			mergeSort2DIntByY(a, m, size/2);		
			
	 		left = (Point2DInt*)calloc(size/2+1, sizeof(Point2DInt));
	 		right = (Point2DInt*)calloc(size/2, sizeof(Point2DInt));			
			 							 
			copyPoint2DIntArray(a, left, start, start+size/2+1);
			copyPoint2DIntArray(a, right, m, m+size/2);	
			
	 		//printf("\n start: %d final: %d", start, size+start);			
	 		k=start;
	 		i=0;
	 		j=0;
	 		while(k<size+start){		
				if(i==size/2+1){
                    a[k].x = right[j].x;
                    a[k].y = right[j].y;
					j++;
					k++;
				}else if(j==size/2){
                    a[k].x = left[i].x;
                    a[k].y = left[i].y;
					i++;
					k++;
				}else{
					if(left[i].y < right[j].y){
                        a[k].x = left[i].x;
                        a[k].y = left[i].y;
                        i++;
                        k++;
					}else{
                        if(left[i].y == right[j].y){
                            if(left[i].x < right[j].x){
                                a[k].x = left[i].x;
                                a[k].y = left[i].y;
                                i++;
                                k++;
                            }else{
                                a[k].x = right[j].x;
                                a[k].y = right[j].y;
                                j++;
                                k++;
                            }
                        }else{
                            a[k].x = right[j].x;
                            a[k].y = right[j].y;
                            j++;
                            k++;
                        }
					}
				}
			}							
						
		}
		free(left);
		free(right);	
	}    
}

void copyPoint3DArray(Point3D *a, Point3D *b, int start, int end){
	for(int i = 0; i < end-start; i++){
		b[i].x = a[start+i].x;
        b[i].y = a[start+i].y;
        b[i].z = a[start+i].z;
	}
}

void mergeSort3DByY(Point3D *a, int start, int size){
    int i, j, k;
	int m;
	
	if(size%2 == 0){
		m=start+size/2;
	}else{
		m=start+size/2+1;
	}

	if(size!=1){
			
		Point3D *left;
		Point3D *right;
		
		if(size%2 == 0){
		 	mergeSort3DByY(a, start, size/2);
		 	mergeSort3DByY(a, m, size/2);
	 		
	 		left = (Point3D*)calloc(size/2, sizeof(Point3D));
	 		right = (Point3D*)calloc(size/2, sizeof(Point3D));
	 		
	 		copyPoint3DArray(a, left, start, start+size/2);
	 		copyPoint3DArray(a, right, m, m+size/2);
	 		
	 		//printf("\n start: %d final: %d", start, size+start);
	 		k=start;
	 		i=0;
	 		j=0;
	 		while(k<size+start){		
				if(i==size/2){
                    a[k].x = right[j].x;
                    a[k].y = right[j].y;
                    a[k].z = right[j].z;
					j++;
					k++;
				}else if(j==size/2){
                    a[k].x = left[i].x;
                    a[k].y = left[i].y;
                    a[k].z = left[i].z;
					i++;
					k++;
				}else{
					if(left[i].y < right[j].y){
                        a[k].x = left[i].x;
                        a[k].y = left[i].y;
                        a[k].z = left[i].z;
                        i++;
                        k++;
					}else{
                        if(left[i].y == right[j].y){
                            if(left[i].x < right[j].x){
                                a[k].x = left[i].x;
                                a[k].y = left[i].y;
                                a[k].z = left[i].z;
                                i++;
                                k++;
                            }else{
                                a[k].x = right[j].x;
                                a[k].y = right[j].y;
                                a[k].z = right[j].z;
                                j++;
                                k++;
                            }
                        }else{
                            a[k].x = right[j].x;
                            a[k].y = right[j].y;
                            a[k].z = right[j].z;
                            j++;
                            k++;
                        }
					}
				}	
			}
		}else{
			mergeSort3DByY(a, start, size/2+1);		 	
			mergeSort3DByY(a, m, size/2);		
			
	 		left = (Point3D*)calloc(size/2+1, sizeof(Point3D));
	 		right = (Point3D*)calloc(size/2, sizeof(Point3D));			
			 							 
			copyPoint3DArray(a, left, start, start+size/2+1);
			copyPoint3DArray(a, right, m, m+size/2);	
			
	 		//printf("\n start: %d final: %d", start, size+start);			
	 		k=start;
	 		i=0;
	 		j=0;
	 		while(k<size+start){		
				if(i==size/2+1){
                    a[k].x = right[j].x;
                    a[k].y = right[j].y;
                    a[k].z = right[j].z;
					j++;
					k++;
				}else if(j==size/2){
                    a[k].x = left[i].x;
                    a[k].y = left[i].y;
                    a[k].z = left[i].z;
					i++;
					k++;
				}else{
					if(left[i].y < right[j].y){
                        a[k].x = left[i].x;
                        a[k].y = left[i].y;
                        a[k].z = left[i].z;
                        i++;
                        k++;
					}else{
                        if(left[i].y == right[j].y){
                            if(left[i].x < right[j].x){
                                a[k].x = left[i].x;
                                a[k].y = left[i].y;
                                a[k].z = left[i].z;
                                i++;
                                k++;
                            }else{
                                a[k].x = right[j].x;
                                a[k].y = right[j].y;
                                a[k].z = right[j].z;
                                j++;
                                k++;
                            }
                        }else{
                            a[k].x = right[j].x;
                            a[k].y = right[j].y;
                            a[k].z = right[j].z;
                            j++;
                            k++;
                        }
					}
				}
			}							
						
		}
		free(left);
		free(right);	
	}
}

void mergeSort3DColorByY(Point3D *a, Color* colorA, int start, int size){
    int i, j, k;
	int m;
	
	if(size%2 == 0){
		m=start+size/2;
	}else{
		m=start+size/2+1;
	}

	if(size!=1){
			
		Point3D *left;
		Point3D *right;
        Color *leftColors;
        Color *rightColors;
		
		if(size%2 == 0){
		 	mergeSort3DColorByY(a, colorA, start, size/2);
		 	mergeSort3DColorByY(a, colorA, m, size/2);
	 		
	 		left = (Point3D*)malloc((size/2)*sizeof(Point3D));
	 		right = (Point3D*)malloc((size/2)*sizeof(Point3D));
            leftColors = (Color*)malloc((size/2)*sizeof(Color));
            rightColors = (Color*)malloc((size/2)*sizeof(Color));
	 		
	 		copyPoint3DArray(a, left, start, start+size/2);
	 		copyPoint3DArray(a, right, m, m+size/2);
            copyColorArray(colorA, leftColors, start, start+size/2);
            copyColorArray(colorA, rightColors, m, m+size/2);
	 		
	 		//printf("\n start: %d final: %d", start, size+start);
	 		k=start;
	 		i=0;
	 		j=0;
	 		while(k<size+start){		
				if(i==size/2){
                    a[k].x = right[j].x;
                    a[k].y = right[j].y;
                    a[k].z = right[j].z;
                    colorA[k] = (Color){.R = rightColors[j].R, .G = rightColors[j].G, .B = rightColors[j].B};
					j++;
					k++;
				}else if(j==size/2){
                    a[k].x = left[i].x;
                    a[k].y = left[i].y;
                    a[k].z = left[i].z;
                    colorA[k] = (Color){.R = leftColors[i].R, .G = leftColors[i].G, .B = leftColors[i].B};
					i++;
					k++;
				}else{
					if(left[i].y < right[j].y){
                        a[k].x = left[i].x;
                        a[k].y = left[i].y;
                        a[k].z = left[i].z;
                        colorA[k] = (Color){.R = leftColors[i].R, .G = leftColors[i].G, .B = leftColors[i].B};
                        i++;
                        k++;
					}else{
                        if(left[i].y == right[j].y){
                            if(left[i].x < right[j].x){
                                a[k].x = left[i].x;
                                a[k].y = left[i].y;
                                a[k].z = left[i].z;
                                colorA[k] = (Color){.R = leftColors[i].R, .G = leftColors[i].G, .B = leftColors[i].B};
                                i++;
                                k++;
                            }else{
                                a[k].x = right[j].x;
                                a[k].y = right[j].y;
                                a[k].z = right[j].z;
                                colorA[k] = (Color){.R = rightColors[j].R, .G = rightColors[j].G, .B = rightColors[j].B};
                                j++;
                                k++;
                            }
                        }else{
                            a[k].x = right[j].x;
                            a[k].y = right[j].y;
                            a[k].z = right[j].z;
                            colorA[k] = (Color){.R = rightColors[j].R, .G = rightColors[j].G, .B = rightColors[j].B};
                            j++;
                            k++;
                        }
					}
				}	
			}
		}else{
		 	mergeSort3DColorByY(a, colorA, start, size/2+1);
		 	mergeSort3DColorByY(a, colorA, m, size/2);
	 		
            left = (Point3D*)malloc((size/2+1)*sizeof(Point3D));
	 		right = (Point3D*)malloc((size/2)*sizeof(Point3D));
            leftColors = (Color*)malloc((size/2+1)*sizeof(Color));
            rightColors = (Color*)malloc((size/2)*sizeof(Color));
	 		
	 		copyPoint3DArray(a, left, start, start+size/2+1);
	 		copyPoint3DArray(a, right, m, m+size/2);
            copyColorArray(colorA, leftColors, start, start+size/2+1);
            copyColorArray(colorA, rightColors, m, m+size/2);            
			
	 		//printf("\n start: %d final: %d", start, size+start);			
	 		k=start;
	 		i=0;
	 		j=0;
	 		while(k<size+start){		
				if(i==size/2+1){
                    a[k].x = right[j].x;
                    a[k].y = right[j].y;
                    a[k].z = right[j].z;
                    colorA[k] = (Color){.R = rightColors[j].R, .G = rightColors[j].G, .B = rightColors[j].B};
					j++;
					k++;
				}else if(j==size/2){
                    a[k].x = left[i].x;
                    a[k].y = left[i].y;
                    a[k].z = left[i].z;
                    colorA[k] = (Color){.R = leftColors[i].R, .G = leftColors[i].G, .B = leftColors[i].B};
					i++;
					k++;
				}else{
					if(left[i].y < right[j].y){
                        a[k].x = left[i].x;
                        a[k].y = left[i].y;
                        a[k].z = left[i].z;
                        colorA[k] = (Color){.R = leftColors[i].R, .G = leftColors[i].G, .B = leftColors[i].B};
                        i++;
                        k++;
					}else{
                        if(left[i].y == right[j].y){
                            if(left[i].x < right[j].x){
                                a[k].x = left[i].x;
                                a[k].y = left[i].y;
                                a[k].z = left[i].z;
                                colorA[k] = (Color){.R = leftColors[i].R, .G = leftColors[i].G, .B = leftColors[i].B};
                                i++;
                                k++;
                            }else{
                                a[k].x = right[j].x;
                                a[k].y = right[j].y;
                                a[k].z = right[j].z;
                                colorA[k] = (Color){.R = rightColors[j].R, .G = rightColors[j].G, .B = rightColors[j].B};
                                j++;
                                k++;
                            }
                        }else{
                            a[k].x = right[j].x;
                            a[k].y = right[j].y;
                            a[k].z = right[j].z;
                            colorA[k] = (Color){.R = rightColors[j].R, .G = rightColors[j].G, .B = rightColors[j].B};
                            j++;
                            k++;
                        }
					}
				}
            }				
		}
        
		free(left);
		free(right);
        free(leftColors);
        free(rightColors);
        
	}
}
