/* first_person_animation_type_from_weapon_state @0x8369DF10 — map a first-person viewmodel state to the
 * weapon animation-graph animation type used to play it (and to locate its associated first-person sound,
 * see weapon_play_first_person_weapon_sound). Returns -1 for states with no animation. Pure lookup table. */

#include <stdint.h>
#include "headers/first_person_weapon_state.h"
#include "headers/first_person_weapon_animation.h"

/* ATTEST: 16-bit in/out. Param read via `extsh r11,r3`; return consumed via `extsh r8,r3`
 * at weapon_play_first_person_weapon_sound (0x8369E0F4) — declared 16-bit signed. */
int16_t first_person_animation_type_from_weapon_state(int16_t state)
{
    switch ( state )
    {
        case _first_person_weapon_state_idle:              return _first_person_weapon_animation_idle;
        case _first_person_weapon_state_overheating:       return _first_person_weapon_animation_overheating;
        case _first_person_weapon_state_overheating_again: return _first_person_weapon_animation_overheating_again;
        case _first_person_weapon_state_overheated:        return _first_person_weapon_animation_overheated;
        case _first_person_weapon_state_charged:       return _first_person_weapon_animation_overcharged;
        case _first_person_weapon_state_posing:            return _first_person_weapon_animation_posing;
        case _first_person_weapon_state_primary_fire:            return _first_person_weapon_animation_primary_fire;
        case _first_person_weapon_state_secondary_fire:            return _first_person_weapon_animation_secondary_fire;
        case _first_person_weapon_state_primary_misfire:         return _first_person_weapon_animation_primary_misfire;
        case _first_person_weapon_state_secondary_misfire:         return _first_person_weapon_animation_secondary_misfire;
        case _first_person_weapon_state_melee:             return _first_person_weapon_animation_melee;
        case _first_person_weapon_state_light_off:         return _first_person_weapon_animation_light_off;
        case _first_person_weapon_state_light_on:          return _first_person_weapon_animation_light_on;
        case _first_person_weapon_state_reload_while_empty:      return _first_person_weapon_animation_reload_while_empty;
        case _first_person_weapon_state_reload_while_full:       return _first_person_weapon_animation_reload_while_full;
        case _first_person_weapon_state_shotgun_enter_reload:             return _first_person_weapon_animation_shotgun_enter;
        case _first_person_weapon_state_shotgun_exit_reload_empty:        return _first_person_weapon_animation_shotgun_exit_empty;
        case _first_person_weapon_state_shotgun_exit_reload_full:         return _first_person_weapon_animation_shotgun_exit_full;
        case _first_person_weapon_state_put_away:          return _first_person_weapon_animation_put_away;
        case _first_person_weapon_state_ready:             return _first_person_weapon_animation_ready;
        case _first_person_weapon_state_throw_grenade:     return _first_person_weapon_animation_throw_grenade;
        case _first_person_weapon_state_throw_grenade_overheated:  return _first_person_weapon_animation_throw_grenade_overheated;
        case _first_person_weapon_state_overheated_exit:          return _first_person_weapon_animation_overheated_exit;
        case _first_person_weapon_state_overheating_super_recoil:       return _first_person_weapon_animation_overheated_supercharge_enter;
        default: return -1;
    }
}
