/* player_control_new_unit @0x836DE364 — resets a local player's control state when they take control of a
 * new unit. Clears the control block, then seeds the desired look angles from the unit's facing, and the
 * desired weapon/grenade/zoom selections from the unit's current state. Pitch is clamped to ~±85.5°.
 *
 * DEVIATION: the decompiler's trailing isnan/range checks on the resulting pitch are residual asserts with
 * no side effects; elided. */

#include <stdint.h>
#include "headers/player_control_globals.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/real_vector3d.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"
#include "headers/math_constants.h"

#include "headers/real_euler_angles2d.h"
extern void *memset(void *dst, int value, unsigned int size);
extern real_euler_angles2d *euler_angles2d_from_vector3d(real_euler_angles2d *angles, const real_vector3d *vector);

void player_control_new_unit(int16_t local_player_index, int unit_index)
{
    player_control *control = &player_control_globals->players[local_player_index];
    memset(control, 0, sizeof(player_control));
    control->unit_index = unit_index;
    control->desired_weapon_index = -1;
    control->desired_grenade_index = -1;
    control->desired_zoom_level = -1;
    control->pitch_maximum = 1.4922565f;
    control->use_autolevel = 0;
    control->pitch_minimum = -1.4922565f;
    control->target_object_index = -1;
    control->inhibited_button_bit_vector = 0;
    control->reset_button_when_released_bit_vector = 0;

    if (unit_index != -1)
    {
        unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
        euler_angles2d_from_vector3d(&control->desired_angles, &unit->unit.desired_facing_vector);
        if (control->desired_angles.n[0] < 0.0f)
            control->desired_angles.n[0] += TWO_PI;
        control->desired_weapon_index = (uint16_t)unit->unit.desired_weapon_index;
        control->desired_grenade_index = unit->unit.desired_grenade_index;
        control->desired_zoom_level = unit->unit.desired_zoom_level;
    }
}
