/* actor_unit_control @ 0x8371FD38 — translate the actor's computed control output block (the 1744..1824
 * region populated through actor_update's control chain) into a unit_control_data packet and drive the
 * actor's unit with it. Movement animation state is mapped through global_movement_animation_states. Control
 * is only applied when the unit is AI-driven (its object field +536 == -1) or player input is disabled: in
 * that case it re-activates a previously frozen unit (meta.frozen at actor+7), issues unit_control, fires any
 * pending animation impulse, and applies persistent control if a timer is queued.
 *
 * The unit's object data pointer comes from the object-header datum (int stride 3, data pointer at +2),
 * indexed by meta.unit_index (actor+24). Control-output reads use named actor_datum members. */

#include <stdint.h>
#include "headers/unit_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/unit_control_data.h"
#include "headers/real_vector2d.h"
#include "headers/blam_data_globals.h"

#include "headers/real_vector2d.h"
/* DEVIATION: was declared `const unsigned char global_movement_animation_states[]` here; 0x8371FD84 scales the index by 2 and loads a halfword (const int16_t[5]). */

extern uint8_t player_input_enabled(void);
extern void unit_set_actively_controlled(int unit_index, uint8_t actively_controlled);
extern void unit_control(int unit_index, const unit_control_data *control_data, int completed_client_update_id);
extern int unit_start_animation_impulse(int unit_index, int16_t animation_impulse, real_vector2d *alignment_vector);
extern void unit_persistent_control(int unit_index, int persistent_control_timer, unsigned int persistent_control_flags);

void actor_unit_control(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int   unit_index = actor->meta.unit_index;
    unit_datum *unit_object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    unit_control_data control;
    control.animation_state = global_movement_animation_states[actor->output.movement_type];
    control.control_flags   = actor->output.control_flags;
    control.primary_trigger = actor->output.analog_primary_trigger;
    control.throttle.n[0]   = actor->output.throttle.n[0];
    control.throttle.n[1]   = actor->output.throttle.n[1];
    control.throttle.n[2]   = actor->output.throttle.n[2];
    control.aiming_speed    = actor->output.aiming_speed;
    control.facing_vector.n[0] = actor->output.facing_vector.n[0];
    control.facing_vector.n[1] = actor->output.facing_vector.n[1];
    control.facing_vector.n[2] = actor->output.facing_vector.n[2];
    control.aiming_vector.n[0] = actor->output.aiming_vector.n[0];
    control.aiming_vector.n[1] = actor->output.aiming_vector.n[1];
    control.aiming_vector.n[2] = actor->output.aiming_vector.n[2];
    control.weapon_index  = -1;
    control.grenade_index = -1;
    control.zoom_level    = -1;
    control.looking_vector.n[0] = actor->output.looking_vector.n[0];
    control.looking_vector.n[1] = actor->output.looking_vector.n[1];
    control.looking_vector.n[2] = actor->output.looking_vector.n[2];

    if ( unit_object->unit.player_index == -1 || !player_input_enabled() )
    {
        if ( actor->meta.frozen )                                /* meta.frozen */
        {
            unit_set_actively_controlled(unit_index, 1u);
            actor->meta.frozen = 0;
        }
        unit_control(unit_index, &control, -1);

        uint16_t animation_impulse = actor->output.animation.impulse;
        if ( animation_impulse != 0xFFFF )
            unit_start_animation_impulse(unit_index, animation_impulse, &actor->output.animation.alignment_vector);

        int16_t persistent_control_timer = actor->output.persistent_control_ticks;
        if ( persistent_control_timer > 0 )
            unit_persistent_control(unit_index, persistent_control_timer, actor->output.persistent_control_flags);
    }
}
