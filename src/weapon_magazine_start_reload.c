/* weapon_magazine_start_reload @0x836DBD70 — begin a reload on a weapon magazine: validate the magazine is
 * idle and loadable, pull the reserve/loaded counts (replicating them across the network mirror for
 * authoritative weapons), kick off the network reload + first-person reload animation/effect, and set the
 * magazine into the reloading state with the animation's duration as its timer.
 * `first_round` distinguishes the first reload step (which seeds the network mirror
 * magazine_rounds_*_at_reload_start/_to_use_at_reload_finish) from continuation steps. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_magazine_definition.h"
#include "headers/networked_datum_role.h"
#include "headers/game_connection.h"
#include "headers/weapon_magazine_state.h"
#include "headers/weapon_trigger_state.h"
#include "headers/weapon_type.h"
#include "headers/weapon_datum_flags.h"
#include "headers/weapon_datum_state.h"
#include "headers/first_person_weapon_message_type.h"
#include "headers/first_person_weapon_animation.h"
#include "headers/blam_data_globals.h"


extern uint8_t weapon_magazine_state_change_ok(int weapon_index);
extern int16_t game_connection(void);
extern void weapon_start_reload_to_network(int weapon_index, int16_t magazine_index);
extern uint8_t weapon_set_state(int weapon_index, int16_t new_state, uint8_t immediate);
extern int weapon_effect_new(int weapon_index, int effect_index, float effect_scale, float effect_error);
extern void first_person_weapon_message_from_weapon(int weapon_index, int16_t message_type);
extern uint16_t weapon_get_first_person_animation_time(int weapon_index, int16_t mode, int16_t animation_type, int16_t shotgun_reload_type);

void weapon_magazine_start_reload(int weapon_index, int16_t magazine_index, uint8_t first_round)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_magazine *magazine = &weapon->weapon.magazines[magazine_index];
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);
    weapon_magazine_definition *magazine_definition =
        &((weapon_magazine_definition *)definition->weapon.magazines.address)[magazine_index];

    /* authoritative weapons: clear a stuck trigger (state 7) when the magazine is idle and a state change
     * is not otherwise permitted */
    if ( weapon->object.datum_role == _networked_datum_puppet )
    {
        char idle = (!magazine->state || magazine->state == _weapon_magazine_reloaded) ? 1 : 0;
        if ( idle == 1 && !weapon_magazine_state_change_ok(weapon_index)
             && weapon->weapon.triggers[0].state == _weapon_trigger_firing_held )
        {
            weapon->weapon.triggers[0].state = _weapon_trigger_idle;
            weapon->weapon.triggers[0].state_timer = 0;
        }
    }

    if ( !magazine->state || magazine->state == _weapon_magazine_reloaded )
    {
        /* only proceed if no trigger is firing, no melee in progress and no overheat */
        char proceed = (weapon->weapon.triggers[0].state || weapon->weapon.triggers[1].state
                        || weapon->weapon.state) ? 0 : 1;

        if ( proceed )
        {
            if ( weapon->object.datum_role == _networked_datum_puppet )   /* authoritative: seed the network mirror counts */
            {
                if ( first_round == 1 )
                {
                    int16_t reserve = weapon->weapon.magazine_rounds_total_at_reload_start[magazine_index];
                    magazine->rounds_total = reserve;
                    magazine->rounds_loaded =
                        weapon->weapon.magazine_rounds_loaded_at_reload_start[magazine_index];
                    weapon->weapon.magazine_rounds_total_to_use_at_reload_finish[magazine_index] = reserve;
                }
                else
                {
                    weapon->weapon.magazine_rounds_total_to_use_at_reload_finish[magazine_index] =
                        magazine->rounds_total;
                }
            }

            if ( magazine->rounds_total > 0
                 && magazine->rounds_loaded < magazine_definition->rounds_loaded_maximum )
            {
                int16_t reload_variant = -1;

                if ( first_round == 1 && !weapon->object.datum_role && game_connection() == _game_connection_network_server )
                    weapon_start_reload_to_network(weapon_index, magazine_index);

                /* weapon_state_primary_reload (5) for magazine 0, secondary_reload (6) for magazine 1 */
                weapon_set_state(weapon_index, weapon_state_primary_reload + magazine_index, 0);
                weapon_effect_new(weapon_index, magazine_definition->reloading_effect.index, 0.0f, 0.0f);
                first_person_weapon_message_from_weapon(weapon_index,
                    magazine->rounds_loaded != 0 ? _first_person_weapon_message_reload_while_full
                                                 : _first_person_weapon_message_reload_while_empty);

                if ( definition->weapon.weapon_type == _weapon_type_shotgun )
                {
                    int16_t capacity = magazine_definition->rounds_loaded_maximum;
                    if ( first_round )
                        reload_variant = (capacity - magazine->rounds_loaded != 1) ? 0 : 2;
                    else
                        reload_variant = (capacity - magazine->rounds_loaded != 1) ? -1 : 1;
                }

                magazine->state = _weapon_magazine_reloading;   /* reloading */
                int16_t animation_time = weapon_get_first_person_animation_time(weapon_index, 0,
                                             _first_person_weapon_animation_reload_while_empty,
                                             reload_variant);
                magazine->state_timer = animation_time;
                magazine->original_time = animation_time;
            }

            weapon->weapon.flags &= ~(1u << _weapon_needs_to_reload_bit);
        }
    }
}
