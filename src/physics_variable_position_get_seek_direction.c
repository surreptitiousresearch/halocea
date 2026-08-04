/* physics_variable_position_get_seek_direction @0x83810928 — DEVIATION: the decompiler fabricated a bogus
 * 5th `a5` parameter and mistyped `cyclical_position` as a `float*` (dereferencing it as
 * `*cyclical_position`/`cyclical_position[1]`). disasm_range(0x83810928,0x8381098C) shows only 4 arguments
 * (f1=position, r4=definition, r5=cyclical_position as a plain byte flag, f2=desired_position — matching
 * the DB's own 4-arg funcs.prototype exactly) and the two `lfs` reads at offsets 0/4 of r4 are
 * `definition->maximum_position`/`minimum_position` (physics_variable_position.h), not a separate array. */

#include <stdint.h>
#include "headers/physics_variable_position.h"

extern float __fabs(float x);

float physics_variable_position_get_seek_direction(float position, const physics_variable_position *definition, uint8_t cyclical_position, float desired_position)
{
    float delta = desired_position - position;

    if ( delta == 0.0f )
        return delta;

    if ( cyclical_position && __fabs(delta) > (definition->maximum_position - definition->minimum_position) * 0.5f )
        delta = -delta;

    return delta <= 0.0f ? -1.0f : 1.0f;
}
