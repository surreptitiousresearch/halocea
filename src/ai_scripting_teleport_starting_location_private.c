/* ai_scripting_teleport_starting_location_private @0x83771260 — teleport every actor under an AI index back
 * to its squad's designated starting location. When only_if_unsupported is set, an actor is skipped unless
 * it is on foot (no vehicle) AND has no valid ground surface beneath it (i.e. it has fallen out of the
 * world). For each eligible actor the routine reads the squad's starting-location entry, derives an
 * orientation from its facing angle, repositions and resets the object, and halts the actor's movement. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/actor_datum.h"
#include "headers/ai_index_actor_iterator.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/encounter_definition.h"
#include "headers/squad_definition.h"
#include "headers/actor_starting_location_definition.h"
#include "headers/blam_data_globals.h"

#include "headers/real_point3d.h"
extern void ai_index_actor_iterator_new(unsigned int ai_index, ai_index_actor_iterator *iterator);
extern actor_datum *ai_index_actor_iterator_next(ai_index_actor_iterator *iterator);
extern int biped_approximate_surface_index(int biped_index, real_point3d *surface_point);
extern int16_t encounter_get_actor_starting_location(uint16_t encounter_index, int squad_index, uint8_t spawn);
extern real_vector3d *vector3d_from_angle(real_vector3d *vector, float angle);
extern void object_set_position(int object_index, const real_point3d *position, const real_vector3d *forward, const real_vector3d *up);
extern void object_reset(int object_index);
extern uint8_t actor_move_halt(uint16_t actor_index);

void ai_scripting_teleport_starting_location_private(int ai_index, uint8_t only_if_unsupported)
{
    ai_index_actor_iterator iterator;
    ai_index_actor_iterator_new(ai_index, &iterator);

    for ( actor_datum *actor = ai_index_actor_iterator_next(&iterator);
          actor;
          actor = ai_index_actor_iterator_next(&iterator) )
    {
        int unit_index = actor->meta.unit_index;
        if ( unit_index == -1 )
            continue;
        if ( only_if_unsupported
          && !(actor->input.vehicle_index == -1 && biped_approximate_surface_index(unit_index, nullptr) == -1) )
            continue;
        if ( actor->meta.encounter_index == -1 )
            continue;

        int squad_index = actor->meta.squad_index;
        encounter_definition *encounter =
            &((encounter_definition *)global_scenario->ai_encounters.address)[(unsigned __int16)actor->meta.encounter_index];
        squad_definition *squad = &((squad_definition *)encounter->squads.address)[squad_index];

        __int16 starting_location = encounter_get_actor_starting_location(actor->meta.encounter_index, squad_index, 1u);
        if ( starting_location != -1 )
        {
            const actor_starting_location_definition *location =
                &((const actor_starting_location_definition *)squad->starting_locations.address)[starting_location];
            real_vector3d forward;
            vector3d_from_angle(&forward, location->facing);
            object_set_position(actor->meta.unit_index, &location->position, &forward, nullptr);
            object_reset(actor->meta.unit_index);
            actor_move_halt(iterator.iterator.index);
        }
    }
}
