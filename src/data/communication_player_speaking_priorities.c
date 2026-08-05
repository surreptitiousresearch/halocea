/* communication_player_speaking_priorities @ 0x82126810 - .rdata read-only table, 16 bytes.
 * ai_communication_priority -> ai_communication_priority: the promoted communication priority used
 * when the player is the one speaking (`priority = communication_player_speaking_priorities[
 * priority];`, ai_communication_event.c:542) - every entry raises or holds the incoming priority.
 * Both index and value therefore live in the same enum.
 *
 * 16 bytes / 2 == 8 == NUMBER_OF_AI_COMMUNICATION_PRIORITIES; int16 elements (the raw dwords
 * 0x00030004, 0x00040006 are incoherent as int32, and the decl spells `const int16_t`).
 *
 * raw -> resolved:
 *   +0x00 0x0000 -> 0 _ai_communication_priority_none   [_ai_communication_priority_none]
 *   +0x02 0x0002 -> 2 _ai_communication_priority_chatter   [_ai_communication_priority_filler]
 *   +0x04 0x0003 -> 3 _ai_communication_priority_talk   [_ai_communication_priority_chatter]
 *   +0x06 0x0004 -> 4 _ai_communication_priority_communicate   [_ai_communication_priority_talk]
 *   +0x08 0x0004 -> 4 _ai_communication_priority_communicate   [_ai_communication_priority_communicate]
 *   +0x0A 0x0006 -> 6 _ai_communication_priority_yell   [_ai_communication_priority_shout]
 *   +0x0C 0x0006 -> 6 _ai_communication_priority_yell   [_ai_communication_priority_yell]
 *   +0x0E 0x0007 -> 7 _ai_communication_priority_exclaim   [_ai_communication_priority_exclaim]
 */
#include <stdint.h>
#include "../headers/ai_communication_priority.h"

const int16_t communication_player_speaking_priorities[NUMBER_OF_AI_COMMUNICATION_PRIORITIES] =
{
    _ai_communication_priority_none,           /* [_ai_communication_priority_none] */
    _ai_communication_priority_chatter,        /* [_ai_communication_priority_filler] */
    _ai_communication_priority_talk,           /* [_ai_communication_priority_chatter] */
    _ai_communication_priority_communicate,    /* [_ai_communication_priority_talk] */
    _ai_communication_priority_communicate,    /* [_ai_communication_priority_communicate] */
    _ai_communication_priority_yell,           /* [_ai_communication_priority_shout] */
    _ai_communication_priority_yell,           /* [_ai_communication_priority_yell] */
    _ai_communication_priority_exclaim,        /* [_ai_communication_priority_exclaim] */
};
