#ifndef FACE_H
#define FACE_H

struct intNode
{
  int value;
  struct intNode *next;
};
typedef struct intNode intNode;

struct FaceNode
{
  intNode *face;
  struct FaceNode *next;
};
typedef struct FaceNode FaceNode;

#endif