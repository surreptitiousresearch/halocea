#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);

void action_vehicle_begin(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    actor->state.action_data.___u0.vehicle.stuck_detection_counter = 0;
    actor->state.action_data.___u0.vehicle.stuck_detection_time = game_time_get();
    *(int *)&actor->state.action_data.___u0.vehicle.stuck_detection_point.x = *(int *)&actor->input.position.body_position.x;
    *(int *)&actor->state.action_data.___u0.vehicle.stuck_detection_point.y = *(int *)&actor->input.position.body_position.y;
    *(int *)&actor->state.action_data.___u0.vehicle.stuck_detection_point.z = *(int *)&actor->input.position.body_position.z;
}
