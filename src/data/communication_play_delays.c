/* communication_play_delays @ 0x82126968 - .rdata read-only table, 20 bytes.
 * ai_communication_protagonist_type -> play delay in SECONDS before the line is voiced:
 * `int play_delay = (int)(communication_play_delays[row->protagonist_type] * 30.0f);`
 * (ai_communication_event.c:588; row is a dialogue_usage, whose protagonist_type field carries
 * this enum). 20 bytes / 4 == 5 == NUMBER_OF_COMMUNICATION_PROTAGONIST_TYPES, so the table is
 * exactly its index domain with no padding; every word is a clean float, matching the decl.
 *
 * raw -> resolved:
 *   +0x00 0x00000000 -> 0.0f  [_comm_protagonist_subject]
 *   +0x04 0x3F000000 -> 0.5f  [_comm_protagonist_cause]
 *   +0x08 0x3F4CCCCD -> 0.8f  [_comm_protagonist_friend]
 *   +0x0C 0x3F000000 -> 0.5f  [_comm_protagonist_target]
 *   +0x10 0x3F4CCCCD -> 0.8f  [_comm_protagonist_enemy]
 */
#include "../headers/ai_communication_protagonist_type.h"

const float communication_play_delays[NUMBER_OF_COMMUNICATION_PROTAGONIST_TYPES] =
{
    0.0f,  /* [_comm_protagonist_subject] */
    0.5f,  /* [_comm_protagonist_cause] */
    0.8f,  /* [_comm_protagonist_friend] */
    0.5f,  /* [_comm_protagonist_target] */
    0.8f,  /* [_comm_protagonist_enemy] */
};
