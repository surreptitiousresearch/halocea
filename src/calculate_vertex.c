#include <math.h>
#include <stdint.h>
#include "headers/geosphere.h"
#include "headers/real_point3d.h"

void calculate_vertex(geosphere *sphere, int16_t new_vertex, int16_t parent1, int16_t parent2,
                      int16_t denominator, int16_t numerator)
{
    real_point3d *vertices = sphere->vertices;
    float        *p1  = vertices[parent1].n;
    float        *p2  = vertices[parent2].n;
    float        *out = vertices[new_vertex].n;
    float         t   = (float)numerator / (float)denominator;
    float         x   = p1[0] * (1.0f - t) + p2[0] * t;
    float         y   = p1[1] * (1.0f - t) + p2[1] * t;
    float         z   = p1[2] * (1.0f - t) + p2[2] * t;
    float         length;

    out[0] = x;
    out[1] = y;
    out[2] = z;

    length = sqrtf(x * x + y * y + z * z);
    if (fabsf(length) >= 0.000099999997f)
    {
        float inv = 1.0f / length;
        out[0] = x * inv;
        out[1] = y * inv;
        out[2] = z * inv;
    }
}
