#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "face.h"
#include "pixel.h"
#include "point.h"
#include "primitives.h"
#include "render.h"
#include "threadPool.h"

Point2DInt *
naiveLineDrawing (Point2DInt p1, Point2DInt p2, int *numElements)
{

  int dx = p2.x - p1.x;
  int dy = p2.y - p1.y;
  float m, b;
  Point2DInt *points;

  if (dx == 0)
    {
      if (p1.y > p2.y)
        {
          swapPoints (&p1, &p2);
        }

      *numElements = p2.y - p1.y;
      points = (Point2DInt *)calloc (*numElements, sizeof (Point2DInt));

      int i, y;
      for (i = 0, y = p1.y; y < p2.y; i++, y++)
        {
          points[i].x = p1.x;
          points[i].y = y;
        }
    }
  else if (dy == 0)
    {
      if (p1.x > p2.x)
        {
          swapPoints (&p1, &p2);
        }

      *numElements = p2.x - p1.x;
      points = (Point2DInt *)calloc (*numElements, sizeof (Point2DInt));

      int i, x;
      for (i = 0, x = p1.x; x < p2.x; i++, x++)
        {
          points[i].x = x;
          points[i].y = p1.y;
        }
    }
  else if ((float)dy / dx == 1)
    {
      if (p1.x > p2.x)
        {
          swapPoints (&p1, &p2);
        }

      *numElements = p2.x - p1.x;
      points = (Point2DInt *)calloc (*numElements, sizeof (Point2DInt));

      int i, x, y;
      for (i = 0, x = p1.x, y = p1.y; x < p2.x; i++, x++, y++)
        {
          points[i].x = x;
          points[i].y = y;
        }
    }
  else if ((float)dy / dx == -1)
    {
      if (p1.x > p2.x)
        {
          swapPoints (&p1, &p2);
        }

      *numElements = p2.x - p1.x;
      points = (Point2DInt *)calloc (p2.x - p1.x, sizeof (Point2DInt));

      int i, x, y;
      for (i = 0, x = p1.x, y = p1.y; x < p2.x; i++, x++, y--)
        {
          points[i].x = x;
          points[i].y = y;
        }
    }
  else
    {
      m = (float)dy / dx;
      b = p1.y - m * p1.x;

      if (m > 0 && m < 1)
        {
          *numElements = 1 + p2.x - p1.x;
          points = (Point2DInt *)calloc (*numElements, sizeof (Point2DInt));
          int i, x;
          for (i = 0, x = p1.x; x <= p2.x; x++, i++)
            {
              int yVal = (int)round (m * x + b);
              points[i].x = x;
              points[i].y = yVal;
            }
        }
      else
        {
          Point2DInt p1t = transformPointToFirstOctant (p1, m);
          Point2DInt p2t = transformPointToFirstOctant (p2, m);

          if (p1t.x > p2t.x)
            {
              swapPoints (&p1t, &p2t);
            }

          float mTransformed = (float)(p2t.y - p1t.y) / (p2t.x - p1t.x);
          float bTransformed = (float)(p1t.y - mTransformed * p1t.x);

          Point2DInt *transformedPoints
              = (Point2DInt *)calloc (1 + p2t.x - p1t.x, sizeof (Point2DInt));
          int counter = 0;
          for (int i = p1t.x; i <= p2t.x; i++)
            {
              transformedPoints[counter].x = i;
              transformedPoints[counter].y
                  = (int)round (mTransformed * i + bTransformed);
              counter++;
            }

          *numElements = 1 + p2t.x - p1t.x;
          points = (Point2DInt *)calloc (*numElements, sizeof (Point2DInt));
          for (int i = 0; i <= p2t.x - p1t.x; i++)
            {
              points[i]
                  = transformPointFromFirstOctant (transformedPoints[i], m);
            }
        }
    }

  return points;
}

void
parallelNaive (void *lineInfo)
{

  // printf("Corriendo el parallel Naive...\n");
  Point2DInt *linePoints;
  int numPoints;
  InfoParallelLineDrawing *info = (InfoParallelLineDrawing *)lineInfo;

  linePoints = naiveLineDrawing (info->p1, info->p2, &numPoints);

  // printf("Dibujando con: %d %d %d\n", (*info->color).R, (*info->color).G,
  // (*info->color).B);
  pthread_mutex_lock (&info->pool->mutexImage);
  setLine (linePoints, numPoints, info->image, *info->color);
  pthread_mutex_unlock (&info->pool->mutexImage);

  // free(lineInfo);
}

void
parallelNaiveLineDrawing (PixelRGB **image, PixelRGB *color, Point2DInt p1,
                          Point2DInt p2, int numThreads, ThreadPool *pool)
{

  // printf("Puntos: (%d, %d) -> (%d, %d)\n", p1.x, p1.y, p2.x, p2.y);

  // clock_t begin, end;
  // begin = clock();

  float xInc = (float)(p2.x - p1.x) / numThreads;
  float yInc = (float)(p2.y - p1.y) / numThreads;
  ;
  // printf("Incrementos: %f %f\n", xInc, yInc);
  float realX = p1.x;
  float realY = p1.y;
  InfoParallelLineDrawing *infoThread = (InfoParallelLineDrawing *)calloc (
      numThreads, sizeof (InfoParallelLineDrawing));
  // printf("Calculando linea...\n");
  for (int i = 0; i < numThreads; i++)
    {
      // printf("%d: %d -> %d (%f)\n", i, (int)realX, (int)(realX+xInc),
      // realX+xInc); printf("%d: %d -> %d (%f)\n", i, (int)realY,
      // (int)(realY+yInc), realY+yInc); printf("\n");
      infoThread[i].p1.x = (int)realX;
      infoThread[i].p1.y = (int)realY;
      realX += xInc;
      realY += yInc;
      infoThread[i].p2.x = (int)realX;
      infoThread[i].p2.y = (int)realY;
      infoThread[i].image = image;
      infoThread[i].color = color;
      infoThread[i].pool = pool;
      addTask (pool, &parallelNaive, (void *)&infoThread[i]);
    }
  // printf("Tareas agregadas...\n");

  // free(infoThread->points);
  // free(infoThread);
  // end = clock();
  // float time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  // printf("Tiempo linea: %f\n", time_spent);
}

Point2DInt *
DDALineDrawing (Point2DInt p1, Point2DInt p2, int *numElements)
{

  int dx = p2.x - p1.x;
  int dy = p2.y - p1.y;
  float m;
  Point2DInt *points;

  if (dx == 0)
    {
      if (p1.y > p2.y)
        {
          swapPoints (&p1, &p2);
        }

      *numElements = p2.y - p1.y;
      points = (Point2DInt *)calloc (*numElements, sizeof (Point2DInt));

      int i, y;
      for (i = 0, y = p1.y; y < p2.y; i++, y++)
        {
          points[i].x = p1.x;
          points[i].y = y;
        }
    }
  else if (dy == 0)
    {
      if (p1.x > p2.x)
        {
          swapPoints (&p1, &p2);
        }

      *numElements = p2.x - p1.x;
      points = (Point2DInt *)calloc (*numElements, sizeof (Point2DInt));

      int i, x;
      for (i = 0, x = p1.x; x < p2.x; i++, x++)
        {
          points[i].x = x;
          points[i].y = p1.y;
        }
    }
  else if ((float)dy / dx == 1)
    {
      if (p1.x > p2.x)
        {
          swapPoints (&p1, &p2);
        }

      *numElements = p2.x - p1.x;
      points = (Point2DInt *)calloc (*numElements, sizeof (Point2DInt));

      int i, x, y;
      for (i = 0, x = p1.x, y = p1.y; x < p2.x; i++, x++, y++)
        {
          points[i].x = x;
          points[i].y = y;
        }
    }
  else if ((float)dy / dx == -1)
    {
      if (p1.x > p2.x)
        {
          swapPoints (&p1, &p2);
        }

      *numElements = p2.x - p1.x;
      points = (Point2DInt *)calloc (p2.x - p1.x, sizeof (Point2DInt));

      int i, x, y;
      for (i = 0, x = p1.x, y = p1.y; x < p2.x; i++, x++, y--)
        {
          points[i].x = x;
          points[i].y = y;
        }
    }
  else
    {
      m = (float)dy / dx;

      if (m > 0 && m < 1)
        {
          float y = p1.y;
          int yInt;
          *numElements = 1 + p2.x - p1.x;
          points = (Point2DInt *)calloc (*numElements, sizeof (Point2DInt));
          int i, x;
          for (i = 0, x = p1.x; x <= p2.x; x++, i++)
            {
              yInt = (int)round (y);
              points[i].x = x;
              points[i].y = yInt;
              y += m;
            }
        }
      else
        {
          Point2DInt p1t = transformPointToFirstOctant (p1, m);
          Point2DInt p2t = transformPointToFirstOctant (p2, m);

          if (p1t.x > p2t.x)
            {
              swapPoints (&p1t, &p2t);
            }

          float mTransformed = (float)(p2t.y - p1t.y) / (p2t.x - p1t.x);

          Point2DInt *transformedPoints
              = (Point2DInt *)calloc (1 + p2t.x - p1t.x, sizeof (Point2DInt));
          int counter = 0;
          float y = p1t.y;
          for (int i = p1t.x; i <= p2t.x; i++)
            {
              transformedPoints[counter].x = i;
              transformedPoints[counter].y = (int)round (y);
              y += mTransformed;
              counter++;
            }
          *numElements = 1 + p2t.x - p1t.x;
          points = (Point2DInt *)calloc (*numElements, sizeof (Point2DInt));

          for (int i = 0; i <= p2t.x - p1t.x; i++)
            {
              points[i]
                  = transformPointFromFirstOctant (transformedPoints[i], m);
            }
        }
    }
  return points;
}

Point2DInt *
BresenhamLineDrawing (Point2DInt p1, Point2DInt p2, int *numElements)
{

  int dx = p2.x - p1.x;
  int dy = p2.y - p1.y;
  Point2DInt *points;

  if (dx == 0)
    {
      if (p1.y > p2.y)
        {
          swapPoints (&p1, &p2);
        }

      *numElements = p2.y - p1.y;
      points = (Point2DInt *)calloc (*numElements, sizeof (Point2DInt));

      int i, y;
      for (i = 0, y = p1.y; y < p2.y; i++, y++)
        {
          points[i].x = p1.x;
          points[i].y = y;
        }
    }
  else if (dy == 0)
    {
      if (p1.x > p2.x)
        {
          swapPoints (&p1, &p2);
        }

      *numElements = p2.x - p1.x;
      points = (Point2DInt *)calloc (*numElements, sizeof (Point2DInt));

      int i, x;
      for (i = 0, x = p1.x; x < p2.x; i++, x++)
        {
          points[i].x = x;
          points[i].y = p1.y;
        }
    }
  else if ((float)dy / dx == 1)
    {
      if (p1.x > p2.x)
        {
          swapPoints (&p1, &p2);
        }

      *numElements = p2.x - p1.x;
      points = (Point2DInt *)calloc (*numElements, sizeof (Point2DInt));

      int i, x, y;
      for (i = 0, x = p1.x, y = p1.y; x < p2.x; i++, x++, y++)
        {
          points[i].x = x;
          points[i].y = y;
        }
    }
  else if ((float)dy / dx == -1)
    {
      if (p1.x > p2.x)
        {
          swapPoints (&p1, &p2);
        }

      *numElements = p2.x - p1.x;
      points = (Point2DInt *)calloc (p2.x - p1.x, sizeof (Point2DInt));

      int i, x, y;
      for (i = 0, x = p1.x, y = p1.y; x < p2.x; i++, x++, y--)
        {
          points[i].x = x;
          points[i].y = y;
        }
    }
  else
    {
      float m = (float)dy / dx;

      if (m > 0 && m < 1)
        {
          if (p1.x > p2.x)
            {
              swapPoints (&p1, &p2);
              dx *= -1;
              dy *= -1;
            }

          *numElements = 1 + p2.x - p1.x;
          points = (Point2DInt *)calloc (*numElements, sizeof (Point2DInt));

          int d = 2 * dy - dx;
          int y = p1.y;
          int i = 0;
          int x;
          points[i].x = p1.x;
          points[i].y = y;
          for (i = 1, x = p1.x + 1; x <= p2.x; i++, x++)
            {
              if (d < 0)
                {
                  d += 2 * dy;
                }
              else
                {
                  d += 2 * (dy - dx);
                  y++;
                }
              points[i].x = x;
              points[i].y = y;
            }
        }
      else
        {
          Point2DInt p1t = transformPointToFirstOctant (p1, m);
          Point2DInt p2t = transformPointToFirstOctant (p2, m);

          if (p1t.x > p2t.x)
            {
              swapPoints (&p1t, &p2t);
            }

          Point2DInt *transformedPoints
              = (Point2DInt *)calloc (1 + p2t.x - p1t.x, sizeof (Point2DInt));
          int counter = 1;
          int dxT = p2t.x - p1t.x;
          int dyT = p2t.y - p1t.y;
          int y = p1t.y;

          int d = 2 * dyT - dxT;
          transformedPoints[0].x = p1t.x;
          transformedPoints[0].y = p1t.y;

          for (int i = p1t.x + 1; i <= p2t.x; i++)
            {
              if (d < 0)
                {
                  d += 2 * dyT;
                }
              else
                {
                  d += 2 * (dyT - dxT);
                  y++;
                }
              transformedPoints[counter].x = i;
              transformedPoints[counter].y = y;
              counter++;
            }

          *numElements = 1 + p2t.x - p1t.x;
          points = (Point2DInt *)calloc (*numElements, sizeof (Point2DInt));

          for (int i = 0; i <= p2t.x - p1t.x; i++)
            {
              points[i]
                  = transformPointFromFirstOctant (transformedPoints[i], m);
            }
        }
    }
  return points;
}

void
drawFace (PixelRGB **image, PixelRGB color, FaceNode face,
          Point2DInt *vertices, int lineAlgorithm)
{

  intNode *aux = face.face;

  Point2DInt *linePoints;
  int numPoints;

  switch (lineAlgorithm)
    {
    case 0:
      while (aux->next)
        {
          linePoints
              = naiveLineDrawing (vertices[aux->value - 1],
                                  vertices[aux->next->value - 1], &numPoints);
          setLine (linePoints, numPoints, image, color);
          aux = aux->next;
          free (linePoints);
        }
      linePoints
          = naiveLineDrawing (vertices[aux->value - 1],
                              vertices[face.face->value - 1], &numPoints);
      setLine (linePoints, numPoints, image, color);
      free (linePoints);
      break;

    case 1:
      while (aux->next)
        {
          linePoints
              = DDALineDrawing (vertices[aux->value - 1],
                                vertices[aux->next->value - 1], &numPoints);
          setLine (linePoints, numPoints, image, color);
          aux = aux->next;
          free (linePoints);
        }
      linePoints = DDALineDrawing (vertices[aux->value - 1],
                                   vertices[face.face->value - 1], &numPoints);
      setLine (linePoints, numPoints, image, color);
      free (linePoints);
      break;

    case 2:
      while (aux->next)
        {
          linePoints = BresenhamLineDrawing (vertices[aux->value - 1],
                                             vertices[aux->next->value - 1],
                                             &numPoints);
          setLine (linePoints, numPoints, image, color);
          aux = aux->next;
          free (linePoints);
        }
      linePoints
          = BresenhamLineDrawing (vertices[aux->value - 1],
                                  vertices[face.face->value - 1], &numPoints);
      setLine (linePoints, numPoints, image, color);
      free (linePoints);
      break;
    }
}

void
drawFaceParallel (PixelRGB **image, PixelRGB *color, FaceNode face,
                  Point2DInt *vertices, int lineAlgorithm, ThreadPool *pool,
                  int numThreads)
{

  intNode *aux = face.face;

  switch (lineAlgorithm)
    {
    case 0:
      // printf("Dibujando con Naive paralelo...\n");
      while (aux->next)
        {
          parallelNaiveLineDrawing (image, color, vertices[aux->value - 1],
                                    vertices[aux->next->value - 1], numThreads,
                                    pool);
          aux = aux->next;
        }
      parallelNaiveLineDrawing (image, color, vertices[aux->value - 1],
                                vertices[face.face->value - 1], numThreads,
                                pool);
      break;
    }
}
