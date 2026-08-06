/* actor_freeze_unit @ 0x8371E470 — drive a frozen actor's unit to a neutral "hold position" pose for one
 * tick: hand unit_control a zeroed control packet (idle animation, no weapon/grenade/zoom, zero throttle)
 * whose facing/aiming/looking vectors are read back from the unit's own current orientation, then mark the
 * unit as no longer actively controlled.
 *
 * Note: the DB prototype is (actor_index, unit_index) but the body only uses the first argument (as the
 * unit/object index for unit_get_*_vector / unit_control); the second is unused here, which is why the
 * AI-freeze caller passes only one value. The decompiler also invented extra params from the memset's 64-bit
 * zero source — dropped. */

#include <stdint.h>
#include <string.h>
#include "headers/unit_control_data.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"
#include "headers/unit_desired_animation_state.h"


extern void unit_get_facing_vector(int unit_index, real_vector3d *facing_vector);
extern void unit_get_aiming_vector(int unit_index, real_vector3d *aiming_vector);
extern void unit_get_looking_vector(int unit_index, real_vector3d *looking_vector);
extern void unit_control(int unit_index, const unit_control_data *control_data, int completed_client_update_id);
extern void unit_set_actively_controlled(int unit_index, uint8_t actively_controlled);

void actor_freeze_unit(int actor_index)
{
    unit_control_data control;

    memset(&control, 0, sizeof(control));
    control.animation_state = _unit_animation_state_alert;
    control.control_flags = 0;
    control.aiming_speed = 1;
    control.weapon_index = -1;
    control.grenade_index = -1;
    control.zoom_level = -1;
    control.throttle = *global_zero_vector3d;
    unit_get_facing_vector(actor_index, &control.facing_vector);
    unit_get_aiming_vector(actor_index, &control.aiming_vector);
    unit_get_looking_vector(actor_index, &control.looking_vector);
    unit_control(actor_index, &control, -1);
    unit_set_actively_controlled(actor_index, 0);
}
