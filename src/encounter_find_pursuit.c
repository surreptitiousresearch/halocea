/* encounter_find_pursuit @0x83709D50 — find (or, if force_create, allocate) the pursuit-tracking record
 * for a given firing position within an encounter, threading a singly-linked list rooted at
 * encounter.first_pursuit_index and chained through pursuit_datum.next_pursuit_index. If the matched (or
 * newly created) record's last_examined_time is older than history_start_time, its tracking state
 * (actor_indices[6], actor_count, next_actor_index_index, last_examined_time) is reset to "empty".
 * Returns the record's index, or -1 if force_create was false and no fresh record was found/created.
 * 2026-07-14: pursuit_datum header created from DB types_members; fully typed. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/encounter_datum.h"
#include "headers/pursuit_datum.h"
#include "headers/blam_data_globals.h"


extern int datum_new(data_array *data);

int encounter_find_pursuit(uint16_t encounter_index, int16_t firing_position_index, int history_start_time, uint8_t force_create)
{
    encounter_datum *encounter = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index);
    unsigned __int8 needs_reset = 0;
    int pursuit_index = encounter->first_pursuit_index;
    pursuit_datum *pursuit_record = nullptr;

    if ( pursuit_index != -1 )
    {
        while ( 1 )
        {
            pursuit_record = DATUM_GET(pursuit_data, pursuit_datum, pursuit_index);
            if ( pursuit_record->firing_position_index == firing_position_index )
                break;

            pursuit_index = pursuit_record->next_pursuit_index;
            if ( pursuit_index == -1 )
                goto create_or_bail;
        }

        if ( pursuit_record->last_examined_time < history_start_time )
            needs_reset = 1;
    }
    else
    {
create_or_bail:
        if ( force_create )
        {
            pursuit_index = datum_new(pursuit_data);
            if ( pursuit_index != -1 )
            {
                needs_reset = 1;
                pursuit_record = DATUM_GET(pursuit_data, pursuit_datum, pursuit_index);
                pursuit_record->firing_position_index = firing_position_index;
                pursuit_record->next_pursuit_index = encounter->first_pursuit_index;
                encounter->first_pursuit_index = pursuit_index;
            }
        }
    }

    if ( needs_reset )
    {
        pursuit_datum *reset_record = DATUM_GET(pursuit_data, pursuit_datum, pursuit_index);

        reset_record->last_examined_time = -1;
        reset_record->actor_count = 0;
        reset_record->next_actor_index_index = 0;

        for ( int i = 0; i < 6; ++i )
            reset_record->actor_indices[i] = -1;

        if ( !force_create )
            return -1;
    }

    return pursuit_index;
}
