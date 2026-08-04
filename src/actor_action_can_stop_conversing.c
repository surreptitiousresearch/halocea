/* actor_action_can_stop_conversing @0x837F2CD8 — true when an actor is allowed to break out of its current
 * AI conversation. Trivially true when it is not in one (external_orders conversation index at actor+476 is
 * NONE). Otherwise the scenario conversation definition's flags gate the break conditions:
 *   bit 1 (0x2) — stop if the actor was damaged (actor status byte +502),
 *   bit 2 (0x4) — stop with a visible enemy   (combat-status word +616 >= 9),
 *   bit 3 (0x8) — stop if alerted to an enemy (combat-status word +616 >= 6). */

#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/scenario.h"
#include "headers/ai_conversation.h"
#include "headers/conversation_datum.h"
#include "headers/actor_target_type.h"
#include "headers/ai_conversation_definition_flags.h"
#include "headers/blam_data_globals.h"


unsigned __int8 actor_action_can_stop_conversing(unsigned __int16 actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int conversation_index = actor->external_orders.conversation_index;

    if ( conversation_index == -1 )
        return 1;

    /* recovered: *((__int16 *)conversation_data->data + 50*idx + 1) -> conversation->conversation_definition_index */
    __int16 definition_index =
        DATA_ARRAY_ELEMENT(conversation_data, conversation_datum, conversation_index)->conversation_definition_index;
    unsigned __int16 flags = ((const ai_conversation *)global_scenario->ai_conversations.address
                             + definition_index)->flags;

    return (flags & (1u << _ai_conversation_stop_if_damaged_bit)) != 0 && actor->situation.specific_threats[8]
        || (flags & (1u << _ai_conversation_stop_if_visible_enemy_bit)) != 0 && actor->target.target_type >= actor_target_potentially_dangerous_enemy
        || (flags & (1u << _ai_conversation_stop_if_alerted_to_enemy_bit)) != 0 && actor->target.target_type >= actor_target_definite_orphan;
}
