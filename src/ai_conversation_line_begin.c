/* ai_conversation_line_begin @0x837CD8CC — begin the current line of a running AI conversation. Resolves
 * the scenario conversation definition and its current line, then (if the line's speaker participant is
 * valid and present in the conversation's participant bitmask) latches the runtime line state:
 *   - speaker actor/unit (or -1 if the speaker slot is empty, which also forces unspatialized playback),
 *   - the "address" unit the speaker looks at: the triggering player (address_type 1) or another
 *     participant's unit (address_type 2),
 *   - whether playback is unspatialized (speaker selection type 6 or 7),
 *   - the dialogue sound tag for the speaker's chosen variant, the line delay (seconds * 30 ticks), and
 *     the line flags; the spoken/finished/advance bits are cleared.
 * Returns 1 when a line was started, 0 when the speaker participant is invalid or inactive. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/scenario.h"
#include "headers/conversation_datum.h"
#include "headers/ai_conversation.h"
#include "headers/ai_conversation_participant.h"
#include "headers/ai_conversation_line.h"
#include "headers/ai_conversation_address_type.h"
#include "headers/ai_conversation_selection_type.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"


static int conversation_actor_unit_index(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    return actor->meta.unit_index;
}

int ai_conversation_line_begin(uint16_t conversation_index)
{
    conversation_datum *conversation =
        DATA_ARRAY_ELEMENT(conversation_data, conversation_datum, conversation_index);

    ai_conversation *definition =
        &((ai_conversation *)global_scenario->ai_conversations.address)[conversation->conversation_definition_index];

    ai_conversation_line *line =
        &((ai_conversation_line *)definition->lines.address)[conversation->line_index];

    int16_t participant_index = line->participant_index;
    if (participant_index < 0
        || participant_index >= (int16_t)definition->participants.count
        || ((1u << participant_index) & conversation->participant_bitmask) == 0)
        return 0;

    ai_conversation_participant *participant =
        &((ai_conversation_participant *)definition->participants.address)[participant_index];
    int speaker_actor_index = conversation->actor_indices[participant_index];
    conversation->line_participant_index = participant_index;

    if (speaker_actor_index == -1)
    {
        conversation->line_unspatialized = 1;
        conversation->line_actor_index = -1;
        conversation->line_unit_index = -1;
        conversation->line_address_unit_index = -1;
    }
    else
    {
        conversation->line_actor_index = speaker_actor_index;
        conversation->line_unit_index = conversation_actor_unit_index(speaker_actor_index);
        conversation->line_address_unit_index = -1;

        if (line->address_type == 1)
        {
            conversation->line_address_unit_index = conversation->triggering_player_unit_index;
        }
        else if (line->address_type == _ai_conversation_address_participant)
        {
            int16_t address_participant = line->address_participant_index;
            if (address_participant >= 0
                && address_participant < (int16_t)definition->participants.count
                && conversation->actor_indices[address_participant] != -1)
            {
                conversation->line_address_unit_index =
                    conversation_actor_unit_index(conversation->actor_indices[address_participant]);
            }
        }

        /* radio / radio_sargeant participants play unspatialized. */
        conversation->line_unspatialized =
            (participant->selection_type == _ai_conversation_selection_radio
             || participant->selection_type == _ai_conversation_selection_radio_sargeant) ? 1 : 0;
    }

    int16_t variant = conversation->dialogue_indices[line->participant_index];
    conversation->line_sound_index = line->dialogue[variant].index;
    conversation->line_delay_timer = (int16_t)(int)(line->delay_time * 30.0f);
    conversation->line_flags = line->flags;
    conversation->line_advance = 0;
    conversation->line_finished = 0;
    conversation->line_spoken = 0;
    return 1;
}
