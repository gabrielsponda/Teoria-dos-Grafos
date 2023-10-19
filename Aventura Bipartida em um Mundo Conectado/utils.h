/******************************************************************************
 *
 * Created by: Carla Negri Lintzmayer
 *             Maycon Sambinelli
 * To be used by our students
 *
 *****************************************************************************/

#ifndef __UTILS_H_
#define __UTILS_H_

#include <stdlib.h>
#include <time.h>

#define DEBUGING 1
#define DEBUG(fmt, ...)                                                        \
  do {                                                                         \
    if (DEBUGING)                                                              \
      fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__,        \
              __VA_ARGS__);                                                    \
  } while (0)

#define INIT_RAND                                                              \
  do {                                                                         \
    srand((long)time(NULL));                                                   \
  } while (0)

void *Malloc(size_t size);

double rand_double();

int rand_int(int a, int b); /* a to b, inclusive */

typedef struct {
  double x;
  double y;
} Point;

Point point(double, double);

#define SWAP(a, b)                                                             \
  do {                                                                         \
    typeof(a) t;                                                               \
    t = a;                                                                     \
    a = b;                                                                     \
    b = t;                                                                     \
  } while (0)

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#endif // __UTILS_H_