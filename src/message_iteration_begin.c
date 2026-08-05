/* message_iteration_begin @0x837A1250 — open one iteration of an in-progress message-delta encode: reset
 * the per-iteration bit counters, then in incremental mode open the field-map bitstream (one bit per body
 * field, counted into the iteration header); in stateless mode just clear the field-map stream state
 * (the compiled code zeroes the 7 ints of the bitstream_t struct in place). Finally open the iteration
 * data stream over the remaining bit budget.
 *
 * Deviation: attested void — the final bitstream_initialize is a plain `bl` (837A12F4) whose r3 merely
 * survives the epilogue to the blr; there is no explicit return computation and the binary has no caller
 * that consumes it, so the decompiler's unsigned __int8 status is threaded, not returned. */

#include <stdint.h>
#include <string.h>
#include "headers/iterated_message.h"

extern _message_definition **message_delta_global_message_list;
extern uint8_t bitstream_initialize(bitstream_t *bit_stream, uint8_t *buffer, unsigned int first_bit, unsigned int bit_count, bitstream_mode mode);

void message_iteration_begin(iterated_message *const message)
{
    message->iteration_body_bits_written = 0;
    message->iteration_header_bits_written = 0;

    if ( message->mode == _message_delta_mode_incremental )
    {
        unsigned int field_count = message_delta_global_message_list[message->definition_type]->body_fields.count;
        bitstream_initialize(&message->iteration_field_map_stream, message->buffer,
                             message->iteration_start_bit, field_count, _bitstream_mode_write);
        message->iteration_header_bits_written += field_count;
    }
    else
    {
        /* recovered: pointer-walk over &iteration_body_bits_written -> named member; the 7-int loop
         * zeroes exactly the 28 bytes of iteration_field_map_stream (0x48..0x63). */
        memset(&message->iteration_field_map_stream, 0, sizeof(bitstream_t));
    }

    bitstream_initialize(&message->iteration_data_stream, message->buffer,
                         message->iteration_start_bit + message->iteration_header_bits_written,
                         message->bits_left - message->iteration_header_bits_written,
                         _bitstream_mode_write);
}
