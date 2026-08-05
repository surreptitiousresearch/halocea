/* first_person_weapon_message @0x836A0010 — apply a weapon "message" (a state-change notification such as
 * fire, reload-empty/partial, switch, drop) to a local player's first-person viewmodel.
 *
 * Most messages map directly to a first-person weapon STATE via
 * first_person_weapon_state_from_weapon_message and are applied with first_person_weapon_set_state. A few
 * carry side effects handled inline:
 *   - 0  (fire): nudges the firing push-back spring velocity.
 *   - 9/10 (reload empty/partial): unzooms the unit.
 *   - 12 (switch): runs the weapon-switch transition and resets the interpolation frame count.
 *   - 13 (drop): clears the bound weapon index.
 *
 * Special case: when the held weapon's definition is a shotgun-style "reload single rounds" weapon
 * (weapon_type == 1) and the message is a reload, the per-shell reload state machine is run here to choose the
 * shotgun reload variant (reload_empty = reload one more, enter = start the per-shell chain) based on how many
 * shells remain to load. */

#include <stdint.h>
#include "headers/first_person_weapon.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_magazine_definition.h"
#include "headers/first_person_weapon_message_type.h"
#include "headers/first_person_weapon_state.h"
#include "headers/weapon_type.h"
#include "headers/shotgun_reload_type.h"
#include "headers/blam_data_globals.h"
extern int _cntlzw(unsigned int);


extern void player_control_unzoom(int unit_index);
extern void first_person_weapon_switch_weapons(int16_t local_player_index);
extern int16_t first_person_weapon_state_from_weapon_message(int16_t message_type);
extern void first_person_weapon_set_state(int16_t local_player_index, int16_t new_state, uint8_t reset_sounds);

void first_person_weapon_message(int16_t local_player_index, int16_t message_type)
{
    if ( local_player_index == -1 )
        return;

    int16_t state_to_apply = -1;
    first_person_weapon *fp_weapon = &first_person_weapons[local_player_index];

    switch ( message_type )
    {
        case _first_person_weapon_message_primary_fire:   /* fire: add push-back velocity */
            fp_weapon->firing_push_back_velocity = fp_weapon->firing_push_back_velocity + 0.050000001f;
            break;
        case _first_person_weapon_message_reload_while_empty:
        case _first_person_weapon_message_reload_while_full:  /* reload: unzoom */
            player_control_unzoom(fp_weapon->unit_index);
            break;
        case _first_person_weapon_message_ready:  /* switch weapons */
            first_person_weapon_switch_weapons(local_player_index);
            break;
        case _first_person_weapon_message_drop:  /* drop: detach weapon */
            fp_weapon->weapon_index = -1;
            break;
        default:
            break;
    }

    if ( fp_weapon->weapon_index != -1 )
    {
        weapon_datum *weapon = (weapon_datum *)DATA_ARRAY_ELEMENT(
            object_header_data, object_header_datum, fp_weapon->weapon_index)->datum;
        if ( weapon->definition_index != -1 )
        {
            weapon_definition *definition =
                TAG_GET(weapon_definition, weapon->definition_index);
            if ( definition->weapon.weapon_type == _weapon_type_shotgun && (message_type == _first_person_weapon_message_reload_while_empty || message_type == _first_person_weapon_message_reload_while_full) )
            {
                weapon_magazine_definition *magazine_definition =
                    (weapon_magazine_definition *)definition->weapon.magazines.address;
                int weapon_state = fp_weapon->state;
                int16_t rounds_loaded = weapon->weapon.magazines[0].rounds_loaded;
                int16_t rounds_total = weapon->weapon.magazines[0].rounds_total;
                int16_t capacity = magazine_definition->rounds_loaded_maximum;

                /* mid-animation (one of the active reload/fire states) vs. a fresh reload */
                if ( weapon_state == _first_person_weapon_state_shotgun_enter_reload || weapon_state == _first_person_weapon_state_overheated_exit
                     || weapon_state == _first_person_weapon_state_shotgun_exit_reload_empty || weapon_state == _first_person_weapon_state_shotgun_exit_reload_full
                     || weapon_state == _first_person_weapon_state_reload_while_empty || weapon_state == _first_person_weapon_state_reload_while_full || weapon->weapon.magazines[0].state )
                {
                    int16_t shells_remaining = rounds_total;
                    if ( capacity - rounds_loaded <= rounds_total )
                        shells_remaining = capacity - rounds_loaded;
                    fp_weapon->shotgun_reload_type = (shells_remaining == 1) ? _shotgun_reload_type_last_round : -1;
                }
                else
                {
                    int16_t shells_to_reload = weapon->weapon.magazines[0].rounds_total;
                    if ( capacity - rounds_loaded <= rounds_total )
                        shells_to_reload = capacity - rounds_loaded;
                    fp_weapon->shotgun_shells_to_reload = shells_to_reload;
                    fp_weapon->shotgun_empty = (_cntlzw(rounds_loaded) & 0x20) != 0;
                    fp_weapon->shotgun_reload_type = (shells_to_reload != 1) ? _shotgun_reload_type_first_round
                                                                             : _shotgun_reload_type_first_and_last_round;
                }

                int16_t reload_type = fp_weapon->shotgun_reload_type;
                if ( reload_type == -1 )
                    state_to_apply = _first_person_weapon_state_reload_while_empty;
                else if ( !fp_weapon->shotgun_reload_type || reload_type == _shotgun_reload_type_first_and_last_round )
                    state_to_apply = _first_person_weapon_state_shotgun_enter_reload;
            }
        }
    }

    if ( state_to_apply == -1 )
        state_to_apply = first_person_weapon_state_from_weapon_message(message_type);
    if ( state_to_apply != -1 )
        first_person_weapon_set_state(local_player_index, state_to_apply, 1u);

    if ( message_type == _first_person_weapon_message_ready )
        fp_weapon->interpolation_frame_count = 0;
}
