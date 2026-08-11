/* actor_get_vision_distances @0x837D5598 — given a bearing (horizontal_angle) to a target, compute the actor's
 * two sight-distance thresholds at that bearing by interpolating the character's central/peripheral vision
 * cones (character tag floats [7]..[11]). Outside the peripheral cone both are 0; within the central cone they
 * are the full distances; between, they blend toward the peripheral distance.
 *
 * Deviation: the database prototype invents phantom params from the three float args' reserved GPR slots; the
 * real output pointers are the two trailing GPR slots — r7 = full_distance_reference and r8 =
 * partial_distance_reference — confirmed by disassembly. Note the value written to full_distance_reference is
 * the 0.7x-reduced threshold and partial_distance_reference receives the larger max*perception threshold — the
 * parameter names are as the database labels them even though the magnitudes read "swapped". */

#include "headers/actor_definition.h"
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>


void actor_get_vision_distances(int actor_index, float maximum_vision_distance,
                                float perception_factor, float horizontal_angle,
                                float *full_distance_reference, float *partial_distance_reference)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_definition *character = TAG_GET(actor_definition, actor->meta.definition_index);

    if ( horizontal_angle > character->perception.peripheral_vision_angle )  /* beyond peripheral cone */
    {
        *partial_distance_reference = 0.0f;
        *full_distance_reference = 0.0f;
        return;
    }

    float reduced = (character->perception.peripheral_distance * perception_factor) * 0.69999999f;
    float full_reduced = (maximum_vision_distance * perception_factor) * 0.69999999f;
    if ( reduced > 3.5f )
        reduced = 3.5f;

    float peripheral_angle = character->perception.maximum_vision_angle;
    if ( horizontal_angle > peripheral_angle )
    {
        *partial_distance_reference = character->perception.peripheral_distance * perception_factor;
        *full_distance_reference = reduced;
        return;
    }

    float central_angle = character->perception.central_vision_angle;
    float central_partial_angle = character->perception.central_vision_angle * 0.80000001f;
    float partial_value;
    if ( horizontal_angle >= central_angle )
    {
        float t = (horizontal_angle - central_angle) / (peripheral_angle - central_angle);
        partial_value = ((1.0f - t) * (maximum_vision_distance * perception_factor))
                      + (t * (character->perception.peripheral_distance * perception_factor));
    }
    else
    {
        partial_value = maximum_vision_distance * perception_factor;
    }
    *partial_distance_reference = partial_value;

    if ( horizontal_angle >= central_partial_angle )
    {
        float t = (horizontal_angle - central_partial_angle) / (peripheral_angle - central_partial_angle);
        full_reduced = ((1.0f - t) * ((maximum_vision_distance * perception_factor) * 0.69999999f))
                     + (t * reduced);
    }
    *full_distance_reference = full_reduced;
}
