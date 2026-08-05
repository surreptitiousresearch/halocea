/* encounter_squad_timer_expire @0x8370C278 — expire a squad's delay timer (clear it to 0). If the squad
 * definition has the "magically aware on timer" flag (0x10), all of its actors are also made instantly
 * aware of the players. */

#include <stdint.h>
#include "headers/squad_definition.h"
#include "headers/squad_flags.h"
#include "headers/encounter_definition.h"
#include "headers/data_array.h"
#include "headers/encounter_datum.h"
#include "headers/scenario.h"
#include "headers/squad_datum.h"
#include "headers/blam_data_globals.h"

extern void ai_scripting_magically_see_players(int ai_index);

void encounter_squad_timer_expire(uint16_t encounter_index, int16_t squad_index)
{
    /* encounter def squads block (element 232 bytes); flags @0x28, bit 0x10 = "magically aware on timer". */
    squad_definition *squad_def =
        &((squad_definition *)((encounter_definition *)global_scenario->ai_encounters.address)[encounter_index].squads.address)[squad_index];
    squad_array[(int16_t)(DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index)->squad_base + squad_index)].delay_timer = 0;
    if ( (squad_def->flags & (1u << _squad_magic_sight_after_timer_bit)) != 0 )
        ai_scripting_magically_see_players((squad_index << 16) & 0xFF0000 | 0x80000000 | encounter_index);
}
