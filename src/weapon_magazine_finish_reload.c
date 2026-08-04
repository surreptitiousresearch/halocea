/* weapon_magazine_finish_reload @0x836DC068 — complete one reload step for a weapon magazine: move rounds
 * from the reserve into the loaded count, update ammo bookkeeping, and either mark the reload finished or
 * chain another reload step if more rounds are still needed.
 *
 * The amount actually loaded is clamped to both the rounds available and rounds_loaded_maximum;
 * networked (authoritative) weapons (datum_role == 1) draw the reserve from
 * magazine_rounds_total_to_use_at_reload_finish. Infinite-ammo cheat and item.flags bit 1 skip the
 * reserve deduction. (DB note: magazines[].rounds_total is the reserve, rounds_loaded the clip.) */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/cheats.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_magazine_definition.h"
#include "headers/networked_datum_role.h"
#include "headers/object_flags.h"
#include "headers/weapon_magazine_state.h"
#include "headers/weapon_magazine_flags.h"
#include "headers/weapon_control_flags.h"
#include "headers/item_flags.h"
#include "headers/blam_data_globals.h"

extern uint8_t game_engine_running(void);
extern void weapon_magazine_start_reload(int weapon_index, int16_t magazine_index, uint8_t first_round);

void weapon_magazine_finish_reload(int weapon_index, int16_t magazine_index)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_magazine *magazine = &weapon->weapon.magazines[magazine_index];
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);
    weapon_magazine_definition *magazine_definition =
        &((weapon_magazine_definition *)definition->weapon.magazines.address)[magazine_index];

    if ( (magazine_definition->flags & (1u << _weapon_magazine_wastes_rounds_when_reloaded_bit)) != 0 )        /* "discard remaining" magazine: clear clip */
        magazine->rounds_loaded = 0;

    int16_t rounds_available;
    if ( weapon->object.datum_role == _networked_datum_puppet )               /* networked authoritative */
        rounds_available = weapon->weapon.magazine_rounds_total_to_use_at_reload_finish[magazine_index];
    else
        rounds_available = magazine->rounds_total;

    int16_t rounds_to_load = magazine_definition->rounds_reloaded;
    if ( rounds_to_load > rounds_available )
        rounds_to_load = rounds_available;

    int16_t new_loaded = (int16_t)(magazine->rounds_loaded + rounds_to_load);
    if ( new_loaded > magazine_definition->rounds_loaded_maximum )
        new_loaded = magazine_definition->rounds_loaded_maximum;

    int16_t remaining_reserve;
    if ( game_engine_running() )
    {
        remaining_reserve = magazine->rounds_total;
        if ( !cheat.infinite_ammo )
        {
            if ( weapon->object.datum_role == _networked_datum_puppet )
                remaining_reserve = rounds_available - new_loaded + magazine->rounds_loaded;
            else
            {
                remaining_reserve += magazine->rounds_loaded - new_loaded;
                magazine->rounds_total = remaining_reserve;
            }
        }
    }
    else
    {
        if ( !cheat.infinite_ammo && (weapon->item.flags & (1u << _item_belongs_to_player_bit)) != 0 )
            magazine->rounds_total += magazine->rounds_loaded - new_loaded;
        remaining_reserve = magazine->rounds_total;
    }

    magazine->rounds_loaded = new_loaded;
    magazine->state_timer = 0;
    magazine->state = _weapon_magazine_reloaded;

    if ( remaining_reserve <= 0
         || new_loaded >= magazine_definition->rounds_loaded_maximum
         || (magazine_definition->flags & (1u << _weapon_magazine_wastes_rounds_when_reloaded_bit)) != 0
         || (weapon->weapon.control_flags
             & ((1u << _weapon_control_primary_trigger_bit)
              | (1u << _weapon_control_secondary_trigger_bit)
              | (1u << _weapon_control_user_switching_weapons_bit))) != 0 )
    {
        weapon->object.flags |= (1u << _object_force_baseline_update_bit);              /* reload complete */
    }
    else
    {
        weapon_magazine_start_reload(weapon_index, magazine_index, 0);   /* load another round group */
    }
}
