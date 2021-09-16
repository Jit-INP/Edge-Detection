#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <assert.h>

//#define FILTER_SIZ 5
#define MAX_FIL_NAM_SIZ 64

//typedef unsigned char pix;
//typedef char filter;
//typedef short pixprod;
typedef int mat;

typedef unsigned char bit;
typedef unsigned char gray;
typedef unsigned char color;


#define NULL_PTR_CHK(x) assert(x != NULL)
#define SAME_PTR_CHK(x, y) assert(x != y)
#define ABOVE_ZERO_CHK(x) assert(x > 0.0)

#define MAX_PIX_VAL ( (mat) ( pow( 2, 8 * sizeof(gray) ) - 1 ) )

#endif // UTILS_H
