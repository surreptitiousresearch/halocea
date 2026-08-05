/* unit_handle_weapon_state_change @0x836C8930 — when a held weapon enters a new state, play the matching
 * unit animation. Most weapon states map to an overlay action (fire/reload/charge etc.); two of them
 * (primary/secondary reload -> reload_1/reload_2) are full-body actions instead. Weapon states outside
 * primary_recoil..secondary_charged produce no animation. The new_weapon_state - 1 biasing (kept from the
 * binary) makes primary_recoil the first handled case; the case labels below are written as
 * <weapon_state> - 1 to preserve that shape while naming the states. */

#include <stdint.h>
#include "headers/weapon_datum_state.h"
#include "headers/unit_animation_action.h"
#include "headers/unit_animation_overlay_action.h"

extern void unit_animation_start_action(int unit_index, int16_t action);
extern void unit_animation_start_overlay_action(int unit_index, int16_t overlay_action);

void unit_handle_weapon_state_change(int unit_index, int16_t new_weapon_state)
{
    int16_t action = -1;
    int16_t overlay_action = -1;
    unsigned int state = new_weapon_state - 1;

    if ( state <= weapon_state_secondary_charged - 1 )
    {
        if ( state )
        {
            switch ( state )
            {
                case weapon_state_secondary_recoil - 1:  overlay_action = _unit_animation_overlay_action_fire_2;    break;
                case weapon_state_primary_chamber - 1:   overlay_action = _unit_animation_overlay_action_chamber_1; break;
                case weapon_state_secondary_chamber - 1: overlay_action = _unit_animation_overlay_action_chamber_2; break;
                case weapon_state_primary_reload - 1:    action = _unit_animation_action_reload_1;                  break;
                case weapon_state_secondary_reload - 1:  action = _unit_animation_action_reload_2;                  break;
                case weapon_state_primary_charged - 1:   overlay_action = _unit_animation_overlay_action_charged_1; break;
                default: /* weapon_state_secondary_charged */
                    overlay_action = _unit_animation_overlay_action_charged_2;
                    break;
            }
        }
        else
        {
            /* new_weapon_state == weapon_state_primary_recoil */
            overlay_action = _unit_animation_overlay_action_fire_1;
        }
    }

    if ( action == -1 )
    {
        if ( overlay_action != -1 )
            unit_animation_start_overlay_action(unit_index, overlay_action);
    }
    else
    {
        unit_animation_start_action(unit_index, action);
    }
}
