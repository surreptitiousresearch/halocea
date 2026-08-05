/* global_secondary_look_priorities @ 0x82128C68 — .rdata, 56 bytes = 28 x int16_t (exact:
 * NUMBER_OF_SECONDARY_LOOK_TYPES == 14 rows of 2). actor_look_secondary resolves an "auto"
 * (_secondary_look_priority_default) request with
 *   global_secondary_look_priorities[2 * type + (combat_status >= _actor_combat_status_certain)],
 * i.e. the table is a [secondary_look_type][actor-is-certain-of-a-target] pair table. It is kept
 * flat here because every corpus declaration is the flat `const int16_t []` the consumer indexes.
 * Raw big-endian halfword pairs -> resolved secondary_look_priority values (unaware, certain):
 *   [0]  0x0001 0x0001   [1]  0x0002 0x0002   [2]  0x0002 0x0002   [3]  0x0003 0x0003
 *   [4]  0x0005 0x0003   [5]  0x0004 0x0004   [6]  0x0005 0x0004   [7]  0x0005 0x0004
 *   [8]  0x0004 0x0003   [9]  0x0005 0x0005   [10] 0x0006 0x0003   [11] 0x0006 0x0003
 *   [12] 0x0007 0x0005   [13] 0x0007 0x0002
 */

#include <stdint.h>
#include "../headers/secondary_look_priority.h"

const int16_t global_secondary_look_priorities[28] =
{
    _secondary_look_priority_default,       _secondary_look_priority_default,       /* _secondary_look_none                 */
    _secondary_look_priority_idle_look,     _secondary_look_priority_idle_look,     /* _secondary_look_environmental_noise  */
    _secondary_look_priority_idle_look,     _secondary_look_priority_idle_look,     /* _secondary_look_started_moving_prop  */
    _secondary_look_priority_idle_aim,      _secondary_look_priority_idle_aim,      /* _secondary_look_weapon_impact        */
    _secondary_look_priority_turn_and_aim,  _secondary_look_priority_idle_aim,      /* _secondary_look_newly_acknowledged_prop */
    _secondary_look_priority_aim,           _secondary_look_priority_aim,           /* _secondary_look_bumped_prop          */
    _secondary_look_priority_turn_and_aim,  _secondary_look_priority_aim,           /* _secondary_look_detonation           */
    _secondary_look_priority_turn_and_aim,  _secondary_look_priority_aim,           /* _secondary_look_shooting_prop        */
    _secondary_look_priority_aim,           _secondary_look_priority_idle_aim,      /* _secondary_look_communicating_prop   */
    _secondary_look_priority_turn_and_aim,  _secondary_look_priority_turn_and_aim,  /* _secondary_look_communicated_direction */
    _secondary_look_priority_stop_and_aim,  _secondary_look_priority_idle_aim,      /* _secondary_look_combat_stimulus_prop */
    _secondary_look_priority_stop_and_aim,  _secondary_look_priority_idle_aim,      /* _secondary_look_damage               */
    _secondary_look_priority_override,      _secondary_look_priority_turn_and_aim,  /* _secondary_look_dangerous_object     */
    _secondary_look_priority_override,      _secondary_look_priority_idle_look,     /* _secondary_look_scripted             */
};
