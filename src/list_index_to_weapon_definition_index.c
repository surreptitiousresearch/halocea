/* list_index_to_weapon_definition_index @0x837485A8 — resolves a weapon_list slot index to its tag
 * definition index, or -1 for an invalid slot. */

#include "headers/game_globals_tag.h"
#include "headers/tag_reference.h"
#include "headers/blam_data_globals.h"


int list_index_to_weapon_definition_index(int list_index)
{
    if (list_index == -1)
        return -1;
    return ((tag_reference *)global_game_globals->weapon_list.address)[list_index].index;
}
