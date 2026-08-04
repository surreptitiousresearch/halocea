#include <stdint.h>
/* ai_scripting_allegiance_remove @0x83770C50 — script command: remove the allegiance between two teams. Valid
 * team indices only. */

extern int game_allegiance_remove(int16_t team1_index, int16_t team2_index);

void ai_scripting_allegiance_remove(int16_t team1_index, int16_t team2_index)
{
    if ( team1_index != -1 && team2_index != -1 )
        game_allegiance_remove(team1_index, team2_index);
}
