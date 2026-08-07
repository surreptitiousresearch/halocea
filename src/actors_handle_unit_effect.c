/* actors_handle_unit_effect @0x83720958 — propagate a unit's "effect" (e.g. a noise the unit just made) to
 * every nearby AI actor that can hear it. Builds a bitset of structure-BSP clusters that are within audible
 * range (encoded sound distance * 2.015748 < 40 world units) of the emitting unit's cluster, then walks all
 * active actors: for any actor in an audible cluster (and not the emitter itself), it checks line-of-sound
 * audibility from both the unit's origin and — if the actor already has a prop for the unit — that prop's
 * remembered position, and on success raises the effect on the actor (actor_handle_unit_effect).
 *
 * Deviation: the decompiler treats the object data as a location[] array; rendered here as typed unit_datum
 * access (object.location, unit.swarm_actor_index falling back to unit.actor_index).
 * actor_audibility_at_point's 7th argument (line_of_sight) is a real
 * argument the decompiler left as an uninitialized phantom (float perception_factor GPR-skip): per disasm it
 * is 0 for the origin test and prop->line_of_sight (prop+0x38) for the prop test. */

#include <stdint.h>
#include <string.h>
#include "headers/ai_sound_volume.h"
#include "headers/encoded_sound_distance.h"
#include "headers/bit_vector.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/structure_bsp.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/actor_iterator.h"
#include "headers/actor_datum.h"
#include "headers/actor_position_data.h"
#include "headers/unit_datum.h"
#include "headers/prop_datum.h"
#include "headers/blam_data_globals.h"

extern int object_get_ultimate_parent(int object_index);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern uint8_t structure_bsp_get_cluster_encoded_sound_distance(structure_bsp *structure_bsp, int16_t from_cluster_index, int16_t to_cluster_index);
extern void actor_perception_find_sense_position(uint16_t actor_index, const real_point3d *position, int prop_index, actor_position_data *sense_position);
extern int16_t actor_audibility_at_point(uint16_t actor_index, actor_position_data *sense_position, const real_point3d *position, const location *location, int16_t sound_volume, float perception_factor, int16_t line_of_sight);
extern int prop_get_base_by_unit_index(int actor_index, int unit_index, uint8_t acquire, uint8_t refresh_upon_acquisition);
extern void actor_handle_unit_effect(int actor_index, int prop_index, int16_t effect_type);

void actors_handle_unit_effect(int owner_unit_index, int16_t effect_type, int16_t volume)
{
    unit_datum *owner_unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, owner_unit_index)->datum);

    const location *emitter_location = &owner_unit->object.location;
    int owner_actor_index = owner_unit->unit.swarm_actor_index;
    if ( owner_actor_index == -1 )
        owner_actor_index = owner_unit->unit.actor_index;

    /* sound emanates from the ultimate parent's cluster when the unit is mounted/attached */
    if ( owner_unit->object.parent_object_index != -1 )
    {
        int ultimate_parent = object_get_ultimate_parent(owner_unit_index);
        unit_datum *parent = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, ultimate_parent)->datum);
        emitter_location = &parent->object.location;
    }

    structure_bsp *bsp = global_structure_bsp;
    int cluster_count = global_structure_bsp->clusters.count;

    unsigned int audible_clusters[18];
    memset(audible_clusters, 0, 4 * BIT_VECTOR_SIZE_IN_LONGS(cluster_count));

    if ( (uint16_t)emitter_location->cluster_index != 0xFFFF && cluster_count > 0 )
    {
        int16_t to_cluster = 0;
        do
        {
            uint8_t encoded = structure_bsp_get_cluster_encoded_sound_distance(
                                          bsp, emitter_location->cluster_index, to_cluster);
            if ( !ENCODED_SOUND_DISTANCE_HAS_NO_PATH(encoded) )
            {
                if ( (float)ENCODED_SOUND_DISTANCE_STEPS(encoded) * ENCODED_SOUND_DISTANCE_WORLD_UNITS_PER_STEP < 40.0f )
                    BIT_VECTOR_SET_FLAG(audible_clusters, to_cluster);
            }
            to_cluster = (int16_t)(to_cluster + 1);
        }
        while ( to_cluster < bsp->clusters.count );
    }

    real_point3d origin;
    object_get_origin(owner_unit_index, &origin);

    actor_iterator iterator;
    actor_iterator_new(&iterator, 1u);
    for ( actor_datum *actor = actor_iterator_next(&iterator); actor; actor = actor_iterator_next(&iterator) )
    {
        if ( iterator.index == owner_actor_index )
            continue;

        /* recovered: raw actor+0x148 -> input.position.body_location.cluster_index */
        uint16_t actor_cluster = (uint16_t)actor->input.position.body_location.cluster_index;
        if ( actor_cluster == 0xFFFF
          || !BIT_VECTOR_TEST_FLAG(audible_clusters, (int16_t)actor_cluster) )
            continue;

        actor_position_data sense_position;
        actor_perception_find_sense_position(iterator.index, &origin, -1, &sense_position);
        if ( actor_audibility_at_point(iterator.index, &sense_position, &origin, emitter_location,
                                       volume, 1.0f, 0) < _ai_sound_volume_loud )
            continue;

        int prop_index = prop_get_base_by_unit_index(iterator.index, owner_unit_index, 1, 1);
        if ( prop_index == -1 )
            continue;

        prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
        if ( actor_audibility_at_point(iterator.index, &sense_position,
                                       &prop->body_position, &prop->body_location,
                                       volume, 1.0f, prop->line_of_sight) >= _ai_sound_volume_loud )
        {
            actor_handle_unit_effect(iterator.index, prop_index, effect_type);
        }
    }
}
