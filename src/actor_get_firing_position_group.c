/* actor_get_firing_position_group 0x837EF980 — return the scenario firing-position-group index
 * appropriate for the given actor and selection criteria. The group is read from the actor's squad
 * entry (stride 232) in the scenario encounter table (stride 176; squads pointer at encounter+132).
 * evaluation_mode selects which of the six squad group slots to read; group_selection_mode 1/2
 * forces the searching flag on or off (overriding the actor's actual searching state). Returns 0
 * when the actor has no encounter. */

#include "headers/squad_definition.h"
#include "headers/encounter_definition.h"
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/scenario.h"
#include "headers/firing_point_evaluation_mode.h"
#include "headers/firing_position_group_selection.h"
#include "headers/firing_position_group.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>


unsigned int actor_get_firing_position_group(uint16_t actor_index,
                                             int16_t evaluation_mode,
                                             int16_t group_selection_mode)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->meta.encounter_index == -1 )
        return 0;

    /* recovered: raw encounter+132 -> encounter_definition.squads.address (tag_block at 128, DB) */
    encounter_definition *encounter = (encounter_definition *)global_scenario->ai_encounters.address
                                      + (unsigned __int16)actor->meta.encounter_index;
    squad_definition *squad = (squad_definition *)encounter->squads.address + actor->meta.squad_index;

    unsigned __int8 searching = actor->state.searching;

    if ( group_selection_mode == _firing_position_group_when_searching )
        searching = 1;
    else if ( group_selection_mode == _firing_position_group_when_not_searching )
        searching = 0;

    /* recovered: raw actor+0x374 -> emotions.currently_defending (DB); the shipped code uses this
     * emotion byte as the vehicle/on-foot firing-group discriminator */
    unsigned __int8 currently_defending = actor->emotions.currently_defending;

    switch ( evaluation_mode )
    {
    case _firing_point_evaluation_mode_panic:
        return squad->firing_position_groups[_firing_position_group_defending_guard];
    case _firing_point_evaluation_mode_guard:
        /* DEVIATION: decompiler raw byte offsets 84..104 lifted to firing_position_groups[] (base 0x54) */
        return squad->firing_position_groups[currently_defending ? _firing_position_group_defending_guard : _firing_position_group_attacking_guard];
    case _firing_point_evaluation_mode_pursue:
        return squad->firing_position_groups[_firing_position_group_pursuing];
    default:
        if ( currently_defending )
            return squad->firing_position_groups[searching ? _firing_position_group_defending_search : _firing_position_group_defending];
        else
            return squad->firing_position_groups[searching ? _firing_position_group_attacking_search : _firing_position_group_attacking];
    }
}
