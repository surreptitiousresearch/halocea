/* message_delta_processor_encode_incremental @0x837A2900 — encode a single message-delta payload in
 * "incremental" mode (diffed against a baseline) into `buffer`, returning the encoded size in bits. Forwards
 * to encode_iterations with a one-element iteration: the source header, data, and baseline pointers are each
 * wrapped in a single-element pointer array. A NULL source_header passes a NULL header array.
 *
 * DEVIATION: the decompiler invented spurious trailing parameters (a8..a18); the disassembly shows a clean
 * 7-parameter prototype (definition_type, source_header, source_data, baseline_data, buffer,
 * buffer_size_in_bits, allow_empty_body) and the final encode_iterations argument is allow_empty_body (r9). */

#include <stdint.h>
#include "headers/message_definition.h"
#include "headers/message_delta_processor_mode.h"

extern int encode_iterations(message_delta_processor_mode mode, message_delta_processor_message_definition_type definition_type, const void *const *headers, const void **datas, const void *const *baselines, int iterations, void *buffer, int buffer_size_in_bits, uint8_t allow_empty_body);

int message_delta_processor_encode_incremental(
        message_delta_processor_message_definition_type definition_type,
        const void *source_header, const void *source_data, const void *baseline_data,
        void *buffer, int buffer_size_in_bits, uint8_t allow_empty_body)
{
    const void *header_slot   = source_header;
    const void *data_slot     = source_data;
    const void *baseline_slot = baseline_data;

    /* const void ** -> const void *const * is a legal implicit conversion (const added to the immediately
     * pointed-to type), so no casts are needed to pass these single-element arrays. */
    return encode_iterations(_message_delta_mode_incremental, definition_type,
                             source_header ? &header_slot : 0,
                             &data_slot, &baseline_slot, 1, buffer,
                             buffer_size_in_bits, allow_empty_body);
}
