/* actors_handle_spatial_effect @0x8371F808 — notify every actor not already in certain combat
 * (combat_status < _actor_combat_status_visible) of a spatial stimulus (weapon impact/detonation, or environmental noise when
 * effect_type is 0) at `position`, provided the actor can actually hear it there (audibility >= _ai_sound_volume_loud).
 *
 * DEVIATION: the decompiler shows `line_of_sight` (actor_audibility_at_point's 7th param) as an
 * uninitialized local; disasm confirms it's just the constant 0 (`li r9, 0`), not a real uninitialized
 * read — reproduced as the literal 0. */

#include <stdint.h>
#include "headers/actor_iterator.h"
#include "headers/actor_datum.h"
#include "headers/real_point3d.h"
#include "headers/location.h"
#include "headers/actor_position_data.h"
#include "headers/ai_sound_volume.h"
#include "headers/actor_combat_status.h"
#include "headers/ai_spatial_effect_type.h"

extern void scenario_location_from_point(location *location, const real_point3d *point);
extern void actor_perception_find_sense_position(int actor_index, const real_point3d *position, int prop_index, actor_position_data *sense_position);
extern int16_t actor_audibility_at_point(int actor_index, actor_position_data *sense_position, const real_point3d *position, const location *location, int16_t sound_volume, float perception_factor, int16_t line_of_sight);
extern void actor_stimulus_weapon_impact(int actor_index, int object_index, const real_point3d *position, int16_t count);
extern void actor_stimulus_weapon_detonation(int actor_index, int object_index, const real_point3d *position, int16_t count);
extern void actor_stimulus_environmental_noise(int actor_index, int object_index, const real_point3d *position, int16_t count);

void actors_handle_spatial_effect(int object_index, int16_t effect_type, const real_point3d *position, int16_t volume, int16_t count)
{
    location sound_location;
    scenario_location_from_point(&sound_location, position);

    actor_iterator iterator;
    actor_iterator_new(&iterator, 1u);

    for ( actor_datum *actor = actor_iterator_next(&iterator); actor; actor = actor_iterator_next(&iterator) )
    {
        if ( actor->state.combat_status >= _actor_combat_status_visible )
            continue;

        actor_position_data sense_position;
        actor_perception_find_sense_position(iterator.index, position, -1, &sense_position);

        if ( actor_audibility_at_point(iterator.index, &sense_position, position, &sound_location, volume, 1.0f, 0) < _ai_sound_volume_loud )
            continue;

        if ( effect_type == _ai_spatial_effect_environmental_noise )
            actor_stimulus_environmental_noise(iterator.index, object_index, position, count);
        else if ( effect_type == _ai_spatial_effect_weapon_impact )
            actor_stimulus_weapon_impact(iterator.index, object_index, position, count);
        else if ( (unsigned int)effect_type < NUMBER_OF_AI_SPATIAL_EFFECTS )
            actor_stimulus_weapon_detonation(iterator.index, object_index, position, count);
    }
}
