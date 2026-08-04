/* actors_move_randomly @0x8371E8A0 — drive every active AI actor with a randomized "wander" control input for
 * this tick. Swarm (flood) actors have each of their member units controlled individually: a zeroed
 * unit_control_data is filled with a forward throttle and a facing vector rotated 30 degrees off the unit's own
 * facing, firing the primary trigger on every 5th tick (keyed by unit index + game time). Non-swarm actors are
 * delegated to actor_randomly_control_unit.
 *
 * DEVIATION: the DB has no prototype and Hex-Rays types the return as actor_datum*, but the returned value is
 * always the null loop terminator from actor_iterator_next, so the function is void. The per-member
 * unit_control_data is memset(0, 64) (disasm 0x8371E96C `stdu r9=0`, ctr=8); the decompiler rendered that
 * zero-fill as a qword store of a register whose high half was scratch for the member-index load.
 * actor_randomly_control_unit reads only r3 (disasm 0x8371E780 `mr r31,r3`; r4 is never read), so its DB
 * second parameter is phantom and it is called with a single argument. 0.86602539 = cos(30 deg),
 * 0.5 = sin(30 deg). Swarm member unit indices are swarm->unit_indices[i]. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/actor_iterator.h"
#include "headers/swarm_datum.h"
#include "headers/unit_control_data.h"
#include "headers/data_array.h"
#include "headers/real_vector3d.h"
#include "headers/unit_control_flags.h"
#include "headers/unit_desired_animation_state.h"
#include "headers/blam_data_globals.h"


extern void actor_iterator_new(actor_iterator *iterator, uint8_t active_only);
extern actor_datum *actor_iterator_next(actor_iterator *iterator);
extern void actor_randomly_control_unit(int unit_index);   /* DB 2nd param phantom (r4 unread) */
extern void unit_get_facing_vector(int unit_index, real_vector3d *facing_vector);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);
extern int game_time_get(void);
extern void unit_control(int unit_index, const unit_control_data *control_data, int completed_client_update_id);
extern void unit_set_actively_controlled(int unit_index, uint8_t actively_controlled);
extern void *memset(void *dst, int value, unsigned int size);

void actors_move_randomly(void)
{
    actor_iterator iterator;
    actor_iterator_new(&iterator, 1u);
    for ( actor_datum *actor = actor_iterator_next(&iterator); actor; actor = actor_iterator_next(&iterator) )
    {
        if ( !actor->meta.swarm )
        {
            actor_randomly_control_unit(actor->meta.unit_index);
            continue;
        }
        if ( actor->meta.swarm_cache_index == -1 )
            continue;

        swarm_datum *swarm = DATA_ARRAY_ELEMENT(swarm_data, swarm_datum, actor->meta.swarm_cache_index);
        for ( int member = 0; member < swarm->unit_count; member = (__int16)(member + 1) )
        {
            int unit_index = swarm->unit_indices[member];

            unit_control_data control;
            memset(&control, 0, sizeof(control));
            control.animation_state = _unit_animation_state_in_combat;
            control.control_flags = 0;
            control.aiming_speed = 1;
            control.weapon_index = -1;
            control.grenade_index = -1;
            control.zoom_level = -1;
            control.throttle = *global_forward3d;
            unit_get_facing_vector(unit_index, &control.facing_vector);
            rotate_vector_about_axis(&control.facing_vector, global_up3d, 0.5f, 0.86602539f);
            control.aiming_vector = control.facing_vector;
            control.looking_vector = control.facing_vector;

            int time = game_time_get();
            if ( time + unit_index == 5 * ((time + unit_index) / 5) )
            {
                control.primary_trigger = 1.0f;
                control.control_flags |= (1u << _unit_control_weapon_primary_trigger_bit);
            }
            unit_control(unit_index, &control, -1);
            unit_set_actively_controlled(unit_index, 0);
        }
    }
}
