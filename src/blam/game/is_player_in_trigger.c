/* is_player_in_trigger @0x836A8640 — true if the unit is inside the trigger volume referenced by the
 * given bsp-switch-trigger-volume entry. Returns false when switch_trigger_index is -1. The entry is
 * an 8-byte scenario_bsp_switch_trigger_volume; its trigger_volume_index selects the actual volume that
 * scenario_trigger_volume_test_object tests the unit's origin against. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_bsp_switch_trigger_volume.h"

extern uint8_t scenario_trigger_volume_test_object(int16_t trigger_volume_index, int object_index);

int is_player_in_trigger(int unit_index, int16_t switch_trigger_index)
{
    if ( switch_trigger_index == -1 )
        return 0;

    scenario_bsp_switch_trigger_volume *entry =
        &((scenario_bsp_switch_trigger_volume *)global_scenario->bsp_switch_trigger_volumes.address)
            [switch_trigger_index];

    if ( !scenario_trigger_volume_test_object(entry->trigger_volume_index, unit_index) )
        return 0;

    return 1;
}
