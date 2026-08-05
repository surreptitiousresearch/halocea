/* communication_notification_delays @ 0x821267D0 - .rdata read-only table, 32 bytes.
 * ai_communication_priority -> AI-notification delay in SECONDS. Both consumers scale it by 30
 * to reach ticks: `speech.ai_notification_delay = (int)(communication_notification_delays[
 * row->communication_priority] * 30.0f)` (ai_communication_finished.c:164,
 * ai_communication_event.c:598). 32 bytes / 4 == 8 == NUMBER_OF_AI_COMMUNICATION_PRIORITIES;
 * every word is a clean IEEE-754 float, so the `const float` spelling both decls carry is right.
 *
 * raw -> resolved:
 *   +0x00 0x3F000000 -> 0.5f   [_ai_communication_priority_none]
 *   +0x04 0x3F000000 -> 0.5f   [_ai_communication_priority_filler]
 *   +0x08 0x3F000000 -> 0.5f   [_ai_communication_priority_chatter]
 *   +0x0C 0x3F000000 -> 0.5f   [_ai_communication_priority_talk]
 *   +0x10 0x3F000000 -> 0.5f   [_ai_communication_priority_communicate]
 *   +0x14 0x3F000000 -> 0.5f   [_ai_communication_priority_shout]
 *   +0x18 0x3F000000 -> 0.5f   [_ai_communication_priority_yell]
 *   +0x1C 0x3E99999A -> 0.3f   [_ai_communication_priority_exclaim]
 */
#include "../headers/ai_communication_priority.h"

const float communication_notification_delays[NUMBER_OF_AI_COMMUNICATION_PRIORITIES] =
{
    0.5f,    /* [_ai_communication_priority_none] */
    0.5f,    /* [_ai_communication_priority_filler] */
    0.5f,    /* [_ai_communication_priority_chatter] */
    0.5f,    /* [_ai_communication_priority_talk] */
    0.5f,    /* [_ai_communication_priority_communicate] */
    0.5f,    /* [_ai_communication_priority_shout] */
    0.5f,    /* [_ai_communication_priority_yell] */
    0.3f,    /* [_ai_communication_priority_exclaim] */
};
