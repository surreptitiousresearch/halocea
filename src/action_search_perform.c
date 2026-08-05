/* action_search_perform @0x83822DD8 — per-tick "search" behaviour: an actor investigating the last known
 * position of a target it has lost track of. It decides whether the search point has been reached
 * (action_data[2]) either by line-of-sight to the estimated target position (search state 1 with a valid firing
 * position) or by proximity to the searched prop; if not reached and not enough squadmates are already
 * searching the same spot it marks the pursuit position examined. Depending on the search state it then
 * moves to the prop, moves to the firing position, or halts. Returns the "search finished" flag (action_data[0])
 * or 1 when a move failed and the search is abandoned this tick.
 *
 * Working state resolved to the DB-named search_state_data arm of action_data. DEVIATION: the ai_test_line_of_sight
 * result is tested via (cntlzw(result) & 0x20) != 0, which is exactly result == 0 (catalog idiom); the final
 * argument's `actor[344] + 1 - (actor[344] + (int344 == -1))` cancels actor[344] and reduces to
 * 1 - (int344 == -1). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/prop_iterator.h"
#include "headers/real_point3d.h"
#include "headers/search_state_data.h"
#include "headers/pursuit_location_type.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"


#include "headers/real_vector3d.h"
#include "headers/path_state.h"
extern void unit_estimate_position(int unit_index, int16_t estimate_mode, const real_point3d *body_position, real_vector3d *desired_facing, real_vector3d *desired_gun_offset, real_point3d *estimated_position);
extern int16_t ai_test_line_of_sight(const real_point3d *p0, int16_t p0_cluster_index, const real_point3d *p1, int16_t p1_cluster_index, int16_t mode, uint8_t test_line_of_fire, int ignore_object_index, uint8_t ignore_vehicles);
extern void prop_iterator_new(prop_iterator *iterator, uint16_t actor_index);
extern prop_datum *prop_iterator_next(prop_iterator *iterator);
extern uint8_t actors_searching_same_position(int actor_index, int friend_actor_index);
extern uint8_t encounter_mark_examined_pursuit_position(int encounter_index, int actor_index, int16_t firing_position_index, int history_start_time);
extern uint8_t actor_move_to_prop(int actor_index, unsigned int prop_index, float accept_radius);
extern uint8_t actor_move_to_firing_position(int actor_index, int16_t firing_position_index, path_state *cached_path_state);
extern uint8_t actor_move_halt(uint16_t actor_index);

uint8_t action_search_perform(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    search_state_data *search_finished = &actor->state.action_data.___u0.search;

    if ( actor->meta.swarm || !actor->meta.timeslice || search_finished->search_done )
        return search_finished->search_done;

    int16_t same_position_count = 0;
    int search_state = actor->state.action_data.___u0.search.pursuit_location.type;
    actor->state.action_data.___u0.search.at_destination = 1;

    if ( search_state || actor->target.target_prop_index == -1 )
    {
        if ( search_state == _pursuit_location_position && (uint16_t)actor->state.action_data.___u0.search.pursuit_location.firing_position_index != 0xFFFF )
        {
            float d1 = actor->input.position.body_position.y - actor->state.action_data.___u0.search.pursuit_location.position.y;
            float d2 = actor->input.position.body_position.z - actor->state.action_data.___u0.search.pursuit_location.position.z;
            float d0 = actor->input.position.body_position.x - actor->state.action_data.___u0.search.pursuit_location.position.x;
            float distance_squared = ((d0 * d0) + ((d2 * d2) + (d1 * d1)));
            if ( distance_squared >= 0.48999998f )
            {
                if ( distance_squared <= 6.25f )
                {
                    real_point3d estimated_position;
                    unit_estimate_position(actor->meta.unit_index, 1, &actor->state.action_data.___u0.search.pursuit_location.position, 0, 0,
                            &estimated_position);
                    actor->state.action_data.___u0.search.at_destination = ai_test_line_of_sight(&actor->input.position.head_position,
                            actor->input.position.body_location.cluster_index, &estimated_position,
                            actor->state.action_data.___u0.search.pursuit_location.cluster_index, 0, 0, -1,
                            1 - (actor->input.vehicle_index == -1)) == 0;
                }
                else
                {
                    actor->state.action_data.___u0.search.at_destination = 0;
                }
            }
            else
            {
                actor->state.action_data.___u0.search.at_destination = 1;
            }
        }
    }
    else
    {
        prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, actor->target.target_prop_index);
        float tolerance = prop->line_of_sight ? 0.69999999f : 1.7f;
        float dy = actor->input.position.body_position.y - prop->body_position.y;
        float dz = actor->input.position.body_position.z - prop->body_position.z;
        float dx = actor->input.position.body_position.x - prop->body_position.x;
        actor->state.action_data.___u0.search.at_destination = ((dx * dx) + ((dz * dz) + (dy * dy)))
                   < (tolerance * tolerance);
    }

    if ( !actor->state.action_data.___u0.search.at_destination && !actor->state.action_data.___u0.search.charging )
    {
        int16_t close_searchers = 0;
        prop_iterator iterator;
        prop_iterator_new(&iterator, actor_index);
        for ( prop_datum *prop = prop_iterator_next(&iterator); prop; prop = prop_iterator_next(&iterator) )
        {
            int state = prop->state;
            if ( state >= _prop_state_becoming_unacknowledged && state <= _prop_state_acknowledged && !prop->enemy && !prop->dead )
            {
                int other_actor = prop->actor_index;
                if ( other_actor != -1 && actors_searching_same_position(actor_index, other_actor) )
                {
                    ++same_position_count;
                    actor_datum *other = DATA_ARRAY_ELEMENT(actor_data, actor_datum, prop->actor_index);
                    if ( !other->meta.swarm && !other->control.moving )
                    {
                        float d1 = other->input.position.body_position.y - actor->input.position.body_position.y;
                        float d2 = other->input.position.body_position.z - actor->input.position.body_position.z;
                        float d0 = other->input.position.body_position.x - actor->input.position.body_position.x;
                        if ( ((d0 * d0) + ((d2 * d2) + (d1 * d1)))
                                < 0.64000005f )
                            ++close_searchers;
                    }
                }
            }
        }

        int16_t required_searchers = (actor->state.action_data.___u0.search.pursuit_location.type != _pursuit_location_position) ? 4 : 2;
        if ( actor->state.action_data.___u0.search.tenacious || same_position_count < required_searchers )
        {
            if ( close_searchers > 0 )
                actor->state.action_data.___u0.search.at_destination = 1;
        }
        else
        {
            search_finished->search_done = 1;
            int examined_position = -1;
            if ( actor->target.target_prop_index != -1 )
                examined_position = DATA_ARRAY_ELEMENT(prop_data, prop_datum,
                        actor->target.target_prop_index)->last_perceived_time;
            int encounter_index = actor->meta.encounter_index;
            if ( encounter_index != -1 )
                encounter_mark_examined_pursuit_position(encounter_index, actor_index,
                        actor->state.action_data.___u0.search.pursuit_location.firing_position_index, examined_position);
        }
    }

    if ( actor->state.action_data.___u0.search.at_destination )
    {
        actor_move_halt(actor_index);
        return search_finished->search_done;
    }

    if ( !actor->state.action_data.___u0.search.pursuit_location.type )
    {
        if ( !actor_move_to_prop(actor_index, actor->target.target_prop_index, 2.5f) )
        {
            actor->state.action_data.___u0.search.search_failed = 1;
            search_finished->search_done = 1;
            return 1;
        }
        return search_finished->search_done;
    }

    if ( actor->state.action_data.___u0.search.pursuit_location.type != _pursuit_location_position )
    {
        actor_move_halt(actor_index);
    }
    else
    {
        actor->firing_positions.current_position_index = -1;
        if ( !actor_move_to_firing_position(actor_index, actor->state.action_data.___u0.search.pursuit_location.firing_position_index, 0) )
        {
            actor->state.action_data.___u0.search.search_failed = 1;
            search_finished->search_done = 1;
            return 1;
        }
    }

    return search_finished->search_done;
}