/* biped_place @0x837AC378 */
#include "headers/scenario_biped_datum.h"

extern void unit_place(int unit_index, scenario_unit_datum *scenario_unit);
extern void object_add_scenario_permutation(int object_index, struct scenario_object_permutation *permutation);

void biped_place(int biped_index, scenario_biped_datum *scenario_biped)
{
    unit_place(biped_index, &scenario_biped->unit);
    object_add_scenario_permutation(biped_index, &scenario_biped->permutation);
}
