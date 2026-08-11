/* light_mark @0x836F8530 — stamp a light as visited this gathering pass. If the light's per-datum marker
 * (light_datum.marker, +0x0C) already equals the current lights_globals.marker counter it was already
 * visited and 0 is returned; otherwise the stamp is updated and 1 is returned (newly marked). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/light_datum.h"
#include "headers/blam_data_globals.h"

int light_mark(int light_index)
{
    light_datum *light = DATA_ARRAY_ELEMENT(light_data, light_datum, light_index);
    if ( light->marker == lights_globals.marker )
        return 0;
    light->marker = lights_globals.marker;
    return 1;
}
