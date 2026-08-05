/* effects_information_get @0x836E1148 — fill an effects_information snapshot: total effect and
 * effect-location datum occupancy, plus a count of active effects (those without the stopped
 * (_effect_stopped_bit, 0x8) or invisible (_effect_invisible_bit, 0x10) datum flag bits set; the flags
 * word sits at datum offset 2, 252-byte stride). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/effect_datum.h"
#include "headers/effect_flags.h"
#include "headers/effects_information.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);

void effects_information_get(effects_information *information)
{
    information->effect_count = effect_data->actual_count;
    information->location_count = effect_location_data->actual_count;
    information->active_effect_count = 0;

    for ( int effect_index = data_next_index(effect_data, -1); effect_index != -1;
          effect_index = data_next_index(effect_data, effect_index) )
    {
        int16_t flags = DATA_ARRAY_ELEMENT(effect_data, effect_datum, effect_index)->flags;
        if ( (flags & (1u << _effect_stopped_bit)) == 0 && (flags & (1u << _effect_invisible_bit)) == 0 )
            ++information->active_effect_count;
    }
}
