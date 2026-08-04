/* get_flag_definition_index @0x837482B8 — reads dword 3 (byte offset 0xC) of the
 * multiplayer_information tag block's single element (confirmed via decompiler). */

#include "headers/game_globals_tag.h"
#include "headers/game_globals_multiplayer_information.h"

int get_flag_definition_index(void)
{
    return ((game_globals_multiplayer_information *)global_game_globals->multiplayer_information.address)->flag.index;
}
