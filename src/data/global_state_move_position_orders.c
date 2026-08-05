/* global_state_move_position_orders @ 0x82128A10 — .rdata, 24 bytes = 12 x int16_t (exact:
 * number_of_actor_default_states == 12). Maps an actor_default_state to the move_position_order
 * that actor_action_set_default_state hands to action_alert_setup for the moving/alert states.
 * Raw big-endian halfwords -> resolved meaning:
 *   [0]  0x0000 [actor_default_state_none]                      _move_position_order_none
 *   [1]  0x0000 [actor_default_state_asleep]                    _move_position_order_none
 *   [2]  0x0000 [actor_default_state_alert]                     _move_position_order_none
 *   [3]  0x0001 [actor_default_state_moving_repeat_position]    _move_position_order_repeat
 *   [4]  0x0002 [actor_default_state_moving_loop]               _move_position_order_loop
 *   [5]  0x0003 [actor_default_state_moving_loop_back_and_forth] _move_position_order_loop_back_and_forth
 *   [6]  0x0004 [actor_default_state_moving_loop_randomly]      _move_position_order_loop_randomly
 *   [7]  0x0005 [actor_default_state_moving_randomly]           _move_position_order_random
 *   [8]  0x0000 [actor_default_state_guarding]                  _move_position_order_none
 *   [9]  0x0000 [actor_default_state_guarding_at_guard_point]   _move_position_order_none
 *   [10] 0x0000 [actor_default_state_searching]                 _move_position_order_none
 *   [11] 0x0000 [actor_default_state_fleeing]                   _move_position_order_none
 *
 * The object is in .rdata (read-only in the original); the corpus declaration in
 * src/actor_action_set_default_state.c is non-const, and the definition matches it so every
 * declaration of the symbol agrees. Adding const is left to the declaration-normalization pass.
 */

#include <stdint.h>
#include "../headers/move_position_order.h"

int16_t global_state_move_position_orders[12] =
{
    _move_position_order_none,                  /* actor_default_state_none                       */
    _move_position_order_none,                  /* actor_default_state_asleep                     */
    _move_position_order_none,                  /* actor_default_state_alert                      */
    _move_position_order_repeat,                /* actor_default_state_moving_repeat_position     */
    _move_position_order_loop,                  /* actor_default_state_moving_loop                */
    _move_position_order_loop_back_and_forth,   /* actor_default_state_moving_loop_back_and_forth */
    _move_position_order_loop_randomly,         /* actor_default_state_moving_loop_randomly       */
    _move_position_order_random,                /* actor_default_state_moving_randomly            */
    _move_position_order_none,                  /* actor_default_state_guarding                   */
    _move_position_order_none,                  /* actor_default_state_guarding_at_guard_point    */
    _move_position_order_none,                  /* actor_default_state_searching                  */
    _move_position_order_none,                  /* actor_default_state_fleeing                    */
};
