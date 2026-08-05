/* action_converse_setup @0x837FCA78 — initialize a converse_state_data block for an actor about to run the
 * "converse" action in the given conversation. Always succeeds (returns 1). If the conversation definition
 * specifies a non-zero run-to distance, the actor will move toward the conversation's triggering unit until
 * within range; otherwise no run-to target is set. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/scenario.h"
#include "headers/converse_state_data.h"
#include "headers/conversation_datum.h"
#include "headers/ai_conversation.h"
#include "headers/blam_data_globals.h"


uint8_t action_converse_setup(uint16_t actor_index, int conversation_index, converse_state_data *state_data)
{
    conversation_datum *conversation =
        DATA_ARRAY_ELEMENT(conversation_data, conversation_datum, conversation_index);
    /* conversation definition: 116-byte elements in scenario.ai_conversations (stride == sizeof) */
    ai_conversation *conversation_definition =
        &((ai_conversation *)global_scenario->ai_conversations.address)[conversation->conversation_definition_index];

    state_data->conversation_index = 0;
    /* DEVIATION: disasm 0x837FCAB0 zeroes bytes 4..7 (failed + in_range + pad) with one stw;
     * untangled into the per-field clears (pad bytes carry no state). */
    state_data->failed = 0;
    state_data->in_range = 0;
    state_data->run_to_distance = 0.0;
    state_data->run_to_unit_index = 0;
    state_data->run_to_prop_index = 0;

    state_data->conversation_index = conversation_index;

    float run_to_distance = conversation_definition->run_to_player_dist;
    state_data->run_to_distance = run_to_distance;
    if ( run_to_distance == 0.0 )
        state_data->run_to_unit_index = -1;
    else
        state_data->run_to_unit_index = conversation->triggering_player_unit_index;
    state_data->in_range = 0;
    state_data->run_to_prop_index = -1;
    return 1;
}
