/* reply_filter_close @0x837CBF60 — ai_conversation reply filter: true if `reply_actor_index` has a prop
 * (prop_datum) close to `original_unit_index` (prop->distance under 5.0 and prop->line_of_sight <= 1). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/ai_information_packet.h"
#include "headers/prop_datum.h"
#include "headers/ai_line_of_sight.h"
#include "headers/blam_data_globals.h"

extern int prop_get_base_by_unit_index(int actor_index, int unit_index, uint8_t acquire, uint8_t refresh_upon_acquisition);

uint8_t reply_filter_close(int original_unit_index, ai_information_packet *communication, int reply_actor_index)
{
    if ( reply_actor_index == -1 )
        return 0;

    int base_by_unit_index = prop_get_base_by_unit_index(reply_actor_index, original_unit_index, 1, 1);
    if ( base_by_unit_index == -1 )
        return 0;

    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, base_by_unit_index);
    return prop->distance < 5.0f && (unsigned int)prop->line_of_sight <= _ai_line_of_sight_occluded;
}
