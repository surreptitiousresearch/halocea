/* action_uncover_begin @0x83824938 — start the "uncover" action: picks a random duration (30Hz ticks)
 * between the actor's unit-definition uncover-time bounds (definition floats +828/+832), clamped up to a
 * pair of minimum-override bounds (+836/+840) unless the action's own "override" flag (resolved to the
 * uncover arm's able_to_search field) is set, and stores it as both the countdown timer and its original duration (action_data+40/+44).
 * If a "notify" flag (resolved to the uncover arm's pursuit_location.type word) is clear and the actor has an associated uncover-target prop
 * (actor->target.target_prop_index) while not yet in certain combat (combat_status < _actor_combat_status_definite), fires an AI communication
 * event (type 21) naming the actor's own unit and the target prop's unit as subject/cause. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/prop_datum.h"
#include "headers/actor_definition.h"
#include "headers/unit_speech_item.h"
#include "headers/actor_combat_status.h"
#include "headers/ai_communication_type.h"
#include "headers/blam_data_globals.h"


#include "headers/ai_information_data.h"
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);

void action_uncover_begin(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_definition *actor_def = TAG_GET(actor_definition, actor->meta.definition_index);

    float min_duration = actor_def->pursuit.uncover_delay_time_lower_bound;
    float max_duration = actor_def->pursuit.uncover_delay_time_upper_bound;

    if ( !actor->state.action_data.___u0.uncover.able_to_search )
    {
        if ( min_duration <= actor_def->pursuit.target_location_time_lower_bound )
            min_duration = actor_def->pursuit.target_location_time_lower_bound;
        if ( max_duration <= actor_def->pursuit.target_location_time_upper_bound )
            max_duration = actor_def->pursuit.target_location_time_upper_bound;
    }

    float duration_seconds = real_seed_random_range(get_global_random_seed_address(), min_duration, max_duration);
    uint16_t notify_suppressed = (uint16_t)actor->state.action_data.___u0.uncover.pursuit_location.type;

    int duration_ticks = (int)(duration_seconds * 30.0f);
    actor->state.action_data.___u0.uncover.uncover_desired_time = duration_ticks;
    actor->state.action_data.___u0.uncover.uncover_remaining_time = duration_ticks;

    if ( !notify_suppressed && actor->target.target_prop_index != -1 && actor->state.combat_status < _actor_combat_status_definite )
    {
        prop_datum *target_prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, actor->target.target_prop_index);
        ai_communication_event(_ai_communication_uncover_start, actor->meta.unit_index, target_prop->unit_index, -1, -1, -1, nullptr);
    }
}
