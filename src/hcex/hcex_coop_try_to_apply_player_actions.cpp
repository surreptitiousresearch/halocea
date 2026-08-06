/* hcex_coop_try_to_apply_player_actions @ 0x83688C30 — split-screen co-op bridge: take the two local
 * players' digested actions gathered in hcex_action_collection, push the look/zoom of each into its
 * player_control, then package both actions into the co-op "server update" and hand it to the client
 * update path (so the local co-op session is driven through the same update machinery as a network game).
 * Does nothing when running the online-test path, or before a local player index has been assigned. */

#include <stdint.h>
#include "../headers/player_action_collection.h"
#include "../headers/game_update.h"
#include "../headers/real_vector3d.h"
#include "../headers/real_euler_angles2d.h"

extern "C" int hcex_coop_online_test_local;
extern "C" int hcex_coop_local_player_index;
extern "C" game_update hcex_coop_update;
extern "C" player_action_collection hcex_action_collection;
extern "C" int hcex_coop_update_number;

extern "C" real_vector3d *vector3d_from_euler_angles2d(real_vector3d *vector, const real_euler_angles2d *angles);
extern "C" void player_control_set_facing(int16_t local_player_index, const real_vector3d *forward);
extern "C" void player_control_set_zoom_level(int16_t local_player_index, int16_t zoom_level);
extern "C" void update_client_handle_server_update(game_update *new_update, int update_number);

extern "C" void hcex_coop_try_to_apply_player_actions(void)
{
    int16_t local_player_index;

    if ( hcex_coop_online_test_local )
        return;
    if ( hcex_coop_local_player_index < 0 )
        return;

    hcex_coop_update.number_of_actions = 2;

    for ( local_player_index = 0; local_player_index < 2; ++local_player_index )
    {
        if ( hcex_coop_local_player_index )
        {
            real_vector3d forward;
            player_control_set_facing(
                local_player_index,
                vector3d_from_euler_angles2d(&forward,
                    &hcex_action_collection.actions[local_player_index].desired_facing));
            player_control_set_zoom_level(
                local_player_index,
                hcex_action_collection.actions[local_player_index].desired_zoom_level);
        }

        hcex_coop_update.completed_client_update_id[local_player_index] = -1;
        hcex_coop_update.actions[local_player_index] = hcex_action_collection.actions[local_player_index];
    }

    update_client_handle_server_update(&hcex_coop_update, hcex_coop_update_number);
}
