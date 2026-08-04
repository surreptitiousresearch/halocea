/* light_unmarked @0x836F84F8 — return true when the light has NOT been visited this gathering pass, i.e.
 * its per-datum marker (light_datum.marker, +0x0C) differs from the current lights_globals.marker stamp. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/light_datum.h"
#include "headers/blam_data_globals.h"

int light_unmarked(uint16_t light_index)
{
    light_datum *light = DATA_ARRAY_ELEMENT(light_data, light_datum, light_index);
    return lights_globals.marker != light->marker;
}
