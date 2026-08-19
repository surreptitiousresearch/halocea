/* effects_reconnect_to_structure_bsp @0x836E2170 — after a structure BSP swap, re-resolve the cluster
 * location of every unattached (world-space) effect. Walks all live effects; for each effect not parented
 * to an object (object_index == -1) it fetches the effect's first live location instance and recomputes the
 * effect's cached location from that marker's world position. Effects whose first location instance is gone
 * (the marker no longer resolves) are deleted.
 *
 * DEVIATION: the reconstruction returned the datum iterator's terminal -1; that r3 is loop residue —
 * the function is entry +0x0C of reconnect_to_structure_bsp_procs, a void(*)(void) table. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/effect_datum.h"
#include "headers/effect_location_datum.h"
#include "headers/blam_data_globals.h"

#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern effect_location_datum *effect_location_get_next_instance(const effect_datum *effect, int *location_datum_index, int16_t camera_mode);
extern void scenario_location_from_point(location *location, const real_point3d *point);
extern void effect_delete(int effect_index);

void effects_reconnect_to_structure_bsp(void)
{
    int result = data_next_index(effect_data, -1);
    for ( int i = result; result != -1; i = result )
    {
        effect_datum *effect = DATA_ARRAY_ELEMENT(effect_data, effect_datum, i);
        if ( effect->object_index == -1 )
        {
            int location_datum_index = effect->location_datum_indices[0];
            effect_location_datum *instance = effect_location_get_next_instance(effect, &location_datum_index, 0);
            if ( instance )
                scenario_location_from_point(&effect->location, &instance->matrix.position);
            else
                effect_delete(i);
        }
        result = data_next_index(effect_data, i);
    }
}
