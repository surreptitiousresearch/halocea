/* get_ball_definition_index @0x837482D0 — reads dword 22 (byte offset 0x58) of the
 * multiplayer_information tag block's single element (confirmed via decompiler). */

#include "headers/game_globals_tag.h"
#include "headers/game_globals_multiplayer_information.h"

int get_ball_definition_index(void)
{
    return ((game_globals_multiplayer_information *)global_game_globals->multiplayer_information.address)->ball.index;
}
