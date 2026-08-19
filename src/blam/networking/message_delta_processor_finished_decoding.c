/* message_delta_processor_finished_decoding @0x837A0FA0 — metrics bookkeeping at the end of a message-delta
 * decode: record the message decode itself (iteration count only when the decode reached a valid state),
 * then the per-category iteration-independent overhead bit counts (message mode, message id, protocol,
 * iteration count, and a zero for padding). */

#include "headers/message_delta_processor_header.h"
#include "headers/message_definition.h"
#include "headers/message_delta_metrics.h"

extern _message_definition *const message_delta_global_message_list[47];

extern void metrics_record_message_decode(const _message_definition *const message_definition, const message_delta_processor_mode mode, const int encoded_size_in_bits, const int iterations);
extern void metrics_record_iteration_independent_overhead_decode(const _message_definition *const message_definition, const message_delta_processor_mode mode, const iteration_independent_overhead_type type, const int bits);

void message_delta_processor_finished_decoding(const message_delta_processor_header *const header)
{
    message_delta_processor_decoding_information *decoding_information = header->decoding_information;
    const _message_definition *definition = message_delta_global_message_list[decoding_information->definition_type];

    int state = decoding_information->state;
    int iteration_count = state ? decoding_information->iteration_count : 0;
    metrics_record_message_decode(definition, decoding_information->mode, state, iteration_count);

    message_delta_processor_mode mode = decoding_information->mode;
    metrics_record_iteration_independent_overhead_decode(definition, mode,
            FIRST_ITERATION_INDEPENDENT_OVERHEAD_TYPE, decoding_information->message_mode_bits);
    metrics_record_iteration_independent_overhead_decode(definition, mode,
            iteration_independent_overhead_message_id, decoding_information->message_id_bits);
    metrics_record_iteration_independent_overhead_decode(definition, mode,
            iteration_independent_overhead_protocol, decoding_information->protocol_bits);
    metrics_record_iteration_independent_overhead_decode(definition, mode,
            iteration_independent_overhead_iterations, decoding_information->iteration_count_bits);
    metrics_record_iteration_independent_overhead_decode(definition, mode,
            iteration_independent_overhead_padding, 0);
}
