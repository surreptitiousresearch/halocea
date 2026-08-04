/* weapon_definition_index_to_list_index @0x83748548 — find a weapon definition's slot in the game
 * globals weapon_list. Linear scan of the weapon_list entries (each a tag_reference to a weapon tag)
 * comparing the resolved definition index; returns the list index, or -1 if not present. */

#include "headers/game_globals_tag.h"
#include "headers/tag_reference.h"

int weapon_definition_index_to_list_index(int weapon_definition_index)
{
    int count = global_game_globals->weapon_list.count;
    if ( count <= 0 )
        return -1;

    const tag_reference *weapons = (const tag_reference *)global_game_globals->weapon_list.address;
    int list_index = 0;
    while ( weapon_definition_index != weapons[list_index].index )
    {
        if ( ++list_index >= count )
            return -1;
    }
    return list_index;
}
