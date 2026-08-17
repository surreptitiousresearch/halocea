/* reply_filter_no_certain_target @0x837CC338 */
#include <stdint.h>
#include "headers/data_array.h"
#include "headers/ai_information_packet.h"
#include "headers/actor_datum.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_mode.h"
#include "headers/blam_data_globals.h"


uint8_t reply_filter_no_certain_target(int original_unit_index, ai_information_packet *communication, int reply_actor_index)
{
    if (reply_actor_index == -1)
        return 0;

    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, reply_actor_index);

    if (actor->state.mode != _actor_mode_combat)
        return 0;

    return actor->state.combat_status < _actor_combat_status_certain;
}
