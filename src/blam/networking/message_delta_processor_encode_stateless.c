/* message_delta_processor_encode_stateless @0x837A2960 — encode a single message-delta payload in
 * "stateless" mode (no baseline / prior state) into `buffer`, returning the encoded size in bits. Forwards
 * to encode_iterations with a one-element iteration: the source header and data pointers are each wrapped in
 * a single-element pointer array. A NULL source_header passes a NULL header array.
 *
 * DEVIATION: the decompiler invented spurious extra parameters (a6..a16) and an uninitialized trailing byte;
 * the disassembly shows a clean 5-parameter prototype and the final encode_iterations argument is a literal
 * 0. */

#include <stdint.h>
#include "headers/message_delta_processor_mode.h"
#include "headers/iterated_message.h"   /* message_delta_processor_message_definition_type */
/* _message_delta_mode_stateless is an enumerator from message_delta_processor_mode.h */

extern int encode_iterations(message_delta_processor_mode mode, message_delta_processor_message_definition_type definition_type, const void *const *headers, const void *const *datas, const void *const *baselines, int iterations, void *buffer, int buffer_size_in_bits, uint8_t allow_empty_body);

int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type,
                                             const void *source_header, const void *source_data,
                                             void *buffer, int buffer_size_in_bits)
{
    const void *header_slot = source_header;
    const void *data_slot = source_data;

    return encode_iterations(_message_delta_mode_stateless, definition_type,
                             source_header ? &header_slot : (const void **)0, &data_slot,
                             0, 1, buffer, buffer_size_in_bits, 0);
}
