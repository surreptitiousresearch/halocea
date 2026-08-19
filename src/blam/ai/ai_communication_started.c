/* ai_communication_started @0x837CDDE8 — notification that a unit began an AI vocalization. For most
 * priorities nothing is done; for the remaining priorities, refresh the unit's speech timers (unless
 * the dialogue timers were already updated this packet). */

#include <stdint.h>
#include "headers/ai_information_packet.h"
#include "headers/ai_communication_priority.h"

extern void ai_communication_update_speech_timers(int unit_index, int16_t priority, int16_t vocalization_type, int16_t dialogue_type_index);

void ai_communication_started(int unit_index, int16_t priority, int16_t vocalization_type, const ai_information_packet *ai_information)
{
    switch ( priority )
    {
        case _ai_communication_priority_none:
        case _ai_communication_priority_filler:
        case _ai_communication_priority_chatter:
        case _ai_communication_priority_exclaim:
        case 10: /* out of range of ai_communication_priority (NUMBER_OF == 8); value unnamed */
            return;
        default:
            if ( !ai_information->updated_dialogue_timers )
                ai_communication_update_speech_timers(unit_index, priority, ai_information->dialogue_type_index, -1); /* phantom 5th arg dropped */
            break;
    }
}
