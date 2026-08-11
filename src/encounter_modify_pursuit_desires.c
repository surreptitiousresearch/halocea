/* encounter_modify_pursuit_desires @0x83709EA0 — adjusts a squad's pursuit-desire out-parameters based on
 * the encounter's default pursuit-search value (word +0x28, not otherwise modeled) — forced to 1 if the
 * squad's flags (squad_definition.flags) bit 0x2 is set. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/encounter_definition.h"
#include "headers/squad_definition.h"
#include "headers/squad_flags.h"
#include "headers/blam_data_globals.h"


void encounter_modify_pursuit_desires(int encounter_index, int16_t squad_index, uint8_t *pursue_tenacious, int16_t *group_pursuit_restriction, uint8_t *group_pursuit_controller, int16_t *desired_target_search, int16_t *desired_pursuit, int16_t *desired_pursuit_search)
{
    encounter_definition *encounter = (encounter_definition *)global_scenario->ai_encounters.address + encounter_index;
    squad_definition *squads = (squad_definition *)encounter->squads.address;

    int16_t pursuit_search = encounter->searching; /* recovered: *(__int16 *)((char *)encounter + 0x28) -> searching */
    if ( (squads[squad_index].flags & (1u << _squad_never_search_bit)) != 0 )
        pursuit_search = 1;

    /* pursuit_search / the desired_* out-param values below are a small AI search-level tri-state
     * (0/1/2). No DB enum pins encounter_definition.searching's value domain — searched
     * types_enum_values for %search%/%pursuit% (candidates _actor_pursuit_* 0/1/2 and _group_pursuit_*
     * do not cleanly cover these out-params). Left raw pending user decision. */
    if ( pursuit_search == 1 )
    {
        *group_pursuit_restriction = 1;
        *desired_pursuit = 2;
        *desired_pursuit_search = 2;
    }
    else if ( pursuit_search == 2 )
    {
        *pursue_tenacious = 1;
        *desired_target_search = 0;
        *desired_pursuit = 0;
        *desired_pursuit_search = 0;
        *group_pursuit_controller = 0;
    }
}
