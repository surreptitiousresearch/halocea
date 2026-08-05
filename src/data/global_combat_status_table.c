/* global_combat_status_table @ 0x821280D4 (.rdata, 24 bytes = 12 x int16_t) — the combat-status
 * FLOOR each actor_target_type imposes. actor_situation_combat_status_update reads
 * global_combat_status_table[actor->target.target_type] and maxes it against the scripted
 * alertness floor and the running suspicion level.
 * Initializer reconstructed from the binary (big-endian words, hi/lo int16 halves):
 *   +0x00 0x00000000 -> [ 0]=0  [ 1]=0
 *   +0x04 0x00000001 -> [ 2]=0  [ 3]=1
 *   +0x08 0x00020002 -> [ 4]=2  [ 5]=2
 *   +0x0C 0x00030004 -> [ 6]=3  [ 7]=4
 *   +0x10 0x00050005 -> [ 8]=5  [ 9]=5
 *   +0x14 0x00070007 -> [10]=7  [11]=7
 * 12 entries: the length the sole consumer declares and exactly number_of_actor_target_types.
 */
#include <stdint.h>
#include "../headers/actor_target_type.h"
#include "../headers/actor_combat_status.h"

const int16_t global_combat_status_table[number_of_actor_target_types] =
{
    _actor_combat_status_none,         /* [actor_target_none]                        */
    _actor_combat_status_none,         /* [actor_target_partial_enemy]               */
    _actor_combat_status_none,         /* [actor_target_dead_enemy]                  */
    _actor_combat_status_wary,         /* [actor_target_disregarded_orphan]          */
    _actor_combat_status_investigate,  /* [actor_target_inspected_orphan]            */
    _actor_combat_status_investigate,  /* [actor_target_uninspected_orphan]          */
    _actor_combat_status_definite,     /* [actor_target_definite_orphan]             */
    _actor_combat_status_certain,      /* [actor_target_acknowledged_enemy]          */
    _actor_combat_status_clear_los,    /* [actor_target_clear_line_of_sight_enemy]   */
    _actor_combat_status_clear_los,    /* [actor_target_potentially_dangerous_enemy] */
    _actor_combat_status_visible,      /* [actor_target_visible_enemy]               */
    _actor_combat_status_visible,      /* [actor_target_damaging_enemy]              */
};
