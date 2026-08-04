/* message_delta_processor_get_maximum_message_size @ 0x837A0998 — Blam engine.
 * Returns the maximum encoded size (in bytes, rounded up) of the message with
 * the given definition type: the definition's maximum_total_size (bits) divided
 * by 8, rounded up. */

#include "headers/message_definition.h"

#include <math.h>

extern _message_definition *message_delta_global_message_list[];

int message_delta_processor_get_maximum_message_size(int definition_type)
{
    int bytes = message_delta_global_message_list[definition_type]->maximum_total_size / 8;
    return (int)ceil((double)bytes);
}
