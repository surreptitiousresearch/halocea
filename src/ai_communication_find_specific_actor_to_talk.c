/* ai_communication_find_specific_actor_to_talk @0x837CE478 — like ai_communication_find_global_actor_to_talk,
 * but restricted to the actors belonging to one AI index (encounter/squad/platoon). Picks the actor with the
 * highest talk weight, or -1.
 *
 * Deviation: the trailing __int16 arguments to ai_communication_actor_talk_weight are mis-attributed to phantom
 * locals by the decompiler (PPC float-arg GPR-skip); the real arguments are passed here. */

#include <stdint.h>
#include "headers/ai_index_actor_iterator.h"
#include "headers/real_point3d.h"

#include "headers/actor_datum.h"
extern void ai_index_actor_iterator_new(unsigned int ai_index, ai_index_actor_iterator *iterator);
extern actor_datum *ai_index_actor_iterator_next(ai_index_actor_iterator *iterator);
extern void unit_get_head_position(int unit_index, real_point3d *head_position);
extern float ai_communication_actor_talk_weight(int actor_index, int subject_unit_index, real_point3d *subject_point, int cause_unit_index, real_point3d *cause_point, float stimulus_range, int16_t ai_communication_type, int16_t ai_communication_priority, int16_t unit_speech_priority, int16_t vocalization_type, int16_t animation_type, int16_t flags);

int ai_communication_find_specific_actor_to_talk(int ai_index, int subject_unit_index, int cause_unit_index,
        float max_distance, int16_t ai_communication_type, int16_t ai_communication_priority,
        int16_t unit_speech_priority, int16_t vocalization_type, int16_t animation_type, int16_t flags)
{
    float best_weight = 0.0f;
    int best_index = -1;
    if ( ai_index == -1 )
        return -1;

    real_point3d subject_point, cause_point;
    if ( subject_unit_index != -1 )
        unit_get_head_position(subject_unit_index, &subject_point);
    if ( cause_unit_index != -1 )
        unit_get_head_position(cause_unit_index, &cause_point);

    ai_index_actor_iterator actors;
    ai_index_actor_iterator_new(ai_index, &actors);
    while ( ai_index_actor_iterator_next(&actors) )
    {
        float weight = ai_communication_actor_talk_weight(actors.iterator.index, subject_unit_index,
                            &subject_point, cause_unit_index, &cause_point, max_distance, ai_communication_type,
                            ai_communication_priority, unit_speech_priority, vocalization_type, animation_type, flags);
        if ( weight > best_weight )
        {
            best_index = actors.iterator.index;
            best_weight = weight;
        }
    }
    return best_index;
}
