/* message_delta_processor_initialize @ 0x837A2C10 — Blam engine.
 * One-time init for the message-delta processor: optionally runs the protocol
 * changeover init, marks every field_type_definition row's `initialized` flag
 * (offset 0x14, i.e. one byte past the 6th int of each 24-byte row) set, builds
 * all message definitions, then initializes metrics. */

#include "headers/blam_data_globals.h"

extern void message_delta_parameters_protocol_initialize(void);
extern int initialize_messages(void);
extern void metrics_initialize(const int message_definition_packed_bit_size);

void message_delta_processor_initialize(void)
{
    if (g_protocol_changeover_enabled == 1)
        message_delta_parameters_protocol_initialize();

    /* mirror of dispose_types.c's loop but setting the flag to 1 instead of 0 */
    int *field_type_row = &gITEM_PLACEMENT_BITS_Z;
    int count = 28;
    do
    {
        field_type_row += 6;
        *(char *)field_type_row = 1;
        --count;
    } while (count);

    initialize_messages();
    metrics_initialize(6);
}
