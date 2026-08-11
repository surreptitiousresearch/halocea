/* actor_look_valid_aim_vector @0x837FACB8 — true if an attempted aiming vector points within the
 * allowed yaw deviation of a facing direction: normalize the attempted aiming vector's horizontal
 * (x,y) components and test whether its dot with the facing vector's (x,y) exceeds yaw_deviation.
 *
 * DEVIATION: the DB's applied prototype and the earlier reconstruction were mis-mapped. The disasm
 * (0x837FACB8..0x837FAD2C) proves only THREE args: r5 (attempted_aiming_vector) is the vector that gets
 * normalized (magnitude/inverse), r4 (facing_vector) is the vector dotted against the normalized r5, and
 * f1 (yaw_deviation) is the threshold — passed single-precision (caller lfs f1,0x12C(r30)), so it is a
 * float, not a double. There is NO r6 "target_direction" parameter; the prior 4-arg form was wrong.
 * The decompiler renders the float loads as COERCE_FLOAT("nit") artifacts; they are plain float loads. */

#include <stdint.h>
#include "headers/real_vector3d.h"
#include <math.h>

uint8_t actor_look_valid_aim_vector(float yaw_deviation, const real_vector3d *facing_vector,
                                    const real_vector3d *attempted_aiming_vector)
{
    float aim_x = attempted_aiming_vector->n[0];
    float aim_y = attempted_aiming_vector->n[1];
    float magnitude = sqrtf(aim_x * aim_x + aim_y * aim_y);
    if ( fabsf(magnitude) >= 0.000099999997f && magnitude > 0.0f )
    {
        float inverse = 1.0f / magnitude;
        return (facing_vector->n[0] * (inverse * aim_x)
              + facing_vector->n[1] * (inverse * aim_y)) > yaw_deviation;
    }
    return 0;
}
