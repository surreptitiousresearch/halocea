/* action_alert_perform @0x83826B20 — per-tick update of an actor's "alert" action working state, which walks the
 * actor along its squad's alert firing positions. If the actor currently has a target alert position (order set,
 * next-position slot empty) it decides whether it has arrived: it must have no active path, or be within its
 * destination tolerance of the stored position, its dwell timer must have elapsed, it must not already be moving,
 * and its unit must not be in the frozen animation state (unit.animation.state == user_animation); when all hold it asks
 * action_alert_next_position for the next slot. Then, if the actor is idle and has a valid next position, it looks
 * up that firing position in the actor's encounter squad, copies it into the working state, rolls a random dwell
 * time (range from the firing position, scaled by 30 ticks/sec) and issues a move. On any failure it clears the
 * working state. Always returns 0.
 *
 * DEVIATION: the alert working-state is resolved to the DB-named alert_state_data arm; the
 * scenario squad / firing-position records are typed (encounter_definition/squad_definition/move_position_definition).
 * The unit-object lookup uses the salted
 * object-header index idiom. The `result` local is always 0, so the return is a literal 0. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_animation_state.h"
#include "headers/actor_datum.h"
#include "headers/scenario.h"
#include "headers/alert_state_data.h"
#include "headers/squad_definition.h"
#include "headers/encounter_definition.h"
#include "headers/move_position_definition.h"
#include <string.h>
#include "headers/blam_data_globals.h"


extern uint8_t actor_path_has_path(uint16_t actor_index);
extern float actor_destination_tolerance(uint16_t actor_index);
extern int16_t action_alert_next_position(int actor_index, int16_t move_position_order, int16_t current_position_index, uint8_t *direction_increasing);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern uint8_t actor_move_to_move_position(int actor_index, int16_t move_position_index);

uint8_t action_alert_perform(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    alert_state_data *alert = &actor->state.action_data.___u0.alert;   /* alert action-working-state */

    if ( alert->move_position_order && (uint16_t)alert->pending_move_position_index == 0xFFFF )
    {
        char reached = 1;
        if ( (uint16_t)alert->target_move_position_index != 0xFFFF && actor_path_has_path(actor_index) )
        {
            float delta_x = alert->target_position_definition.position.n[0] - actor->input.position.body_position.x;  /* target - actor body pos */
            float delta_z = alert->target_position_definition.position.n[2] - actor->input.position.body_position.z;
            float delta_y = alert->target_position_definition.position.n[1] - actor->input.position.body_position.y;
            float dist_sq = ((delta_y * delta_y)
                    + ((delta_z * delta_z) + (delta_x * delta_x)));
            float tolerance = actor_destination_tolerance(actor_index);
            if ( tolerance <= 0.5f )
                tolerance = 0.5f;
            if ( dist_sq > (tolerance * tolerance) )
                reached = 0;
        }

        if ( reached )
        {
            if ( alert->wait_ticks <= 0 )
            {
                if ( alert->must_play_animation )
                {
                    reached = 0;
                }
                else
                {
                    int unit_index = actor->meta.unit_index;
                    unit_datum *unit_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
                    if ( (uint8_t)unit_object->unit.animation.state == _unit_state_user_animation )
                        reached = 0;
                }
            }
            else
            {
                reached = 0;
            }
        }

        if ( reached )
            alert->pending_move_position_index = action_alert_next_position(actor_index, alert->move_position_order,
                    alert->target_move_position_index, &alert->move_position_increasing);
    }

    if ( !actor->meta.timeslice )
        return 0;
    if ( actor->meta.dormant )
        return 0;

    int next_position = alert->pending_move_position_index;
    if ( next_position == -1 )
        return 0;

    bool moved = 0;
    if ( actor->meta.encounter_index != -1 )
    {
        encounter_definition *encounter =
            &((encounter_definition *)global_scenario->ai_encounters.address)[(uint16_t)actor->meta.encounter_index];
        squad_definition *squad =
            &((squad_definition *)encounter->squads.address)[actor->meta.squad_index];

        if ( next_position >= 0 && next_position < squad->move_positions.count )
        {
            /* stride 80 == sizeof(move_position_definition); kept float* view for the [5]/[6]/memcpy(0x50) access */
            float *firing_position =
                (float *)&((move_position_definition *)squad->move_positions.address)[next_position];
            float range_max = firing_position[6];
            float range_min = firing_position[5];
            float dwell = real_seed_random_range(get_global_random_seed_address(), range_min, range_max);

            int16_t saved_position = alert->pending_move_position_index;
            alert->pending_move_position_index = -1;
            alert->target_move_position_index = saved_position;
            memcpy(&alert->target_position_definition, firing_position, 0x50);
            int16_t current_position = alert->target_move_position_index;
            alert->must_play_animation = 1;
            alert->wait_ticks = (int)(dwell * 30.0f);
            moved = actor_move_to_move_position(actor_index, current_position) != 0;
        }
    }

    if ( !moved )
    {
        int16_t saved_position = alert->pending_move_position_index;
        alert->pending_move_position_index = -1;
        alert->wait_ticks = 0;
        alert->must_play_animation = 0;
        alert->target_move_position_index = saved_position;
    }

    return 0;
}
