/* encounters_unit_died @0x8370D098 — when a unit dies, credit every active enemy encounter that is in
 * combat against the dead unit's team with one more "enemy defeated". No-op for teamless units or before
 * AI is initialised for the map. */

#include <stdint.h>
#include "headers/unit_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/data_iterator.h"
#include "headers/encounter_datum.h"
#include "headers/encounter_iterator.h"
#include "headers/ai_globals.h"
#include "headers/blam_data_globals.h"

extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);

void encounters_unit_died(int unit_index)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    if ( (uint16_t)unit->object.owner_team_index == 0xFFFF )
        return;
    /* Deviation: decompiler scopes the iterator init under the ai_initialized check but runs the loop
     * unconditionally (uninitialised-iterator UB otherwise); the original clearly gates the whole walk. */
    if ( !ai_globals->ai_initialized_for_map )
        return;

    encounter_iterator iterator;
    data_iterator_new(&iterator.encounter_iterator, encounter_data);
    iterator.active_only = 1;

    for ( encounter_datum *encounter = encounter_iterator_next(&iterator);
          encounter;
          encounter = encounter_iterator_next(&iterator) )
    {
        if ( game_team_is_enemy(encounter->team_index, unit->object.owner_team_index)
          && encounter->enemy_target
          && !encounter->stand_down
          && !encounter->post_combat )
        {
            ++encounter->enemies_defeated;
        }
    }
}
