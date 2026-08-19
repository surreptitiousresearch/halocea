/* unit_get_zoom_level @0x836C7EC0 — the unit's current zoom level (-1 when not zoomed). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"


int unit_get_zoom_level(int unit_index)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    return unit->unit.current_zoom_level;
}
