/* encounter_link_activation @0x8370A9F8 — add link_encounter_index to an encounter's activation-link list,
 * so activating this encounter also activates the linked one. The list holds at most 3 entries (count at
 * +0x20, entries at +0x22). Returns 1 if the link is present after the call (already there or newly added),
 * 0 if the list was full. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/encounter_datum.h"
#include "headers/blam_data_globals.h"


int encounter_link_activation(uint16_t encounter_index, int16_t link_encounter_index)
{
    encounter_datum *encounter = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index);
    int result = 0;
    int link_count = encounter->link_encounter_count;   /* +0x20 */
    if ( link_count > 0 )
    {
        int i = 0;
        while ( encounter->link_encounter_indices[i] != link_encounter_index )
        {
            i = (__int16)(i + 1);
            if ( i >= encounter->link_encounter_count )
                goto check_room;
        }
        result = 1;
    }
check_room:
    if ( !(unsigned char)result && link_count < 3 )
    {
        encounter->link_encounter_indices[link_count] = link_encounter_index;
        ++encounter->link_encounter_count;
        return 1;
    }
    return result;
}
