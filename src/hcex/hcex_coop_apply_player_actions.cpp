/* hcex_coop_apply_player_actions @0x83681A20 — hcex bridge: copy both coop players' digested actions from
 * the ws-engine's flat haloPLAYER_ACTION records into Blam's hcex_action_collection, bump the coop update
 * number, and arm one tick of application (consumed by hcex_coop_try_to_apply_player_actions). No-op
 * unless a coop local player is active. */

#include "../headers/haloPLAYER_ACTION.h"
#include "../headers/player_action_collection.h"

extern int hcex_coop_local_player_index;
extern int hcex_coop_update_number;
extern int hcex_coop_nticks_to_apply;
extern player_action_collection hcex_action_collection;

extern "C" void hcex_coop_apply_player_actions(haloPLAYER_ACTION *hcex_actions)
{
    if ( hcex_coop_local_player_index < 0 )
        return;

    ++hcex_coop_update_number;

    hcex_action_collection.actions[0].control_flags = hcex_actions[0].control_flags;
    hcex_action_collection.actions[0].desired_facing = *(real_euler_angles2d *)&hcex_actions[0].desired_facing_yaw;
    hcex_action_collection.actions[0].desired_grenade_index = hcex_actions[0].desired_grenade_index;
    hcex_action_collection.actions[0].desired_weapon_index = hcex_actions[0].desired_weapon_index;
    hcex_action_collection.actions[0].desired_zoom_level = hcex_actions[0].desired_zoom_level;
    hcex_action_collection.actions[0].primary_trigger = hcex_actions[0].primary_trigger;
    hcex_action_collection.actions[0].throttle = *(real_vector2d *)&hcex_actions[0].throttle_x;

    hcex_action_collection.actions[1].control_flags = hcex_actions[1].control_flags;
    hcex_action_collection.actions[1].desired_facing = *(real_euler_angles2d *)&hcex_actions[1].desired_facing_yaw;
    hcex_action_collection.actions[1].desired_grenade_index = hcex_actions[1].desired_grenade_index;
    hcex_action_collection.actions[1].desired_weapon_index = hcex_actions[1].desired_weapon_index;
    hcex_action_collection.actions[1].desired_zoom_level = hcex_actions[1].desired_zoom_level;
    hcex_action_collection.actions[1].primary_trigger = hcex_actions[1].primary_trigger;
    hcex_action_collection.actions[1].throttle.n[0] = hcex_actions[1].throttle_x;
    hcex_action_collection.actions[1].throttle.n[1] = hcex_actions[1].throttle_y;

    hcex_coop_nticks_to_apply = 1;
}
