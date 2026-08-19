/* player_profile_get_highest_completed_solo_level @0x83765078 — scan a player profile's per-level completion
 * flags for the ten single-player campaign maps and report the highest-indexed level that has been completed and
 * the highest difficulty it was completed on. Each single_player_map_flags[i] byte holds one bit per difficulty
 * (bit 1 = easy, 2 = normal, 4 = heroic, 8 = legendary). Levels are scanned in ascending order and the outputs
 * are overwritten unconditionally on every completed level, so the final values reflect the last (highest) level
 * with any bit set, at that level's highest difficulty. Defaults are level -1, difficulty 1 (normal) when nothing
 * is completed.
 *
 * DEVIATION: the decompiler emitted the scan as a 5-wide unroll (five running pointers advanced by 5 per
 * iteration, ctr = 2 iterations = 10 levels). Restored the equivalent single loop over all ten levels; the
 * ascending scan order and overwrite semantics are identical. */

#include <stdint.h>
#include "headers/player_profile.h"
#include "headers/game_difficulty.h"

void player_profile_get_highest_completed_solo_level(player_profile *profile, int16_t *level, int16_t *difficulty)
{
    *level = -1;
    *difficulty = game_difficulty_level_normal;

    for ( int i = 0; i < 10; ++i )
    {
        char flags = profile->single_player_map_flags[i];
        if ( !flags )
            continue;

        if ( (flags & (1 << game_difficulty_level_impossible)) != 0 )
        {
            *level = i;
            *difficulty = game_difficulty_level_impossible;
        }
        else if ( (flags & (1 << game_difficulty_level_hard)) != 0 )
        {
            *level = i;
            *difficulty = game_difficulty_level_hard;
        }
        else if ( (flags & (1 << game_difficulty_level_normal)) != 0 )
        {
            *level = i;
            *difficulty = game_difficulty_level_normal;
        }
        else if ( (flags & (1 << game_difficulty_level_easy)) != 0 )
        {
            *level = i;
            *difficulty = game_difficulty_level_easy;
        }
    }
}
