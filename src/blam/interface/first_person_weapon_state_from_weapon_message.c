/* first_person_weapon_state_from_weapon_message @0x8369DE20 — map a weapon "message" code to the
 * first-person viewmodel state it should drive (see first_person_weapon_set_state). Returns -1 for messages
 * that have no associated first-person state. Pure lookup table. */

#include <stdint.h>
#include "headers/first_person_weapon_message_type.h"
#include "headers/first_person_weapon_state.h"

/* ATTEST: 16-bit in/out. Param read via `extsh r11,r3`; return consumed via `extsh r11,r3`
 * at weapon_play_first_person_weapon_sound (0x8369E0E4) — declared 16-bit signed. */
int16_t first_person_weapon_state_from_weapon_message(int16_t message_type)
{
    switch ( message_type )
    {
        case _first_person_weapon_message_primary_fire:  return _first_person_weapon_state_primary_fire;
        case _first_person_weapon_message_secondary_fire:  return _first_person_weapon_state_secondary_fire;
        case _first_person_weapon_message_primary_misfire:  return _first_person_weapon_state_primary_misfire;
        case _first_person_weapon_message_secondary_misfire:  return _first_person_weapon_state_secondary_misfire;
        case _first_person_weapon_message_melee:  return _first_person_weapon_state_melee;
        case _first_person_weapon_message_light_on:  return _first_person_weapon_state_light_on;
        case _first_person_weapon_message_light_off:  return _first_person_weapon_state_light_off;
        case _first_person_weapon_message_reload_while_empty:  return _first_person_weapon_state_reload_while_empty;
        case _first_person_weapon_message_reload_while_full: return _first_person_weapon_state_reload_while_full;
        case _first_person_weapon_message_put_away: return _first_person_weapon_state_put_away;
        case _first_person_weapon_message_ready: return _first_person_weapon_state_ready;
        case _first_person_weapon_message_charged: return _first_person_weapon_state_charged;
        case _first_person_weapon_message_overheating: return _first_person_weapon_state_overheating;
        case _first_person_weapon_message_overheating_super_recoil: return _first_person_weapon_state_overheating_super_recoil;
        case _first_person_weapon_message_throw_grenade: return _first_person_weapon_state_throw_grenade;
        default: return -1;   /* no first-person state for this message */
    }
}
