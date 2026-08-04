/* message_delta_processor_encode_incremental_iterated @0x837A2A08 — encode `iterations` message-delta
 * payloads in "incremental" mode (each diffed against its baseline) into `buffer`, returning the total
 * encoded size in bits. A thin forwarder that passes its parallel header/data/baseline arrays straight
 * through to encode_iterations.
 *
 * DEVIATION: the disassembly confirms an 8-parameter prototype; the last encode_iterations argument is the
 * allow_empty_body flag (arrives in r10). */

#include <stdint.h>
#include "headers/message_definition.h"
#include "headers/message_delta_processor_mode.h"

/* encode_iterations only reads through `datas`, so const void *const * is the truthful callee type
 * (matches the caller's attested param); avoids a const-dropping cast at the call. */
extern int encode_iterations(message_delta_processor_mode mode, message_delta_processor_message_definition_type definition_type, const void *const *headers, const void *const *datas, const void *const *baselines, int iterations, void *buffer, int buffer_size_in_bits, uint8_t allow_empty_body);

int message_delta_processor_encode_incremental_iterated(
        message_delta_processor_message_definition_type definition_type,
        const void *const *headers, const void *const *datas, const void *const *baselines,
        int iterations, void *buffer, int buffer_size_in_bits, uint8_t allow_empty_body)
{
    return encode_iterations(_message_delta_mode_incremental, definition_type, headers,
                             datas, baselines, iterations, buffer,
                             buffer_size_in_bits, allow_empty_body);
}
