/* physics_variable_speed_update_seek @0x838107D4 — step a variable-speed value toward a target: accelerate
 * or decelerate *speed by magnitude (via physics_variable_speed_update) in whichever direction closes the
 * gap, clamping to desired_speed once reached. Returns 1 when the target is reached (this step or already),
 * 0 while still seeking.
 * DEVIATION: the decompiler's uninitialized v5/v6/v7/v9/v10 are re-reads of *speed / desired_speed and the
 * constant 0 return; recovered from disassembly. Float params (soft-float doubles in the decompiler). */

#include "headers/physics_variable_speed.h"

extern void physics_variable_speed_update(float *speed, const physics_variable_speed *definition, float magnitude);

int physics_variable_speed_update_seek(float *speed, const physics_variable_speed *definition, float desired_speed, float magnitude)
{
    if ( *speed > desired_speed )
    {
        physics_variable_speed_update(speed, definition, -magnitude);
        if ( *speed > desired_speed )
            return 0;
        *speed = desired_speed;
        return 1;
    }
    if ( *speed >= desired_speed )   /* already at target */
        return 1;
    physics_variable_speed_update(speed, definition, magnitude);
    if ( *speed < desired_speed )
        return 0;
    *speed = desired_speed;
    return 1;
}
