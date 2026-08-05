/* communication_recipient_default_look_priorities @ 0x82126800 - .rdata read-only table, 16 bytes.
 * ai_communication_priority -> secondary_look_priority: the default look priority given to the
 * recipient of a communication event when the dialogue row does not override it.
 * ai_communication_event.c:636 substitutes it for a per-row recipient_look_priority of -1 or 1;
 * the value ends up in ai_information_packet.look_priority, the secondary-look slot.
 *
 * 16 bytes / 2 == 8 == NUMBER_OF_AI_COMMUNICATION_PRIORITIES. int16 elements: the raw dwords
 * (0x00030004, 0x00060004) are incoherent as int32, and every decl spells `const int16_t`.
 *
 * raw -> resolved:
 *   +0x00 0x0000 -> 0 _secondary_look_priority_none   [_ai_communication_priority_none]
 *   +0x02 0x0003 -> 3 _secondary_look_priority_idle_aim   [_ai_communication_priority_filler]
 *   +0x04 0x0003 -> 3 _secondary_look_priority_idle_aim   [_ai_communication_priority_chatter]
 *   +0x06 0x0004 -> 4 _secondary_look_priority_aim   [_ai_communication_priority_talk]
 *   +0x08 0x0005 -> 5 _secondary_look_priority_turn_and_aim   [_ai_communication_priority_communicate]
 *   +0x0A 0x0005 -> 5 _secondary_look_priority_turn_and_aim   [_ai_communication_priority_shout]
 *   +0x0C 0x0006 -> 6 _secondary_look_priority_stop_and_aim   [_ai_communication_priority_yell]
 *   +0x0E 0x0004 -> 4 _secondary_look_priority_aim   [_ai_communication_priority_exclaim]
 */
#include <stdint.h>
#include "../headers/ai_communication_priority.h"
#include "../headers/secondary_look_priority.h"

const int16_t communication_recipient_default_look_priorities[NUMBER_OF_AI_COMMUNICATION_PRIORITIES] =
{
    _secondary_look_priority_none,                 /* [_ai_communication_priority_none] */
    _secondary_look_priority_idle_aim,             /* [_ai_communication_priority_filler] */
    _secondary_look_priority_idle_aim,             /* [_ai_communication_priority_chatter] */
    _secondary_look_priority_aim,                  /* [_ai_communication_priority_talk] */
    _secondary_look_priority_turn_and_aim,         /* [_ai_communication_priority_communicate] */
    _secondary_look_priority_turn_and_aim,         /* [_ai_communication_priority_shout] */
    _secondary_look_priority_stop_and_aim,         /* [_ai_communication_priority_yell] */
    _secondary_look_priority_aim,                  /* [_ai_communication_priority_exclaim] */
};
