/* global_secondary_look_times @ 0x82128CA0 (.rdata, 56 bytes)
 * DB applied_types: const float global_secondary_look_times[14];
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   +0x0000 = 0x00000000
 *   +0x0004 = 0x3FA66666
 *   +0x0008 = 0x3F666666
 *   +0x000C = 0x3F666666
 *   +0x0010 = 0x3F666666
 *   +0x0014 = 0x3F333333
 *   +0x0018 = 0x3F666666
 *   +0x001C = 0x3F666666
 *   +0x0020 = 0x3F99999A
 *   +0x0024 = 0x40000000
 *   +0x0028 = 0x40000000
 *   +0x002C = 0x40200000
 *   +0x0030 = 0x3FC00000
 *   +0x0034 = 0x447A0000
 * .rdata, 56 bytes = 14 x float (exact:
 * NUMBER_OF_SECONDARY_LOOK_TYPES == 14). actor_look_secondary indexes it by secondary_look_type
 * to get the base duration, in seconds, of the glance that type produces (doubled when the actor
 * is not yet in combat, then scaled by the character's event-look modifier range).
 * Raw big-endian words -> IEEE-754 float:
 *   [0]  0x00000000 0.0     [1]  0x3FA66666 1.3     [2]  0x3F666666 0.9   [3]  0x3F666666 0.9
 *   [4]  0x3F666666 0.9     [5]  0x3F333333 0.7     [6]  0x3F666666 0.9   [7]  0x3F666666 0.9
 *   [8]  0x3F99999A 1.2     [9]  0x40000000 2.0     [10] 0x40000000 2.0   [11] 0x40200000 2.5
 *   [12] 0x3FC00000 1.5     [13] 0x447A0000 1000.0
 * /
 */
#include "../headers/secondary_look_type.h"

const float global_secondary_look_times[14] =
{
    0.0f,       /* _secondary_look_none                   */
    1.3f,       /* _secondary_look_environmental_noise    */
    0.9f,       /* _secondary_look_started_moving_prop    */
    0.9f,       /* _secondary_look_weapon_impact          */
    0.9f,       /* _secondary_look_newly_acknowledged_prop */
    0.7f,       /* _secondary_look_bumped_prop            */
    0.9f,       /* _secondary_look_detonation             */
    0.9f,       /* _secondary_look_shooting_prop          */
    1.2f,       /* _secondary_look_communicating_prop     */
    2.0f,       /* _secondary_look_communicated_direction */
    2.0f,       /* _secondary_look_combat_stimulus_prop   */
    2.5f,       /* _secondary_look_damage                 */
    1.5f,       /* _secondary_look_dangerous_object       */
    1000.0f,    /* _secondary_look_scripted (effectively "until cancelled") */
};
