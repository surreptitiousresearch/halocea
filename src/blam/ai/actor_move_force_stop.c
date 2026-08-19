/* actor_move_force_stop @0x837C8A48 — force an actor to stop moving: clear its move-animation impulse
 * (byte @ actor +1284), zero its desired velocity (vector @ actor +1760), and cancel any unit-control
 * animation impulse. Fails (returns 0) if a movement animation is busy or an action denies the
 * transition. Actor records are stride 1828. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"

extern uint8_t actor_move_animation_busy(int actor_index);
extern uint8_t actor_action_deny_transition(int actor_index);
extern void actor_unit_control_stop_animation_impulse(int actor_index);

uint8_t actor_move_force_stop(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( actor_move_animation_busy(actor_index) || actor_action_deny_transition(actor_index) )
        return 0;
    actor->control.moving = 0;
    actor->output.throttle = *global_zero_vector3d;  /* recovered: *(real_vector3d *)(actor+1760) -> output.throttle */
    actor_unit_control_stop_animation_impulse(actor_index);
    return 1;
}
