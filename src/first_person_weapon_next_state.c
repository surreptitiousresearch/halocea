/* first_person_weapon_next_state @0x836A0358 — decide the first-person viewmodel's next state when the
 * current animation finishes. Idle/ready-style states return to idle (0); firing/charging states go to
 * the "put away"/ready transition (3). Shotgun reload states (0xD/0xE/0xF) advance into the per-shell
 * reload chain (16/17) only when the held weapon is a shotgun and a reload is pending. State 0x12 just
 * rewinds its animation a frame. If a new state was selected it is applied via
 * first_person_weapon_set_state. */

#include <stdint.h>
#include "headers/first_person_weapon.h"
#include "headers/global_tag_instances.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_definition.h"
#include "headers/first_person_weapon_state.h"
#include "headers/weapon_type.h"
#include "headers/shotgun_reload_type.h"
#include "headers/blam_data_globals.h"

extern void first_person_weapon_set_state(int16_t local_player_index, int16_t new_state, uint8_t reset_sounds);

void first_person_weapon_next_state(int16_t local_player_index)
{
    int16_t new_state = -1;
    first_person_weapon *fpw = &first_person_weapons[local_player_index];

    switch (fpw->state)
    {
        case _first_person_weapon_state_idle:
        case _first_person_weapon_state_posing:
        case _first_person_weapon_state_primary_fire:
        case _first_person_weapon_state_secondary_fire:
        case _first_person_weapon_state_primary_misfire:
        case _first_person_weapon_state_secondary_misfire:
        case _first_person_weapon_state_melee:
        case _first_person_weapon_state_light_off:
        case _first_person_weapon_state_light_on:
        case _first_person_weapon_state_shotgun_exit_reload_empty:
        case _first_person_weapon_state_shotgun_exit_reload_full:
        case _first_person_weapon_state_ready:
        case _first_person_weapon_state_throw_grenade:
        case _first_person_weapon_state_overheated_exit:
            new_state = _first_person_weapon_state_idle;
            break;

        case _first_person_weapon_state_overheating:
        case _first_person_weapon_state_overheating_again:
        case _first_person_weapon_state_throw_grenade_overheated:
        case _first_person_weapon_state_overheating_super_recoil:
            new_state = _first_person_weapon_state_overheated;
            break;

        case _first_person_weapon_state_reload_while_empty:
        case _first_person_weapon_state_reload_while_full:
        {
            weapon_definition *weapon_def = TAG_GET(weapon_definition, DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, fpw->weapon_index)->datum->definition_index);
            if (weapon_def->weapon.weapon_type != _weapon_type_shotgun || !fpw->shotgun_reload_type || fpw->shotgun_reload_type == -1)
                new_state = _first_person_weapon_state_idle;
            else
                new_state = (fpw->shotgun_empty == 0) + _first_person_weapon_state_shotgun_exit_reload_empty; /* exit_empty or exit_full */
            break;
        }

        case _first_person_weapon_state_shotgun_enter_reload:
        {
            weapon_definition *weapon_def = TAG_GET(weapon_definition, DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, fpw->weapon_index)->datum->definition_index);
            if (weapon_def->weapon.weapon_type == _weapon_type_shotgun && fpw->shotgun_reload_type == _shotgun_reload_type_first_and_last_round)
                new_state = (fpw->shotgun_empty == 0) + _first_person_weapon_state_shotgun_exit_reload_empty; /* exit_empty or exit_full */
            else
                new_state = _first_person_weapon_state_idle;
            break;
        }

        case _first_person_weapon_state_put_away:
            --fpw->state_animation.frame_index;
            break;

        default:
            break;
    }

    if (new_state != -1)
        first_person_weapon_set_state(local_player_index, new_state, 0);
}
