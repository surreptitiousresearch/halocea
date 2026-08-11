/* devices_initialize_for_new_map @0x837B54E8 — per-map device_groups_data setup: marks the data_array valid,
 * then allocates and seeds one device_group_datum per scenario_device_group exactly like
 * create_initial_device_groups.c (cont63). */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_device_group_can_change_only_once_flags.h"
#include "headers/data_array.h"
#include "headers/device_group_datum.h"
#include "headers/scenario_device_group.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern void data_make_valid(data_array *data);
extern int datum_new(data_array *data);
extern void *datum_get(data_array *data, int index);

void devices_initialize_for_new_map(void)
{
    data_make_valid(device_groups_data);

    scenario *scen = global_scenario;

    for ( int16_t i = 0; i < scen->device_groups.count; i++ )
    {
        scenario_device_group *definition = (scenario_device_group *)scen->device_groups.address + i;
        uint16_t changes_only_once = (definition->flags & (1u << _scenario_device_group_can_change_only_once_bit)) != 0;
        float initial_value = definition->initial_value;

        int datum_index = datum_new(device_groups_data);
        if ( datum_index != -1 )
        {
            /* recovered: (char *)device_groups_data->data + ((8 * datum_index) & 0x7FFF8) -> datum_get; stride 8 = device_group_datum datum_size (incl. 2-byte salt header), (8*h)&0x7FFF8 == 8 * DATUM_INDEX_TO_ABSOLUTE_INDEX(h) */
            device_group_datum *datum =
                datum_get(device_groups_data, datum_index);
            datum->desired_value = initial_value;
            datum->flags = changes_only_once;
        }
    }
}
