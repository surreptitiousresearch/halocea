/* players_reconnect_to_structure_bsp @0x836ACB58 — after a structure-BSP switch, gather every local player onto
 * the same side of the swap. Only runs when a BSP-switch trigger is pending AND there is more than one local
 * player (single-player teleports are handled per-player). The trigger's associated cutscene flag (word @+6 of
 * the 8-byte bsp_switch_trigger_volume record) provides a candidate destination: its position is nudged upward
 * in 0.05-unit steps (at most 0.3) until it is clear of collision. Then the first player whose unit is still
 * inside the switch trigger volume and whose physics pill resolves to a valid cluster becomes the anchor: the
 * destination is the cleared flag position raised by the anchor's pill width (or, with no valid flag position,
 * the anchor's own pill base). Every OTHER local player with a live unit is then teleported to that destination
 * (player_teleport_on_bsp_switch) and the pending trigger is consumed. Finally, unconditionally, every player's
 * cached BSP cluster word (@+60) is invalidated to force re-resolution against the new structure BSP.
 *
 * DEVIATION: the decompiler's post-loop `if (raised < 0.3)` re-check is unreachable (the loop exits via the
 * >= 0.3 break) and is folded into the break. The junk pointer return (last data_iterator_next result, always
 * null) is kept to match the DB prototype, as in objects_reconnect_to_structure_bsp. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/data_iterator.h"
#include "headers/players_globals.h"
#include "headers/scenario.h"
#include "headers/scenario_cutscene_flag.h"
#include "headers/scenario_bsp_switch_trigger_volume.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"


extern uint8_t collision_test_point(unsigned int flags, const real_point3d *point, int ignore_object_index);
extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);
extern uint8_t scenario_trigger_volume_test_object(int16_t trigger_volume_index, int object_index);
extern void biped_get_physics_pill(int biped_index, real_point3d *base, float *height, float *width);
extern int16_t scenario_cluster_index_from_point(const real_point3d *point);
extern int16_t local_player_get_next(int16_t local_player_index);
extern void player_teleport_on_bsp_switch(int player_index, int source_unit_index, const real_point3d *position);

int16_t *players_reconnect_to_structure_bsp(void)
{
    data_iterator iterator;

    if ( (uint16_t)players_globals->bsp_switch_trigger_index != 0xFFFF
      && players_globals->local_player_count > 1 )
    {
        int anchor_unit_index = -1;
        uint8_t found_anchor = 0;
        uint8_t flag_position_valid = 0;
        real_point3d destination;

        int16_t cutscene_flag_index =
            ((const scenario_bsp_switch_trigger_volume *)global_scenario->bsp_switch_trigger_volumes.address)
                [(uint16_t)players_globals->bsp_switch_trigger_index].safe_flag_index;
        if ( cutscene_flag_index != -1 )
        {
            destination = ((const scenario_cutscene_flag *)global_scenario->cutscene_flags.address)
                    [cutscene_flag_index].position;
            float raised = 0.0f;
            flag_position_valid = 1;
            while ( collision_test_point(0x4029u, &destination, -1) )
            {
                raised = raised + 0.05f;
                destination.n[2] = destination.n[2] + 0.05f;
                if ( raised >= 0.3f )
                {
                    flag_position_valid = 0;
                    break;
                }
            }
        }

        data_iterator_new(&iterator, player_data);
        for ( player_datum *player = data_iterator_next(&iterator); player;
                player = data_iterator_next(&iterator) )
        {
            if ( found_anchor )
                break;
            int unit_index = player->unit_index;
            if ( unit_index == -1 )
                continue;

            int16_t trigger_index = players_globals->bsp_switch_trigger_index;
            const scenario_bsp_switch_trigger_volume *trigger_volume;
            if ( trigger_index == -1 )
                trigger_volume = 0;
            else
                trigger_volume = &((const scenario_bsp_switch_trigger_volume *)
                        global_scenario->bsp_switch_trigger_volumes.address)[trigger_index];
            if ( trigger_index == -1 || !scenario_trigger_volume_test_object(trigger_volume->trigger_volume_index, unit_index) )
                continue;

            real_point3d pill_points[2];
            float pill_height;
            float pill_width;
            biped_get_physics_pill(player->unit_index, pill_points, &pill_height, &pill_width);
            if ( scenario_cluster_index_from_point(pill_points) != -1 )
            {
                if ( flag_position_valid )
                    destination.n[2] = pill_width + destination.n[2];
                else
                    destination = pill_points[0];
                anchor_unit_index = player->unit_index;
                found_anchor = 1;
            }
        }

        if ( found_anchor )
        {
            for ( int16_t local_player = local_player_get_next(-1); local_player != -1;
                    local_player = local_player_get_next(local_player) )
            {
                int player_index = players_globals->local_player_indices[local_player];
                player_datum *local_player_datum = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
                int unit_index = local_player_datum->unit_index;
                if ( unit_index != -1 && unit_index != anchor_unit_index )
                {
                    player_teleport_on_bsp_switch(player_index, anchor_unit_index, &destination);
                    local_player_datum->cluster_index = -1;
                }
            }
        }

        players_globals->bsp_switch_trigger_index = -1;
    }

    data_iterator_new(&iterator, player_data);
    player_datum *player = data_iterator_next(&iterator);
    while ( player )
    {
        player->cluster_index = -1;    /* invalidate cached BSP cluster for every player */
        player = data_iterator_next(&iterator);
    }
    return (int16_t *)player;
}
