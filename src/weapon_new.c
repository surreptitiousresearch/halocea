/* weapon_new @0x836D85D0 — one-time runtime init for a newly placed/created weapon object: clears the
 * "in inventory" flag and resets its owning-unit slot reference, then for each of the weapon's magazines
 * fills it to (the tag-defined default rounds-loaded, clamped to total capacity) rounds loaded plus the
 * remainder as reserve, resets every trigger's autofire/rate-of-fire tracking state, and — if this is a
 * client or server game (game_connection() 1 or 2) — clears the weapon's network ownership/ready flags.
 * Always returns 1 (success). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_magazine_definition.h"
#include "headers/weapon_datum_state.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"


extern int16_t game_connection(void);

uint8_t weapon_new(int weapon_index)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;

    weapon->weapon.state = weapon_state_idle;
    weapon->weapon.overheated_effect_index = -1;

    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);
    int magazine_count = definition->weapon.magazines.count;

    for ( int i = 0; i < magazine_count; ++i )
    {
        weapon_magazine_definition *magazine_definition =
            &((weapon_magazine_definition *)definition->weapon.magazines.address)[i];
        weapon_magazine *magazine = &weapon->weapon.magazines[i];

        int16_t default_rounds_loaded = magazine_definition->rounds_total_initial;
        int16_t total_capacity = magazine_definition->rounds_loaded_maximum;
        int16_t loaded_rounds = default_rounds_loaded <= total_capacity ? default_rounds_loaded : total_capacity;

        magazine->rounds_loaded = loaded_rounds;
        magazine->rounds_total = default_rounds_loaded - loaded_rounds;
    }

    int trigger_count = definition->weapon.triggers.count;
    for ( int i = 0; i < trigger_count; ++i )
    {
        weapon_trigger *trigger = &weapon->weapon.triggers[i];
        trigger->charging_effect_index = -1;
        trigger->idle_ticks = 127;
        trigger->delay_ticks_before_empty_clip_auto_reload = 0;
    }

    int16_t connection = game_connection();
    if ( connection == _game_connection_network_client || connection == _game_connection_network_server )
    {
        weapon->weapon.baseline_valid = 0;
        weapon->weapon.baseline_index = 0;
        weapon->weapon.message_index = 0;
        weapon->object.was_network_at_rest = 0;
    }

    return 1;
}
