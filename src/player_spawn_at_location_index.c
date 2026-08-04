/* player_spawn_at_location_index @0x836AB8A8 — spawn a player's unit at an explicitly named scenario
 * starting-location index (as opposed to player_spawn, which picks the best location itself). When the index
 * is -1 the routine is a no-op. Otherwise, if the player-information tag supplies a valid unit definition, it
 * reads the location's position/facing from the scenario player-starting-locations block (52-byte stride),
 * builds an object_placement_data with the player's change color, and creates the biped. The fresh unit is
 * wired to the player (owner/team/controlling player), actively controlled, handed to player control, and (in
 * campaign) given starting equipment. The cleanup tail resets the player's pending action and marks its
 * observer position obsolete — all still gated inside the "index != -1" branch.
 *
 * Twin of player_spawn (@0x836ABAC8); models its raw player-datum (stride 512) and unit-object offsets. The
 * change-color __PAIR64__ stores in the decompiler are just four copies of the same color. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/game_globals_multiplayer_information.h"
#include "headers/game_globals_player_information.h"
#include "headers/object_placement_data.h"
#include "headers/real_rgb_color.h"
#include "headers/real_vector3d.h"
#include "headers/game_globals_tag.h"
#include "headers/scenario.h"
#include "headers/scenario_player.h"
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


extern uint8_t game_engine_running(void);
extern void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index);
extern real_vector3d *vector3d_from_angle(real_vector3d *vector, float angle);
extern real_rgb_color *game_engine_player_get_change_color(real_rgb_color *result, int player_index);
extern int object_new(object_placement_data *data);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void unit_set_actively_controlled(int unit_index, uint8_t actively_controlled);
extern void player_control_new_unit(int16_t local_player_index, int unit_index);
extern void player_add_equipment(int unit_index, int16_t profile_index, uint8_t reset);
extern void observer_obsolete_position(int16_t local_player_index);

void player_spawn_at_location_index(int player_index, int starting_location_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    if ( starting_location_index == -1 )
        return;

    game_globals_player_information *player_information = (game_globals_player_information *)global_game_globals->player_information.address;
    if ( player_information->player_unit.index != -1 )
    {
        scenario_player *starting_location = nullptr;
        if ( (__int16)starting_location_index >= 0
          && (__int16)starting_location_index < global_scenario->players.count )
            starting_location = &((scenario_player *)global_scenario->players.address)[(__int16)starting_location_index];

        int definition_index;
        if ( game_engine_running() )
            definition_index = ((game_globals_multiplayer_information *)global_game_globals->multiplayer_information.address)->player_unit.index;
        else
            definition_index = player_information->player_unit.index;

        object_placement_data placement;
        object_placement_data_new(&placement, definition_index, -1);
        placement.position.n[0] = starting_location->position.n[0];
        placement.position.n[1] = starting_location->position.n[1];
        placement.position.n[2] = starting_location->position.n[2];
        vector3d_from_angle(&placement.forward, starting_location->facing);
        placement.up.n[0] = global_up3d->n[0];
        placement.up.n[1] = global_up3d->n[1];
        placement.up.n[2] = global_up3d->n[2];

        real_rgb_color change_color_buffer[8];
        real_rgb_color *change_color = game_engine_player_get_change_color(change_color_buffer, player_index);
        placement.change_colors[0] = *change_color;
        placement.change_colors[1] = *change_color;
        placement.change_colors[2] = *change_color;
        placement.change_colors[3] = *change_color;

        int new_unit_index = object_new(&placement);
        if ( new_unit_index != -1 )
        {
            unit_datum *unit = object_try_and_get_and_verify_type(new_unit_index, object_mask_unit);
            if ( unit )
            {
                unit->object.owner_player_index = player_index;
                unit->object.owner_team_index = (__int16)player->team_index;
                unit->unit.player_index = player_index;             /* controlling player index */
                player->unit_index = new_unit_index;
                unit_set_actively_controlled(new_unit_index, 1);

                int local_player_index = (unsigned __int16)player->local_player_index;
                if ( local_player_index != 0xFFFF )
                    player_control_new_unit(local_player_index, new_unit_index);

                if ( !game_engine_running() )
                {
                    int starting_profile_count = global_scenario->starting_profiles.count;
                    __int16 starting_profile_index;
                    if ( starting_profile_count <= 1 || player->statistics.deaths <= 0 )
                    {
                        if ( !starting_profile_count )
                            goto cleanup;
                        starting_profile_index = 0;
                    }
                    else
                    {
                        starting_profile_index = 1;
                    }
                    player_add_equipment(player->unit_index, starting_profile_index, 1);
                }
            }
        }
    }

cleanup:
    /* 4-byte clear spans both powerup_durations[0] and [1] */
    *(int *)&player->powerup_durations[0] = 0;
    player->action_result = 0;
    player->action_object_index = -1;
    int local_player_index_final = (unsigned __int16)player->local_player_index;
    if ( local_player_index_final != 0xFFFF )
        observer_obsolete_position(local_player_index_final);
}
