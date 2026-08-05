/* global_post_combat_translation_table @ 0x82114788 (.rdata, 8 bytes = 4 x int16_t) — maps the
 * post-combat behaviour bucket encounter_post_combat picked to the order it writes into
 * actor->external_orders.postcombat_type.
 * Initializer reconstructed from the binary (big-endian words):
 *   +0x00 0x00090007 -> [0] = 9 -> actor_postcombat_shoot_corpse
 *                       [1] = 7 -> actor_postcombat_check_enemy
 *   +0x04 0x0008000A -> [2] = 8 -> actor_postcombat_check_friend
 *                       [3] = 10 -> actor_postcombat_celebrate
 * Length pinned by the index space: encounter_post_combat indexes it with selected_behavior[i],
 * a post_combat_behavior_type (0..3), and 8 bytes / 2 == 4 entries exactly.
 */
#include <stdint.h>
#include "../headers/actor_postcombat_type.h"
#include "../headers/post_combat_behavior_type.h"

const int16_t global_post_combat_translation_table[NUMBER_OF_POST_COMBAT_BEHAVIOR_TYPES] =
{
    actor_postcombat_shoot_corpse,  /* [_post_combat_behavior_shoot_target] */
    actor_postcombat_check_enemy,   /* [_post_combat_behavior_check_enemy]  */
    actor_postcombat_check_friend,  /* [_post_combat_behavior_check_friend] */
    actor_postcombat_celebrate,     /* [_post_combat_behavior_celebrate]    */
};
