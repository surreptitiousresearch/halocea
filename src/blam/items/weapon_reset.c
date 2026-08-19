/* weapon_reset @0x836DCB98 — reset all of a weapon's trigger and magazine runtime state.
 *
 * Each trigger has its state forced to 8 and its state_timer cleared. Each magazine that is mid-reload
 * (state == 1) and whose elapsed reload time has not yet reached the first-person reload animation length
 * is finished immediately, then every magazine's reload state and timer are cleared. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_trigger_state.h"
#include "headers/weapon_magazine_state.h"
#include "headers/first_person_weapon_animation.h"
#include "headers/blam_data_globals.h"

extern uint16_t weapon_get_first_person_animation_time(int weapon_index, int16_t mode, int16_t animation_type, int16_t shotgun_reload_type);
extern void weapon_magazine_finish_reload(int weapon_index, int16_t magazine_index);

void weapon_reset(int weapon_index)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);

    int trigger_count = definition->weapon.triggers.count;
    for ( int16_t trigger = 0; trigger < trigger_count; trigger++ )
    {
        weapon->weapon.triggers[trigger].state = _weapon_trigger_ready;
        weapon->weapon.triggers[trigger].state_timer = 0;
    }

    int magazine_count = definition->weapon.magazines.count;
    for ( int16_t magazine = 0; magazine < magazine_count; magazine++ )
    {
        weapon_magazine *mag = &weapon->weapon.magazines[magazine];
        if ( mag->state == _weapon_magazine_reloading
             && 2 * mag->state_timer < weapon_get_first_person_animation_time(weapon_index, 0,
                    _first_person_weapon_animation_reload_while_empty, -1) )
        {
            weapon_magazine_finish_reload(weapon_index, magazine);
        }
        mag->state = _weapon_magazine_idle;
        mag->state_timer = 0;
    }
}
