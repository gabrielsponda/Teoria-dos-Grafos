/******************************************************************************
 *
 * Created by: Carla Negri Lintzmayer
 *             Maycon Sambinelli
 * To be used by our students
 *
 *****************************************************************************/


#include "utils.h"

void* Malloc(size_t size) {
    void *ptr = malloc(size);
    if (!ptr)
        abort();
    return ptr;
}

double rand_double() {
   return rand() / (double) RAND_MAX;
}

int rand_int(int a, int b) {
   return (int)((b - a + 1) * rand_double()) + a;
}

Point point(double x, double y) {
    Point p;
    p.x = x;
    p.y = y;
    return p;
}