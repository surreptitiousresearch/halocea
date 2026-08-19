/* cheat_all_vehicles @0x8369A528 — spawn one of each vehicle in front of the acting player. Prefers the
 * vehicle list inside the game globals' multiplayer_information block; otherwise enumerates every 'vehi'
 * tag (up to 16) that defines at least one seat (vehicle tag +0x2CC > 0).
 *
 * DEVIATION: the decompiler aliases a raw byte scratch buffer; reconstructed with a typed reference array.
 * The multiplayer vehicle list is a tag_block nested at byte 32 of the multiplayer_information element. */

#include <stdint.h>
#include "headers/game_globals_definition.h"
#include "headers/game_globals_multiplayer_information.h"
#include "headers/game_globals_tag_reference.h"
#include "headers/tag_block.h"
#include "headers/tag_iterator.h"
#include "headers/global_tag_instances.h"
#include "headers/vehicle_definition.h"
#include "headers/blam_data_globals.h"
extern void tag_iterator_new(tag_iterator *iterator, uint32_t key_group_tag);
extern int tag_iterator_next(tag_iterator *iterator);

extern void cheat_objects(const game_globals_tag_reference *references, int16_t name_count);

void cheat_all_vehicles(void)
{
    const game_globals_tag_reference *references;
    int16_t count;

    if ( global_game_globals->multiplayer_information.count )
    {
        const tag_block *vehicles = &((game_globals_multiplayer_information *)global_game_globals->multiplayer_information.address)->vehicles;
        references = (const game_globals_tag_reference *)vehicles->address;
        count = vehicles->count;
    }
    else
    {
        game_globals_tag_reference vehicle_list[16];
        count = 0;
        tag_iterator iterator;
        tag_iterator_new(&iterator, 0x76656869u /* 'vehi' */);
        for ( int tag_index = tag_iterator_next(&iterator); tag_index != -1; tag_index = tag_iterator_next(&iterator) )
        {
            if ( (uint16_t)count >= 0x10u )
                break;
            vehicle_definition *vehicle_def = TAG_GET(vehicle_definition, tag_index);
            if ( vehicle_def->unit.powered_seats.count > 0 ) /* +716 */
                vehicle_list[count++].reference.index = tag_index;
        }
        references = vehicle_list;
    }
    cheat_objects(references, count);
}
