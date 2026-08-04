/* ai_get_major_upgrade_chance @0x836E7738 — translate a squad's major-upgrade type into either a fixed
 * force/deny decision or a per-difficulty random chance.
 *
 *   type 0          -> force_major = true,  not random
 *   type 3          -> force_major = false, not random
 *   type 1,2,4(+)   -> random, chance = game_difficulty_get_value(table index) */

#include <stdint.h>
#include "headers/game_difficulty_value.h"
#include "headers/squad_major_upgrade_type.h"

extern float game_difficulty_get_value(int16_t value_type);

void ai_get_major_upgrade_chance(int16_t upgrade_type, uint8_t *force_major, uint8_t *is_random,
                                 float *random_chance)
{
    __int16 difficulty_value_index;

    if ( upgrade_type < _squad_major_upgrade_few || upgrade_type > _squad_major_upgrade_all )
    {
        difficulty_value_index = _game_difficulty_major_normal_placement;
        goto random_chance_from_difficulty;
    }
    if ( upgrade_type == _squad_major_upgrade_few )
    {
        difficulty_value_index = _game_difficulty_major_few_placement;
random_chance_from_difficulty:
        *is_random = 1;
        *random_chance = game_difficulty_get_value(difficulty_value_index);
        return;
    }
    if ( upgrade_type == _squad_major_upgrade_many )
    {
        difficulty_value_index = _game_difficulty_major_many_placement;
        goto random_chance_from_difficulty;
    }
    *is_random = 0;
    *force_major = upgrade_type != _squad_major_upgrade_none;
}
