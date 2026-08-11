/* encounter_place_actor @0x8370BD08 — place one actor for a squad: pick a free starting location, resolve the
 * actor palette entry (the starting location may override the squad's default), roll the major-upgrade chance,
 * and hand off to actor_place. Returns true when an actor was successfully created. */

#include <stdint.h>
#include "headers/actor_definition.h"
#include "headers/tag_reference.h"
#include "headers/actor_palette_entry.h"
#include "headers/squad_definition.h"
#include "headers/encounter_definition.h"
#include "headers/scenario.h"
#include "headers/actor_starting_location_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"


extern int16_t encounter_get_actor_starting_location(int encounter_index, int squad_index, uint8_t spawn);
extern void ai_get_major_upgrade_chance(int16_t upgrade_type, uint8_t *force_major, uint8_t *is_random, float *random_chance);
extern uint8_t ai_consider_major_upgrade(int encounter_index, int16_t squad_index, float upgrade_chance);
extern int actor_place(int actor_variant_definition_index, int encounter_index, int16_t squad_index, actor_starting_location_definition *starting_location, uint8_t upgrade_major, int16_t variant_number);

/* Squad layout (stride 232): +32 actor palette index, +128 upgrade type, +212 starting-locations address
 * (each starting location 28 bytes). */

uint8_t encounter_place_actor(int encounter_index, int16_t squad_index, int16_t variant_number,
                              uint8_t spawn)
{
    squad_definition *squad =
        &((squad_definition *)((encounter_definition *)global_scenario->ai_encounters.address)[(uint16_t)encounter_index].squads.address)[squad_index];

    int16_t starting_location_index = encounter_get_actor_starting_location(encounter_index, squad_index, spawn);
    if ( starting_location_index == -1 )
        return 0;

    int16_t actor_palette_index = squad->actor_palette_index;
    actor_starting_location_definition *starting_locations =
        (actor_starting_location_definition *)squad->starting_locations.address;
    actor_starting_location_definition *starting_location = &starting_locations[starting_location_index];
    if ( (uint16_t)starting_location->actor_palette_index != 0xFFFF )
        actor_palette_index = starting_location->actor_palette_index;
    if ( actor_palette_index < 0 )
        return 0;
    if ( actor_palette_index >= global_scenario->ai_actor_palette.count )
        return 0;

    actor_palette_entry *palette_entry =
        &((actor_palette_entry *)global_scenario->ai_actor_palette.address)[actor_palette_index];
    int actor_definition_index = palette_entry->reference.index;
    if ( actor_definition_index == -1 )
        return 0;

    actor_definition *actor_def = TAG_GET(actor_definition, actor_definition_index);
    uint8_t upgrade_major = 0;
    if ( actor_def->perception.unused4 != -1 )  /* byte 48: an unused perception slot doubling as the has-major-upgrade sentinel */
    {
        uint8_t is_random = 0;
        int16_t upgrade_type = squad->major_upgrade;
        float random_chance = 0.0f;
        ai_get_major_upgrade_chance(upgrade_type, &upgrade_major, &is_random, &random_chance);
        if ( is_random )
            upgrade_major = ai_consider_major_upgrade(encounter_index, squad_index, random_chance);
    }

    return actor_place(palette_entry->reference.index, encounter_index, squad_index, starting_location,
                       upgrade_major, variant_number) != -1;
}
