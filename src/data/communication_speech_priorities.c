/* communication_speech_priorities @ 0x821267C0 - .rdata read-only table, 16 bytes.
 * ai_communication_priority -> unit_speech_priority: the unit speech priority at which a
 * communication line of each communication priority is spoken. ai_communication_event.c and
 * ai_communication_finished.c pass the looked-up value on as unit_test_speech's `priority`.
 *
 * Element width is 16-bit, proven by the load in ai_communication_find_actor_to_reply_to_player:
 *   837CF63C  addi  r23, r11, communication_speech_priorities@l
 *   837CF678  lhzx  r29, r9, r23
 * and by the raw big-endian dwords, which are incoherent as int32 but a clean ladder as int16:
 * 0x00000003 0x00030003 0x00040005 0x00050008.
 * 16 bytes / 2 == 8 == NUMBER_OF_AI_COMMUNICATION_PRIORITIES, so the table is exactly its index
 * domain and carries no padding.
 *
 * raw -> resolved:
 *   +0x00 0x0000 ->  0 _unit_speech_none   [_ai_communication_priority_none]
 *   +0x02 0x0003 ->  3 _unit_speech_talk   [_ai_communication_priority_filler]
 *   +0x04 0x0003 ->  3 _unit_speech_talk   [_ai_communication_priority_chatter]
 *   +0x06 0x0003 ->  3 _unit_speech_talk   [_ai_communication_priority_talk]
 *   +0x08 0x0004 ->  4 _unit_speech_communicate   [_ai_communication_priority_communicate]
 *   +0x0A 0x0005 ->  5 _unit_speech_shout   [_ai_communication_priority_shout]
 *   +0x0C 0x0005 ->  5 _unit_speech_shout   [_ai_communication_priority_yell]
 *   +0x0E 0x0008 ->  8 _unit_speech_exclamation   [_ai_communication_priority_exclaim]
 */
#include <stdint.h>
#include "../headers/ai_communication_priority.h"
#include "../headers/unit_speech_priority.h"

const int16_t communication_speech_priorities[NUMBER_OF_AI_COMMUNICATION_PRIORITIES] =
{
    _unit_speech_none,         /* [_ai_communication_priority_none] */
    _unit_speech_talk,         /* [_ai_communication_priority_filler] */
    _unit_speech_talk,         /* [_ai_communication_priority_chatter] */
    _unit_speech_talk,         /* [_ai_communication_priority_talk] */
    _unit_speech_communicate,  /* [_ai_communication_priority_communicate] */
    _unit_speech_shout,        /* [_ai_communication_priority_shout] */
    _unit_speech_shout,        /* [_ai_communication_priority_yell] */
    _unit_speech_exclamation,  /* [_ai_communication_priority_exclaim] */
};
