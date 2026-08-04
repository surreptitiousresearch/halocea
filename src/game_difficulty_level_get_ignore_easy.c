/* game_difficulty_level_get_ignore_easy @0x836882C8 — return the current difficulty, but treat Easy (0) as
 * Normal (1) so scripts that branch on difficulty never see the Easy tier. */

/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include "headers/blam_data_globals.h"
#include "headers/game_globals.h"
#include "headers/game_difficulty.h"


int game_difficulty_level_get_ignore_easy(void)
{
    int difficulty = game_globals->options.difficulty;
    if ( difficulty <= game_difficulty_level_normal )
        difficulty = game_difficulty_level_normal;
    return (__int16)difficulty;
}
