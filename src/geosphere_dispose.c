#include "headers/geosphere.h"

extern void dlFree(void *ptr);

void geosphere_dispose(geosphere *sphere)
{
    dlFree(sphere->vertices);
    dlFree(sphere->triangle_strip_vertex_indices);
    dlFree(sphere);
}
