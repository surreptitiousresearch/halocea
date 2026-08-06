/* actor_randomly_control_unit @0x8371E768 — build a synthetic control packet that walks a unit
 * forward while facing a direction rotated ~60 degrees (sine=0.5, cosine=0.86602539) off its current
 * facing about the world up axis, and periodically fires (every 5th game tick, offset by actor_index).
 * Feeds the packet to unit_control() and marks the unit as no longer actively controlled.
 *
 * DEVIATION (2026-07-31): one param, not two. disasm (0x8371E768) reads only r3 (the actor index,
 * kept in r31 throughout); r4 is only ever written as scratch, never read — so the DB prototype's 2nd
 * `unit_index` param is a phantom. The decompiler's further phantom params were the zero-fill source for
 * control_data's unrolled memset. */

#include <stdint.h>
#include "headers/unit_control_data.h"
#include "headers/unit_control_flags.h"
#include "headers/unit_desired_animation_state.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"

extern void *memset(void *destination, int value, unsigned int size);
extern int game_time_get(void);
extern void unit_get_facing_vector(int unit_index, real_vector3d *facing_vector);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);
extern void unit_control(int unit_index, const unit_control_data *control_data, int completed_client_update_id);
extern void unit_set_actively_controlled(int unit_index, uint8_t actively_controlled);


void actor_randomly_control_unit(int actor_index)
{
    unit_control_data control_data;
    memset(&control_data, 0, sizeof(control_data));

    control_data.control_flags = 0;
    control_data.animation_state = _unit_animation_state_in_combat;
    control_data.aiming_speed = 1;
    control_data.weapon_index = -1;
    control_data.grenade_index = -1;
    control_data.zoom_level = -1;
    control_data.throttle = *global_forward3d;

    unit_get_facing_vector(actor_index, &control_data.facing_vector);
    rotate_vector_about_axis(&control_data.facing_vector, global_up3d, 0.5f, 0.86602539f);
    control_data.aiming_vector = control_data.facing_vector;
    control_data.looking_vector = control_data.facing_vector;

    if ( !((game_time_get() + actor_index) % 5) )
    {
        control_data.control_flags |= (1u << _unit_control_weapon_primary_trigger_bit);
        control_data.primary_trigger = 1.0f;
    }

    unit_control(actor_index, &control_data, -1);
    unit_set_actively_controlled(actor_index, 0);
}
