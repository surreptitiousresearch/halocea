/* vehicle_set_scenario_datum_index @0x83761020 — record which scenario placement datum a spawned vehicle came
 * from, stored at object data +1456. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/blam_data_globals.h"


void vehicle_set_scenario_datum_index(int vehicle_index, int16_t scenario_datum_index)
{
    vehicle_datum *vehicle = (vehicle_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;
    /* DB member ___u27 is an anonymous 2-byte union; scenario-datum-index arm */
    vehicle->vehicle.___u27.vehicle_scenario_datum_index = scenario_datum_index;
}
