/* encounter_attach_actor @0x8370DCD0 — link a freshly created actor into an encounter's squad/platoon structure.
 * Pushes the actor onto the encounter's actor list, resolves its squad and platoon (from the scenario encounter
 * definition), activates the encounter if the actor is active, sets active/dormant/team state, joins its unit to
 * the encounter, bumps squad/platoon/encounter living counts, and copies the platoon's defending flag. No-op if
 * AI is not initialised for the current map.
 *
 * Deviation: Hex-Rays' has_previous_team (the 4th arg) is the database name; callers pass 0. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/encounter_datum.h"
#include "headers/encounter_definition.h"
#include "headers/squad_definition.h"
#include "headers/ai_globals.h"
#include "headers/scenario.h"
#include "headers/squad_datum.h"
#include "headers/platoon_datum.h"
#include "headers/blam_data_globals.h"


extern uint8_t encounter_activate(int encounter_index);
extern int actor_set_active(uint16_t actor_index, uint8_t active);
extern void actor_set_dormant(uint16_t actor_index, uint8_t dormant);
extern void encounter_attach_unit(int encounter_index, int unit_index);
extern void actor_set_team(uint16_t actor_index, int16_t team_index);
extern void ai_update_team_status(void);

void encounter_attach_actor(int actor_index, int encounter_index, int16_t squad_index,
                            uint8_t has_previous_team)
{
    if ( !ai_globals->ai_initialized_for_map )
        return;

    encounter_datum *encounter = DATUM_GET(encounter_data, encounter_datum, encounter_index);
    encounter_definition *scenario_encounter = (encounter_definition *)global_scenario->ai_encounters.address
                             + (unsigned __int16)encounter_index;
    __int16 squad_base = encounter->squad_base;
    __int16 platoon_index = ((squad_definition *)scenario_encounter->squads.address + squad_index)->platoon_index;
    actor_datum *actor = DATUM_GET(actor_data, actor_datum, actor_index);
    squad_datum *squad = &squad_array[(__int16)(squad_base + squad_index)];

    actor->meta.disconnected_encounter_index = -1;
    actor->meta.disconnected_squad_index = -1;
    actor->meta.next_actor_index = encounter->first_actor_index;
    encounter->first_actor_index = actor_index;

    if ( platoon_index < 0 || platoon_index >= scenario_encounter->platoons.count )
        platoon_index = -1;

    int active = actor->meta.active;
    unsigned __int8 activated = 0;
    actor->meta.encounter_index = encounter_index;
    actor->meta.squad_index = squad_index;
    actor->meta.platoon_index = platoon_index;

    if ( active && !actor->meta.dormant )
    {
        encounter->remain_active_timer = 150;
        activated = encounter_activate(encounter_index);
    }
    if ( !activated )
    {
        actor_set_active(actor_index, encounter->active);
        if ( encounter->active )
            actor_set_dormant(actor_index, 0);
    }

    int unit_index = actor->meta.unit_index;
    if ( unit_index != -1 )
        encounter_attach_unit(encounter_index, unit_index);

    __int16 actor_team = actor->meta.team_index;
    __int16 encounter_team = encounter->team_index;
    if ( actor_team != encounter_team )
    {
        if ( !has_previous_team || encounter->current_count )
        {
            actor_set_team(actor_index, encounter_team);
        }
        else
        {
            encounter->team_index = actor_team;
            ai_update_team_status();
        }
    }

    ++encounter->original_count;
    ++squad->original_count;
    if ( actor->meta.unique_leader )
        ++encounter->unique_leader_count;

    if ( platoon_index != -1 )
    {
        platoon_datum *platoon = &platoon_array[(__int16)(encounter->platoon_base + platoon_index)];
        actor->external_orders.defending = platoon->defending;
        actor->emotions.currently_defending = platoon->defending;
        ++platoon->original_count;
    }
    encounter->status_dirty = 1;
}
