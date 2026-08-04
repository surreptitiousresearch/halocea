/* effect_try_and_get @0x836E0C68 — fetch the effect datum at `index` from the global effect_data array,
 * returning NULL if the slot is unallocated. Thin typed wrapper over datum_try_and_get. */

#include "headers/data_array.h"
#include "headers/effect_datum.h"
#include "headers/blam_data_globals.h"

extern void *datum_try_and_get(const data_array *data, int index);

effect_datum *effect_try_and_get(int index)
{
    return datum_try_and_get(effect_data, index);
}
