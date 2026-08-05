/* action_alert_setup @0x838264D0 — resets an actor's "alert" behavior action_data payload and seeds its
 * initial move-position order. When the actor is a swarm member (actor+6 != 0), the requested
 * `move_position_order` is overridden to 0 rather than honored. Always returns 1. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/alert_state_data.h"
#include "headers/blam_data_globals.h"
extern void *memset(void *dst, int value, unsigned int n);


uint8_t action_alert_setup(uint16_t actor_index, int16_t move_position_order, int16_t initial_move_position_index, alert_state_data *state_data)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    memset(state_data, 0, sizeof(alert_state_data));

    uint8_t is_swarm = actor->meta.swarm;

    state_data->pending_move_position_index = initial_move_position_index;
    state_data->wait_ticks = 0;
    state_data->target_move_position_index = -1;
    state_data->must_play_animation = 0;
    state_data->move_position_increasing = 1;
    state_data->move_position_order = is_swarm ? 0 : move_position_order;

    return 1;
}
