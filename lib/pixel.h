#include <stdint.h>

#ifndef _PIXEL_
#define _PIXEL_

struct pixel
{
  const uint8_t r, g, b;
};

typedef struct pixel pixel;

#endif
