/* limit3d @0x836C5690 — clamp a 3D vector's magnitude to length. If the squared magnitude exceeds
 * length*length, the vector is rescaled to exactly length and 1 is returned; otherwise it is left
 * unchanged and 0 is returned. */

#include "headers/real_vector3d.h"

extern float __fsqrts(float x);

int limit3d(real_vector3d *vector, float length)
{
    float magnitude_squared = vector->n[2] * vector->n[2]
                            + (vector->n[0] * vector->n[0] + vector->n[1] * vector->n[1]);
    if ( magnitude_squared <= length * length )
        return 0;

    float scale = length / __fsqrts(magnitude_squared);
    vector->n[0] = vector->n[0] * scale;
    vector->n[1] = vector->n[1] * scale;
    vector->n[2] = vector->n[2] * scale;
    return 1;
}
