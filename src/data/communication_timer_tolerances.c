/* communication_timer_tolerances @ 0x82126828 - .rdata read-only table, 320 bytes.
 * 320 / 4 == 80 floats == 8 * 2 * 5, and both decls (ai_communication_event.c:70,
 * ai_communication_consider_speech.c:16) spell `const float [8][2][5]`. The leading dimension is
 * ai_communication_priority: ai_communication_consider_speech indexes it with the raw
 * `communication_priority` argument, and ai_communication_event's scratch tables are strided
 * `2 * (8 * team + dialogue) + group` - the literal 8 there is this dimension. The middle
 * dimension is the two dialogue "groups" the same loop walks (`for group = 0..1`).
 *
 * All values are tolerances in SECONDS; every consumer multiplies by 30 to reach ticks.
 * Third-dimension slots observed in use:
 *   [0] ai_communication_event.c:315 vs the team chatter timer
 *   [1] ai_communication_event.c:325 vs the team talk timer
 *   [2] ai_communication_consider_speech.c:37 - the minimum gap before a unit repeats a line
 *   [3] ai_communication_event.c:336 vs the team shout timer
 *   [4] ai_communication_event.c:349 - the floor a stored scratch value must clear
 *
 * Rows 1..4 (filler..communicate) carry the long gaps and rows 5..7 (shout/yell/exclaim) the
 * short ones, i.e. higher-priority lines interrupt more readily; row 0 (_none) is all zero.
 */
#include "../headers/ai_communication_priority.h"

const float communication_timer_tolerances[NUMBER_OF_AI_COMMUNICATION_PRIORITIES][2][5] =
{
    {   /* [_ai_communication_priority_none] */
        { 0.0f,   0.0f,   0.0f,   0.0f,   0.0f },
        { 0.0f,   0.0f,   0.0f,   0.0f,   0.0f },
    },
    {   /* [_ai_communication_priority_filler] */
        { 4.0f,   4.5f,   2.5f,   2.0f,   0.0f },
        { 5.0f,   5.0f,   3.0f,   2.5f,   0.0f },
    },
    {   /* [_ai_communication_priority_chatter] */
        { 3.0f,   3.5f,   2.0f,   1.5f,   0.0f },
        { 4.5f,   4.5f,   2.5f,   2.0f,   0.0f },
    },
    {   /* [_ai_communication_priority_talk] */
        { 0.3f,   1.0f,   1.0f,   1.0f,   0.8f },
        { 1.0f,   2.0f,   1.5f,   1.5f,   0.3f },
    },
    {   /* [_ai_communication_priority_communicate] */
        { 0.0f,   0.5f,   0.5f,   0.5f,   1.3f },
        { 0.2f,   1.0f,   1.0f,   1.0f,   0.8f },
    },
    {   /* [_ai_communication_priority_shout] */
        { 0.0f,   0.0f,   1.0f,   1.0f,   1.5f },
        { 0.0f,   0.0f,   1.5f,   1.5f,   1.0f },
    },
    {   /* [_ai_communication_priority_yell] */
        { 0.0f,   0.0f,   0.5f,   0.0f,   1.5f },
        { 0.0f,   0.0f,   0.5f,   0.0f,   1.5f },
    },
    {   /* [_ai_communication_priority_exclaim] */
        { 0.0f,   0.0f,   0.5f,   0.0f,   0.0f },
        { 0.0f,   0.0f,   0.5f,   0.0f,   0.0f },
    },
};
