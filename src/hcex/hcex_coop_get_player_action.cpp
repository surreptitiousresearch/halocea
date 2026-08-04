/* hcex_coop_get_player_action @0x83681928 — hcex bridge: fetch this tick's local-player actions for coop,
 * throttled to once per game tick (hcex_coop_last_tick). Builds the current player_action_collection and
 * flattens both local players' player_action into the ws-engine-side haloPLAYER_ACTION pair. Returns 0 if
 * already fetched this tick, 1 otherwise. */

#include "../headers/haloPLAYER_ACTION.h"
#include "../headers/player_action_collection.h"

extern int hcex_coop_last_tick;
extern int game_time_get(void);
extern void update_client_build_client_update(player_action_collection *action_collection);

extern "C" int hcex_coop_get_player_action(haloPLAYER_ACTION *hcex_actions)
{
    if ( hcex_coop_last_tick == game_time_get() )
        return 0;
    hcex_coop_last_tick = game_time_get();

    player_action_collection actions;
    update_client_build_client_update(&actions);

    for ( int i = 0; i < 2; i++ )
    {
        hcex_actions[i].control_flags = actions.actions[i].control_flags;
        hcex_actions[i].desired_facing_yaw = actions.actions[i].desired_facing.n[0];
        hcex_actions[i].desired_facing_pitch = actions.actions[i].desired_facing.n[1];
        hcex_actions[i].throttle_x = actions.actions[i].throttle.n[0];
        hcex_actions[i].throttle_y = actions.actions[i].throttle.n[1];
        hcex_actions[i].primary_trigger = actions.actions[i].primary_trigger;
        hcex_actions[i].desired_weapon_index = actions.actions[i].desired_weapon_index;
        hcex_actions[i].desired_grenade_index = actions.actions[i].desired_grenade_index;
        hcex_actions[i].desired_zoom_level = actions.actions[i].desired_zoom_level;
    }

    return 1;
}
