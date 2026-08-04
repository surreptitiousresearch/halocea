/* cheat_all_weapons @0x8369A280 — spawn one of each weapon in front of the acting player. Uses the game
 * globals' weapon_list block when present; otherwise falls back to enumerating every 'weap' tag (up to WEAPON_LIST_NUM_WEAPONS)
 * into a temporary reference list.
 *
 * DEVIATION: the decompiler aliases a raw byte scratch buffer; reconstructed with a typed reference array. */

#include <stdint.h>
#include "headers/game_globals_definition.h"
#include "headers/game_globals_tag_reference.h"
#include "headers/tag_iterator.h"
#include "headers/weapon_list_index.h"
#include "headers/blam_data_globals.h"
extern void tag_iterator_new(tag_iterator *iterator, uint32_t key_group_tag);
extern int tag_iterator_next(tag_iterator *iterator);

extern void cheat_objects(const game_globals_tag_reference *references, int16_t name_count);

void cheat_all_weapons(void)
{
    const game_globals_tag_reference *references;
    __int16 count;

    if ( global_game_globals->weapon_list.count
      && (references = (const game_globals_tag_reference *)global_game_globals->weapon_list.address) != nullptr )
    {
        count = global_game_globals->weapon_list.count;
    }
    else
    {
        game_globals_tag_reference weapons[WEAPON_LIST_NUM_WEAPONS];
        count = 0;
        tag_iterator iterator;
        tag_iterator_new(&iterator, 0x77656170u /* 'weap' */);
        for ( int tag_index = tag_iterator_next(&iterator); tag_index != -1; tag_index = tag_iterator_next(&iterator) )
        {
            if ( (unsigned __int16)count >= WEAPON_LIST_NUM_WEAPONS )
                break;
            weapons[count++].reference.index = tag_index;
        }
        references = weapons;
    }
    cheat_objects(references, count);
}
