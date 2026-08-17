/* vehicle_place @0x8375CCF8 */
#include "headers/scenario_vehicle_datum.h"

extern void unit_place(int unit_index, scenario_unit_datum *scenario_unit);
extern void object_add_scenario_permutation(int object_index, struct scenario_object_permutation *permutation);

void vehicle_place(int vehicle_index, scenario_vehicle_datum *scenario_vehicle)
{
    unit_place(vehicle_index, &scenario_vehicle->unit);
    object_add_scenario_permutation(vehicle_index, &scenario_vehicle->permutation);
}
