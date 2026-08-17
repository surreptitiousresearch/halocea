/* action_vehicle_begin @0x838213F8 */
#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);

void action_vehicle_begin(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    actor->state.action_data.___u0.vehicle.stuck_detection_counter = 0;
    actor->state.action_data.___u0.vehicle.stuck_detection_time = game_time_get();
    /* DEVIATION: decompiler word-punned this 12-byte real_point3d copy (lwz/stw triple, 0xAC..0xB8); plain struct assignment */
    actor->state.action_data.___u0.vehicle.stuck_detection_point = actor->input.position.body_position;
}
