/* game_difficulty_get_team_value @0x837B6800 — like game_difficulty_get_value but team-aware. Picks the
 * difficulty row: normally the current difficulty level, but in game-engine (multiplayer) it forces level
 * 1 (Normal), and for friendly (non-enemy team 1) units it remaps to the friend-difficulty settings.
 * Indexes global_game_globals->difficulty_information.address[4*value_type + clamp(level,0,3)], 1.0 fallback.
 *
 * Deviation: the decompiler's `*((float *)&v6 + 1)` is the high word of the double register holding the
 * float result — it just returns the float value. */

#include <stdint.h>
#include "headers/game_globals_tag.h"  /* declares game_globals_tag and global_game_globals */
#include "headers/game_globals_difficulty_information.h"
#include "headers/game_difficulty.h"

extern int16_t game_difficulty_level_get(void);
extern uint8_t game_engine_running(void);
extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);
extern int16_t global_difficulty_friend_settings[];

extern float game_difficulty_get_value_by_difficulty(int16_t value_type, int16_t difficulty);
float game_difficulty_get_team_value(int16_t value_type, int16_t team_index)
{
    int16_t difficulty_level = game_difficulty_level_get();
    if ( game_engine_running() )
    {
        difficulty_level = game_difficulty_level_normal;
    }
    else if ( !game_team_is_enemy(1, team_index) )  /* friendly to team 1 */
    {
        if ( (uint16_t)global_difficulty_friend_settings[value_type] == 0xFFFF )
            difficulty_level = game_difficulty_level_normal;
        else
            value_type = global_difficulty_friend_settings[value_type];
    }

    /* DEVIATION: inlined body of game_difficulty_get_value_by_difficulty@0x837B66F8 (zero-xref donor); donor's (value_type, difficulty) params fold to this host's already-remapped locals (team/engine-adjusted value_type, difficulty_level) — collapsed to a direct call */
    return game_difficulty_get_value_by_difficulty(value_type, difficulty_level);
}
