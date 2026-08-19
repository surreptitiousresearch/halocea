/* unit_set_mouth_aperture @0x836CD178 — move a unit's mouth-open value (unit data +0x2E8) toward the requested
 * aperture, but limit the per-call change to +-0.3 so the mouth animates smoothly rather than snapping. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"


void unit_set_mouth_aperture(int unit_index, float aperture)
{
    unit_datum *unit_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    float current = unit_data->unit.mouth_aperture;
    float delta = aperture - current;
    if ( delta < -0.30000001f )
        unit_data->unit.mouth_aperture = current + -0.30000001f;
    else if ( delta > 0.30000001f )
        unit_data->unit.mouth_aperture = current + 0.30000001f;
    else
        unit_data->unit.mouth_aperture = current + delta;
}
