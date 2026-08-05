/* create_initial_device_groups @0x837B5454 — allocate one device_group_datum per scenario-authored
 * scenario_device_group, seeded with its initial value and its "changes only once" flag (bit 0 of the
 * scenario definition's flags). Silently skips groups that fail to allocate a datum. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_device_group_can_change_only_once_flags.h"
#include "headers/data_array.h"
#include "headers/device_group_datum.h"
#include "headers/scenario_device_group.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern int datum_new(data_array *data);
extern void *datum_get(data_array *data, int index);

void create_initial_device_groups(void)
{
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
