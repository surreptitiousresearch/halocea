/* global_default_states @ 0x82128A28 — .rdata, 24 bytes = 12 x uint16_t (exact:
 * number_of_actor_default_states == 12). actor_action_get_default_state maps an actor's
 * initial-state enum through this table to the actor_default_state it falls back to.
 * Raw big-endian halfwords -> resolved meaning:
 *   [0]  0x0000 [none]                      -> actor_default_state_none
 *   [1]  0x0002 [asleep]                    -> actor_default_state_alert
 *   [2]  0x0002 [alert]                     -> actor_default_state_alert
 *   [3]  0x0003 [moving_repeat_position]    -> actor_default_state_moving_repeat_position
 *   [4]  0x0004 [moving_loop]               -> actor_default_state_moving_loop
 *   [5]  0x0005 [moving_loop_back_and_forth]-> actor_default_state_moving_loop_back_and_forth
 *   [6]  0x0006 [moving_loop_randomly]      -> actor_default_state_moving_loop_randomly
 *   [7]  0x0007 [moving_randomly]           -> actor_default_state_moving_randomly
 *   [8]  0x0008 [guarding]                  -> actor_default_state_guarding
 *   [9]  0x0009 [guarding_at_guard_point]   -> actor_default_state_guarding_at_guard_point
 *   [10] 0x0009 [searching]                 -> actor_default_state_guarding_at_guard_point
 *   [11] 0x0008 [fleeing]                   -> actor_default_state_guarding
 */

#include <stdint.h>
#include "../headers/actor_default_state.h"

const uint16_t global_default_states[12] =
{
    actor_default_state_none,                       /* none                       */
    actor_default_state_alert,                      /* asleep                     */
    actor_default_state_alert,                      /* alert                      */
    actor_default_state_moving_repeat_position,     /* moving_repeat_position     */
    actor_default_state_moving_loop,                /* moving_loop                */
    actor_default_state_moving_loop_back_and_forth, /* moving_loop_back_and_forth */
    actor_default_state_moving_loop_randomly,       /* moving_loop_randomly       */
    actor_default_state_moving_randomly,            /* moving_randomly            */
    actor_default_state_guarding,                   /* guarding                   */
    actor_default_state_guarding_at_guard_point,    /* guarding_at_guard_point    */
    actor_default_state_guarding_at_guard_point,    /* searching                  */
    actor_default_state_guarding,                   /* fleeing                    */
};
