/* post_evaluator_pursuit @0x837EF220 — firing-position post-evaluator for the "pursue" behavior. For a
 * candidate firing position it records/queries the actor's pursuit-examination history for this position in the
 * encounter: if the actor has line of sight (or is far enough along the path, >= 6 units) it queries whether
 * the position was already examined (recovering the examining-actor count and last-examined time); otherwise it
 * marks the position examined now. A "fresh" position (not previously examined) gets a +15 tenacity bonus when
 * the pursuit is tenacious, or is rejected (and invalidated unless rejected positions are allowed) when it is
 * not. Valid positions then receive a recency bonus (up to +10 for stale/never-seen, scaled by elapsed ticks /
 * 30 otherwise) and a crowding bonus of 5 per examining slot below 4. Returns firing_position->valid.
 *
 * DEVIATION: the DB int64/BYTE3 register puns are resolved from disasm — the return is firing_position->valid
 * (the byte at 0x30), the examining-actor count and last-examined time are plain out-params (var_50 int16,
 * var_48 int), and the int64 stores at 0x837EF378/0x3C0 are just int->double conversion scratch. Actor
 * encounter index is actor+0x34. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/firing_position.h"
#include "headers/firing_position_evaluation_context.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"


extern int game_time_get(void);
extern uint8_t encounter_mark_examined_pursuit_position(int encounter_index, int actor_index, int16_t firing_position_index, int history_start_time);
extern uint8_t encounter_pursuit_position_already_examined(int encounter_index, int actor_index, int16_t firing_position_index, int history_start_time, int16_t *actor_count_reference, int *last_examined_time_reference);

int post_evaluator_pursuit(int actor_index, firing_position_evaluation_context *evaluation_context, firing_position *firing_position)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int current_time = game_time_get();

    int16_t examining_actor_count = 0;
    int last_examined_time = -1;

    if ( !firing_position )
        return 0;

    int encounter_index = actor->meta.encounter_index;
    int candidate_time;
    uint8_t fresh;

    if ( firing_position->line_of_sight || firing_position->path_distance_from_actor >= 6.0f )
    {
        uint8_t already_examined = encounter_pursuit_position_already_examined(
            encounter_index, actor_index, firing_position->original_index,
            evaluation_context->evaluation_data.___u0.pursue.last_perceived_time,
            &examining_actor_count, &last_examined_time);
        candidate_time = last_examined_time;
        fresh = already_examined == 0;
    }
    else
    {
        encounter_mark_examined_pursuit_position(
            encounter_index, actor_index, firing_position->original_index,
            evaluation_context->evaluation_data.___u0.pursue.last_perceived_time);
        candidate_time = current_time;
        examining_actor_count = 7;
        fresh = 0;
    }

    if ( evaluation_context->evaluation_data.___u0.pursue.tenacious )
    {
        if ( fresh )
            firing_position->evaluation = firing_position->evaluation + 15.0f;
    }
    else if ( !fresh )
    {
        firing_position->rejected = 1;
        if ( !evaluation_context->allow_rejected_positions )
            firing_position->valid = 0;
    }

    if ( firing_position->valid )
    {
        float recency_bonus = 0.0f;
        if ( candidate_time == -1 || candidate_time + 300 < current_time )
            recency_bonus = 10.0f;
        else if ( candidate_time < current_time )
            recency_bonus = (float)(current_time - candidate_time) * SECONDS_PER_TICK;
        firing_position->evaluation = firing_position->evaluation + recency_bonus;

        float crowding_bonus = 0.0f;
        if ( examining_actor_count < 4 )
            crowding_bonus = (float)(4 - examining_actor_count) * 5.0f;
        firing_position->evaluation = firing_position->evaluation + crowding_bonus;
    }

    return firing_position->valid;
}
