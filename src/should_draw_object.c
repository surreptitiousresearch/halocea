/* should_draw_object @0x837BCB80 — whether an object should get a motion-sensor blip: must be a valid
 * unit-type object the game engine allows on the radar; always drawn if it is firing its primary weapon
 * (control_flags bit 0x800 = _unit_control_weapon_primary_trigger_bit) or has a nonzero, non-released
 * unit.grenade_throw_state; otherwise only drawn while moving fast enough (or in multiplayer /
 * debug-all-units), and only while not actively camouflaged (or in multiplayer, per bit 0x10 =
 * _unit_active_camouflaged_bit of unit.flags). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/hud_globals.h"
#include "headers/real_vector3d.h"
#include "headers/unit_flags.h"
#include "headers/unit_control_flags.h"
#include "headers/unit_grenade_throw_state.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t game_engine_draw_object_in_motion_sensor(int unit_index);
extern uint8_t game_engine_running(void);
extern void object_get_velocities(int object_index, real_vector3d *translational_velocity, real_vector3d *angular_velocity);

uint8_t should_draw_object(int object_index)
{
    if (!object_try_and_get_and_verify_type(object_index, object_mask_unit) || !game_engine_draw_object_in_motion_sensor(object_index))
        return 0;

    unit_datum *object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);

    if ((object->unit.control_flags & (1u << _unit_control_weapon_primary_trigger_bit)) || (object->unit.grenade_throw_state && object->unit.grenade_throw_state != _grenade_throw_state_released))
        return 1;

    real_vector3d velocity;
    object_get_velocities(object_index, &velocity, 0);

    uint8_t active_or_multiplayer = game_engine_running() || !(object->unit.flags & (1u << _unit_active_camouflaged_bit));

    float speed_squared = velocity.n[2] * velocity.n[2] + velocity.n[1] * velocity.n[1] + velocity.n[0] * velocity.n[0];
    uint8_t fast_enough_or_debug =
        (speed_squared >= hud_globals->defaults.motion_sensor_velocity_sensitivity) || debug_motion_sensor_draw_all_units;

    if (!active_or_multiplayer)
        return 0;
    return fast_enough_or_debug ? 1 : 0;
}
