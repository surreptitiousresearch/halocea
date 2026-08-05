/* reply_filter_searching @0x837CC0C0 — communication-reply eligibility filter: the replying actor must
 * be running the uncover action (state.action == actor_action_uncover, with alert sub-index == 1) or the
 * search action (state.action == actor_action_search). Raw actor-datum offsets (stride 1828), matches the sibling
 * reply_filter_no_certain_target's convention. original_unit_index and communication are unused. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/ai_information_packet.h"
#include "headers/actor_datum.h"
#include "headers/actor_action.h"
#include "headers/blam_data_globals.h"


uint8_t reply_filter_searching(int original_unit_index, ai_information_packet *communication, int reply_actor_index)
{
    if (reply_actor_index == -1)
        return 0;

    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, reply_actor_index);
    int16_t status = actor->state.action;

    if (status == actor_action_uncover)
        return actor->state.action_data.___u0.alert.pending_move_position_index == 1;

    return status == actor_action_search;
}
