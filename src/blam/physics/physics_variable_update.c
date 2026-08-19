/* physics_variable_update @0x83810878 — advance a seek-toward-target scalar by one physics tick: update
 * `speed` via physics_variable_speed_update, add it to `position`, then clamp/wrap `position` into
 * [minimum_position, maximum_position] exactly like physics_variable_position_update.
 *
 * DEVIATION: the funcs-table address (0x83810894) lands mid-body, at the `bl physics_variable_speed_update`
 * instruction rather than the true prologue at 0x83810878 (`mflr`/`stwu` frame setup) — the decompiler's
 * output for the recorded address showed `position`/`speed`/`definition` as uninitialized locals (v5/v7/v8)
 * because it had lost the parameter-to-register bindings that are only established in the true prologue.
 * Restored by disassembling from the true entry, which shows r3=position saved to r10, r4=speed saved to
 * r9, and r5=definition passed straight through (its offset-8 sub-view handed to physics_variable_speed_update). */

#include <stdint.h>
#include "headers/physics_variable.h"
#include "headers/physics_variable_speed.h"

extern void physics_variable_speed_update(float *speed, const physics_variable_speed *definition, float magnitude);

void physics_variable_update(float *position, float *speed, const physics_variable *definition, uint8_t cyclical_position, float magnitude)
{
    physics_variable_speed_update(speed, (const physics_variable_speed *)&definition->maximum_speed, magnitude);

    float new_position = *position + *speed;
    *position = new_position;

    if ( new_position < definition->minimum_position )
    {
        if ( cyclical_position )
        {
            *position = (definition->maximum_position - definition->minimum_position) + new_position;
            return;
        }
        *position = definition->minimum_position;
        return;
    }

    if ( new_position <= definition->maximum_position )
        return;

    if ( cyclical_position )
        *position = new_position - (definition->maximum_position - definition->minimum_position);
    else
        *position = definition->maximum_position;
}
