/* message_delta_processor_initialize @ 0x837A2C10 — Blam engine.
 * One-time init for the message-delta processor: optionally runs the protocol
 * changeover init, marks every field_type_definition row's `initialized` flag
 * (offset 0x14, i.e. one byte past the 6th int of each 24-byte row) set, builds
 * all message definitions, then initializes metrics. */

#include "headers/blam_data_globals.h"

extern void message_delta_parameters_protocol_initialize(void);
extern void initialize_messages(void);
extern void metrics_initialize(const int message_definition_packed_bit_size);

void message_delta_processor_initialize(void)
{
    if (g_protocol_changeover_enabled == 1)
        message_delta_parameters_protocol_initialize();

    /* DEVIATION: the base symbol was read off IDA's DISPLACEMENT spelling. @0x837A2C40 the binary
     * does `addi r11, r11, (gITEM_PLACEMENT_BITS_Z - 0x84182E10)` -- that expression is IDA printing
     * the constant -4, not a reference to gITEM_PLACEMENT_BITS_Z (a 4-byte int at 0x84182E0C, which
     * happens to sit exactly 4 bytes below the real base). r11 comes from
     * `lis/addi _message_delta_global_field_type_list` @0x837A2C34-2C38 (0x84182E10), so the 28
     * `stbu r10, 0x18(r11)` @0x837A2C48 write byte 0x14 -- `initialized` -- of each 24-byte row of
     * message_delta_global_field_type_list[28], exactly as this file's header comment states. The
     * old transcription walked 672 bytes off a 4-byte global. Byte-for-byte the same 28 stores;
     * same defect as initialize_types.c / dispose_types.c. */
    for ( int type_index = 0; type_index < 28; type_index++ )
        message_delta_global_field_type_list[type_index].initialized = 1;

    initialize_messages();
    metrics_initialize(6);
}
