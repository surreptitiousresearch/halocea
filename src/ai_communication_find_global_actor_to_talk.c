/* ai_communication_find_global_actor_to_talk @0x837CE578 — across all active actors, pick the best-suited one
 * (highest talk weight) to voice a communication. find_actor_mode selects the team relation required relative
 * to team_index: 0 = same team, 1 = not an enemy, 2 = an enemy; team_index == -1 accepts any actor. Returns the
 * chosen actor index, or -1.
 *
 * Deviations: the database call to ai_communication_actor_talk_weight has its trailing __int16 arguments
 * mis-attributed to phantom locals (a float param consumes a GPR slot per the PPC ABI); the real arguments
 * (ai_communication_type / vocalization_type / animation_type / flags) are passed here. The decompiler also
 * sampled the cause head position from subject_unit_index — corrected to cause_unit_index. */

#include <stdint.h>
#include "headers/actor_iterator.h"
#include "headers/actor_datum.h"
#include "headers/real_point3d.h"

extern void actor_iterator_new(actor_iterator *iterator, uint8_t active_only);
extern actor_datum *actor_iterator_next(actor_iterator *iterator);
extern void unit_get_head_position(int unit_index, real_point3d *head_position);
extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);
extern float ai_communication_actor_talk_weight(int actor_index, int subject_unit_index, real_point3d *subject_point, int cause_unit_index, real_point3d *cause_point, float stimulus_range, int16_t ai_communication_type, int16_t ai_communication_priority, int16_t unit_speech_priority, int16_t vocalization_type, int16_t animation_type, int16_t flags);

int ai_communication_find_global_actor_to_talk(int16_t team_index, int16_t find_actor_mode,
        int subject_unit_index, int cause_unit_index, float max_distance, int16_t ai_communication_type,
        int16_t ai_communication_priority, int16_t unit_speech_priority, int16_t vocalization_type,
        int16_t animation_type, int16_t flags)
{
    float best_weight = 0.0f;
    int best_index = -1;

    real_point3d subject_point, cause_point;
    if ( subject_unit_index != -1 )
        unit_get_head_position(subject_unit_index, &subject_point);
    if ( cause_unit_index != -1 )
        unit_get_head_position(cause_unit_index, &cause_point);

    actor_iterator actors;
    actor_iterator_new(&actors, 1u);
    for ( actor_datum *actor = actor_iterator_next(&actors); actor; actor = actor_iterator_next(&actors) )
    {
        char eligible;
        if ( team_index == -1 )
        {
            eligible = 1;
        }
        else
        {
            uint8_t is_enemy = game_team_is_enemy(team_index, actor->meta.team_index);
            if ( find_actor_mode == 1 )
                eligible = (is_enemy == 0);
            else if ( find_actor_mode )
                eligible = is_enemy;
            else
                eligible = (team_index == actor->meta.team_index);
        }
        if ( !eligible )
            continue;

        float weight = ai_communication_actor_talk_weight(actors.index, subject_unit_index, &subject_point,
                            cause_unit_index, &cause_point, max_distance, ai_communication_type,
                            ai_communication_priority, unit_speech_priority, vocalization_type, animation_type, flags);
        if ( weight > best_weight )
        {
            best_index = actors.index;
            best_weight = weight;
        }
    }
    return best_index;
}
