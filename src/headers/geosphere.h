#ifndef GEOSPHERE_H
#define GEOSPHERE_H

#include <stdint.h>
#include "real_point3d.h"

typedef struct geosphere
{
    int16_t        segment_count;
    int16_t        _pad;
    real_point3d  *vertices;
    int16_t       *triangle_strip_vertex_indices;
    int16_t        vertex_count;
    int16_t        triangle_count;
    int16_t        triangle_strip_count;
    int16_t        _pad1;
} geosphere; /* 20 bytes */

#endif /* GEOSPHERE_H */
