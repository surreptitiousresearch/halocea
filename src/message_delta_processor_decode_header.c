/* message_delta_processor_decode_header @0x837A2438 — decode the (iterated) header of the current
 * message-delta iteration. Failure — no bits consumed when the message has header fields or is
 * incremental — rolls back the pending header-field metrics and rewinds the input stream to the original
 * position; success accumulates the consumed bits into the decode state, commits the metrics, and flags
 * the iteration header decoded. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/message_definition.h"
#include "headers/message_delta_metrics.h"
#include "headers/bitstream_t.h"

typedef struct _message_definition _message_definition;
#include "headers/bitstream_t.h"
extern _message_definition *message_delta_global_message_list[47];

extern int decode_message_dependent_header_iterated(message_delta_processor_header *const header);
extern void metrics_rollback_header_field_data(const _message_definition *message_definition, message_delta_processor_mode mode, transmission_direction direction);
extern void metrics_commit_header_field_data(const _message_definition *message_definition, message_delta_processor_mode mode, transmission_direction direction);
extern uint8_t bitstream_seek(bitstream_t *const bit_stream, const int offset, const bitstream_seek_origin origin);

uint8_t message_delta_processor_decode_header(message_delta_processor_header *const header)
{
    message_delta_processor_decoding_information *decoding_information = header->decoding_information;
    int bits = decode_message_dependent_header_iterated(header);

    uint8_t succeeded;
    if ( bits <= 0
      && (bits
       || decoding_information->mode
       || message_delta_global_message_list[decoding_information->definition_type]->header_fields->count > 0) )
    {
        succeeded = 0;
        metrics_rollback_header_field_data(
            message_delta_global_message_list[decoding_information->definition_type],
            decoding_information->mode, transmission_direction_received);
    }
    else
    {
        succeeded = 1;
        decoding_information->state += bits;
        metrics_commit_header_field_data(
            message_delta_global_message_list[decoding_information->definition_type],
            decoding_information->mode, transmission_direction_received);
        decoding_information->iteration_header_decoded = 1;
    }

    if ( !succeeded )
        bitstream_seek(decoding_information->input_stream, decoding_information->original_stream_position,
                       _bitstream_seek_beginning);
    return succeeded;
}
