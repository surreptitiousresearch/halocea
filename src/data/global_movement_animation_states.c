/* global_movement_animation_states @ 0x82114DB8 — .rdata, 5 x int16_t (10 bytes used; the symbol
 * slot runs 12 bytes to the next .rdata symbol, so the trailing zero halfword is alignment
 * padding). Maps actor_movement_type -> unit_desired_animation_state; actor_unit_control loads
 * control.animation_state from it and unit_control stores that into unit.animation.desired_state.
 * Raw big-endian halfwords -> resolved meaning:
 *   +0x00 0x0001 -> [actor_movement_type_alert]    _unit_animation_state_alert
 *   +0x02 0x0000 -> [actor_movement_type_asleep]   _unit_animation_state_asleep
 *   +0x04 0x0003 -> [actor_movement_type_combat]   _unit_animation_state_in_combat
 *   +0x06 0x0005 -> [actor_movement_type_panic]    _unit_animation_state_flee
 *   +0x08 0x0006 -> [actor_movement_type_flaming]  _unit_animation_state_flaming
 *   +0x0A 0x0000 -> alignment padding (number_of_actor_movement_types == 5)
 *
 * DEVIATION: src/actor_unit_control.c used to declare this `extern const unsigned char
 * global_movement_animation_states[]`. The element type is 16-bit, not 8-bit: actor_unit_control
 * @0x8371FD38 scales the index by two and loads a halfword (`lhz r4, 0x6DC(r31)` /
 * `slwi r9, r3, 1` / `lhzx r7, r9, r6`), and the DB applied type is `const __int16[5]`
 * (independently recorded in src/headers/unit_desired_animation_state.h). Defined here at the
 * evidenced width; the consumer decl now comes from headers/blam_data_globals.h (2026-08-06).
 */

#include <stdint.h>
#include "../headers/unit_desired_animation_state.h"

const int16_t global_movement_animation_states[5] =
{
    _unit_animation_state_alert,      /* actor_movement_type_alert   */
    _unit_animation_state_asleep,     /* actor_movement_type_asleep  */
    _unit_animation_state_in_combat,  /* actor_movement_type_combat  */
    _unit_animation_state_flee,       /* actor_movement_type_panic   */
    _unit_animation_state_flaming,    /* actor_movement_type_flaming */
};
