/* global_difficulty_friend_settings @ 0x82126260 (.rdata, 72 bytes) — friendly-unit remap of the
 * game_difficulty value selector. game_difficulty_get_team_value, for a unit friendly to team 1,
 * replaces value_type with global_difficulty_friend_settings[value_type]; 0xFFFF means "no friend
 * variant exists" and the caller falls back to the Normal difficulty row instead.
 * Initializer reconstructed from the binary (big-endian words, hi/lo int16 halves):
 *   +0x00 0x00040005 -> [0]=4  [1]=5     (the four enemy_* scales remap to their friend_* twins)
 *   +0x04 0x00060007 -> [2]=6  [3]=7
 *   +0x08..+0x40 0xFFFFFFFF x15 -> [4]..[33] = -1
 *   +0x44 0xFFFF0000 -> [34] = -1, then 2 bytes of alignment padding
 * Length: 35 == NUMBER_OF_GAME_DIFFICULTY_VALUES, the value_type index space; 35 * 2 == 70 bytes
 * plus the 2-byte pad that rounds the object up to the 72 bytes before the next .rdata symbol.
 *
 * DEVIATION: the object lives in .rdata (read-only), so the original declaration was almost
 * certainly const; it is spelled non-const here to match the corpus's only declaration of it
 * (src/game_difficulty_get_team_value.c).
 */
#include <stdint.h>
#include "../headers/game_difficulty_value.h"

int16_t global_difficulty_friend_settings[NUMBER_OF_GAME_DIFFICULTY_VALUES] =
{
    _game_difficulty_friend_damage_scale,    /* [_game_difficulty_enemy_damage_scale]   */
    _game_difficulty_friend_vitality_scale,  /* [_game_difficulty_enemy_vitality_scale] */
    _game_difficulty_friend_shield_scale,    /* [_game_difficulty_enemy_shield_scale]   */
    _game_difficulty_friend_recharge_scale,  /* [_game_difficulty_enemy_recharge_scale] */
    /* [4].._game_difficulty_unused4: no friendly variant */
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1,
};
