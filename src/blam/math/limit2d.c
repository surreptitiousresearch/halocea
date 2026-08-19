/* limit2d @0x836DE1E8 — clamp a 2D vector's magnitude to length. If the squared magnitude exceeds
 * length*length, the vector is rescaled to exactly length and 1 is returned; otherwise it is left
 * unchanged and 0 is returned. */

#include "headers/real_vector2d.h"

extern float __fsqrts(float x);

int limit2d(real_vector2d *vector, float length)
{
    float magnitude_squared = vector->n[0] * vector->n[0] + vector->n[1] * vector->n[1];
    if ( magnitude_squared <= length * length )
        return 0;

    float scale = length / __fsqrts(magnitude_squared);
    vector->n[0] = vector->n[0] * scale;
    vector->n[1] = vector->n[1] * scale;
    return 1;
}
