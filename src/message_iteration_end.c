/* message_iteration_end @0x837A14E0 — finalize the current message-delta iteration. If the iteration wrote
 * any body bits (or an empty body is allowed), its header+body bit cost is committed to the running totals,
 * the iteration count is advanced, and the header-field metrics are committed; otherwise the header-field
 * metrics are rolled back (the iteration is dropped). The field-map bitstream (incremental mode only) and
 * the data bitstream are then disposed. */

#include <stdint.h>
#include "headers/iterated_message.h"
#include "headers/message_delta_metrics.h"

extern _message_definition *const message_delta_global_message_list[47];
extern int bitstream_dispose(bitstream_t *bit_stream);

void message_iteration_end(iterated_message *message, const uint8_t allow_empty_body)
{
    message_delta_processor_message_definition_type definition_type = message->definition_type;
    int iteration_body_bits_written = message->iteration_body_bits_written;
    const _message_definition *definition = message_delta_global_message_list[definition_type];

    if ( iteration_body_bits_written > 0 || allow_empty_body )
    {
        int iteration_bits = message->iteration_header_bits_written + iteration_body_bits_written;
        int iteration_start_bit = message->iteration_start_bit;
        int iterations_written = message->iterations_written;
        message_delta_processor_mode mode = message->mode;

        message->bits_written += iteration_bits;
        message->bits_left -= iteration_bits;
        message->iteration_start_bit = iteration_bits + iteration_start_bit;
        message->iterations_written = iterations_written + 1;

        metrics_commit_header_field_data(definition, mode, FIRST_TRANSMISSION_DIRECTION);

        message_delta_processor_mode current_mode = message->mode;
        int count;
        if ( current_mode == _message_delta_mode_incremental )
            count = message_delta_global_message_list[message->definition_type]->body_fields.count;
        else
            count = 0;
        metrics_record_iteration_dependent_overhead_encode(definition, current_mode,
                                                           FIRST_ITERATION_DEPENDENT_OVERHEAD_TYPE, count);
    }
    else
    {
        metrics_rollback_header_field_data(message_delta_global_message_list[definition_type], message->mode,
                                           FIRST_TRANSMISSION_DIRECTION);
    }

    if ( message->mode == _message_delta_mode_incremental )
        bitstream_dispose(&message->iteration_field_map_stream);
    bitstream_dispose(&message->iteration_data_stream);
}
