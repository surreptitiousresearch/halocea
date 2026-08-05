/* message_finished_encoding @0x837A1768 — close out a fully-encoded message-delta message. If anything was
 * written, the iteration count is back-patched into the iteration-independent header (when the definition
 * allows more than one iteration, using bits_needed[max_iterations-1] bits), the iteration-independent
 * header size is folded into the totals, and per-message + per-overhead-category metrics are recorded.
 * Returns the final total bit size (0 if nothing was written). */

#include "headers/iterated_message.h"
#include "headers/message_delta_metrics.h"

#include "headers/bitstream_t.h"
#include "headers/bitstream_t.h"
extern _message_definition *message_delta_global_message_list[47];
extern unsigned int bits_needed[];

extern int bitstream_write_bits(bitstream_t *bit_stream, const void *value, int bit_count);

int message_finished_encoding(iterated_message *message)
{
    int bits_written = message->bits_written;
    message_delta_processor_message_definition_type definition_type = message->definition_type;
    _message_definition *definition = message_delta_global_message_list[definition_type];

    if ( bits_written <= 0 )
        return 0;

    int max_iterations = definition->max_iterations;
    if ( max_iterations > 1 )
    {
        int iteration_count = message->iterations_written - 1;
        unsigned int count_bits = bits_needed[max_iterations - 1];
        bitstream_write_bits(&message->iteration_independent_stream, &iteration_count, count_bits);
        message->iteration_count_bits = count_bits;
    }

    int iteration_independent_size_in_bits = definition->iteration_independent_size_in_bits;
    int bits_left = message->bits_left;
    int total_bits = message->bits_written + iteration_independent_size_in_bits;
    message->padding_bits = 0;
    int iterations_written = message->iterations_written;
    message->bits_written = total_bits;
    message->bits_left = bits_left - iteration_independent_size_in_bits;

    metrics_record_message_encode(definition, message->mode, total_bits, iterations_written);
    metrics_record_iteration_independent_overhead_encode(definition, message->mode,
                                                         FIRST_ITERATION_INDEPENDENT_OVERHEAD_TYPE,
                                                         message->message_mode_bits);
    metrics_record_iteration_independent_overhead_encode(definition, message->mode,
                                                         iteration_independent_overhead_message_id,
                                                         message->message_id_bits);
    metrics_record_iteration_independent_overhead_encode(definition, message->mode,
                                                         iteration_independent_overhead_protocol,
                                                         message->protocol_bits);
    metrics_record_iteration_independent_overhead_encode(definition, message->mode,
                                                         iteration_independent_overhead_iterations,
                                                         message->iteration_count_bits);
    metrics_record_iteration_independent_overhead_encode(definition, message->mode,
                                                         iteration_independent_overhead_padding,
                                                         message->padding_bits);
    return total_bits;
}
