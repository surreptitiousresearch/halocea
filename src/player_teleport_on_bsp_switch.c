/* player_teleport_on_bsp_switch @0x836ACA08 — after a structure BSP swap, check whether the player's
 * unit needs teleporting to stay in a valid location: if the player's globally-configured BSP-switch
 * trigger volume no longer contains their unit (or none is configured), and either the unit's bounding
 * sphere center no longer resolves to a valid BSP leaf or the trigger volume test failed, force the
 * player out of their current vehicle seat if it differs from source_unit's, then teleport (or, if
 * already parented into a vehicle whose parent didn't change, treat it as already handled). Marks
 * players_globals->force_respawn if the teleport failed.
 *
 * DEVIATION: `_cntlzw(v12) & 0x20` is the standard "count-leading-zeros" idiom for `v12 == 0`,
 * simplified here to a plain comparison. */

#include <stdint.h>
#include "headers/object_type.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/players_globals.h"
#include "headers/scenario.h"
#include "headers/scenario_bsp_switch_trigger_volume.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"


extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t scenario_trigger_volume_test_object(int16_t trigger_volume_index, int object_index);
extern int scenario_leaf_index_from_point(const real_point3d *point);
extern void unit_exit_seat_end(int object_index, uint8_t is_part_of_non_forced_exit, uint8_t should_allow_clients, uint8_t should_replicate_if_appropriate);
extern uint8_t player_teleport_internal(int player_index, int source_unit_index, const real_point3d *position);

void player_teleport_on_bsp_switch(int player_index, int source_unit_index, const real_point3d *position)
{
    int unit_index = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index;
    object_datum *unit = object_try_and_get_and_verify_type(unit_index, object_mask_biped);
    if ( !unit )
        return;

    int16_t bsp_switch_trigger_index = players_globals->bsp_switch_trigger_index;
    uint8_t needs_teleport;

    if ( bsp_switch_trigger_index == -1 )
    {
        needs_teleport = 1;
    }
    else
    {
        const scenario_bsp_switch_trigger_volume *trigger_volume =
            &((const scenario_bsp_switch_trigger_volume *)global_scenario->bsp_switch_trigger_volumes.address)[bsp_switch_trigger_index];
        needs_teleport = !scenario_trigger_volume_test_object(trigger_volume->trigger_volume_index, unit_index);
    }

    if ( scenario_leaf_index_from_point(&unit->object.bounding_sphere_center) == -1 || needs_teleport )
    {
        int parent_object_index = unit->object.parent_object_index;

        if ( parent_object_index != -1 )
        {
            object_datum *source_unit = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, source_unit_index)->datum);
            if ( parent_object_index != source_unit->object.parent_object_index )
                unit_exit_seat_end(unit_index, 0, 0, 1u);
        }

        uint8_t teleported;
        if ( unit->object.parent_object_index == -1 )
            teleported = player_teleport_internal(player_index, source_unit_index, position);
        else
            teleported = 1;

        players_globals->force_respawn = (teleported == 0);
    }
}
