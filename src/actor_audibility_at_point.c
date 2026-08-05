/* actor_audibility_at_point @0x837D56A0 — decide how well the actor can hear a sound of a given volume emitted
 * at `position`/`location`. Starts from the character's hearing range, attenuated by facing (sounds behind the
 * actor are quieter), the actor's alertness, the sound volume class, deafening clusters and a reduced
 * line-of-sight factor. Returns 0 if inaudible (out of range or BSP sound-path too long), else 2 or 3
 * depending on volume.
 *
 * Deviation: the database's float param (perception_factor) is unused; Hex-Rays invented a trailing a8 from its
 * reserved GPR slot. The value actually consumed (decompiler's a8) is the real line_of_sight argument. */

#include <stdint.h>
#include "headers/actor_definition.h"
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_position_data.h"
#include "headers/real_point3d.h"
#include "headers/location.h"
#include "headers/structure_bsp.h"
#include "headers/ai_sound_volume.h"
#include "headers/actor_mode.h"
#include "headers/ai_line_of_sight.h"
#include "headers/encoded_sound_distance.h"
#include "headers/blam_data_globals.h"
#include "headers/structure_bsp.h"
extern float __fsqrts(float);

extern uint8_t scenario_location_deafening(const location *location);
extern uint8_t structure_bsp_get_cluster_encoded_sound_distance(structure_bsp *structure_bsp, int16_t from_cluster_index, int16_t to_cluster_index);

int16_t actor_audibility_at_point(uint16_t actor_index, actor_position_data *sense_position,
                              const real_point3d *position, const location *location, int16_t sound_volume,
                              float perception_factor, int16_t line_of_sight)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( !sound_volume
      || (uint16_t)sense_position->body_location.cluster_index == 0xFFFF
      || (uint16_t)location->cluster_index == 0xFFFF )
    {
        return 0;
    }

    float dy = position->n[1] - sense_position->head_position.n[1];
    float dz = position->n[2] - sense_position->head_position.n[2];
    float dx = position->n[0] - sense_position->head_position.n[0];
    actor_definition *character = TAG_GET(actor_definition, actor->meta.definition_index);
    float range = character->perception.hearing_distance;
    float distance_squared = (dy * dy) + ((dx * dx) + (dz * dz));

    /* sound coming from behind the actor's facing is heard at 80% range */
    if ( (sense_position->body_facing.n[0] * dx)
           + ((sense_position->body_facing.n[2] * dz) + (sense_position->body_facing.n[1] * dy)) < 0.0f )
        range = character->perception.hearing_distance * 0.80000001f;

    int alertness = actor->state.mode;
    if ( alertness == _actor_mode_alert )
        range = range * 0.69999999f;
    else if ( alertness == _actor_mode_asleep )
        range = range * 0.40000001f;

    switch ( sound_volume )
    {
        case _ai_sound_volume_quiet: range = range * 0.2f; break;
        case _ai_sound_volume_medium: range = range * 0.44999999f; break;
        case _ai_sound_volume_shout: range = range * 0.69999999f; break;
    }

    if ( scenario_location_deafening(&sense_position->body_location) || scenario_location_deafening(location) )
        range = range * 0.25f;
    if ( line_of_sight && line_of_sight != _ai_line_of_sight_occluded )
        range = range * 0.69999999f;

    if ( distance_squared >= (range * range) )
        return 0;

    uint8_t encoded = structure_bsp_get_cluster_encoded_sound_distance(
        global_structure_bsp, location->cluster_index, sense_position->body_location.cluster_index);
    if ( ENCODED_SOUND_DISTANCE_HAS_NO_PATH(encoded) )
        return 0;

    /* deviation: __SPAIR64__ is the decompiler's int->double widening; the value is the encoded steps as float */
    float sound_path = (float)ENCODED_SOUND_DISTANCE_STEPS(encoded) * ENCODED_SOUND_DISTANCE_WORLD_UNITS_PER_STEP * 2.0f;
    if ( sound_path <= __fsqrts(distance_squared) )
        sound_path = __fsqrts(distance_squared);
    if ( sound_path >= range )
        return 0;

    if ( sound_volume < _ai_sound_volume_shout )
        return _ai_sound_volume_loud;
    return _ai_sound_volume_shout;
}
