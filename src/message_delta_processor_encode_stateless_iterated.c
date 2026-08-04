/* message_delta_processor_encode_stateless_iterated @0x837A29C0 — encode `iterations` message-delta payloads
 * in "stateless" mode (no baselines) into `buffer`, returning the total encoded size in bits. A thin
 * forwarder to encode_iterations: the baseline array is NULL and the per-iteration allow_empty_body flag is
 * a literal 0.
 *
 * DEVIATION: the decompiler read the uninitialized allow_empty_body stack slot as a phantom parameter; the
 * disassembly stores a literal 0 there (li r11,0; stb r11,var_9). Real prototype is the 6 parameters below. */

#include <stdint.h>
#include "headers/message_definition.h"
#include "headers/message_delta_processor_mode.h"

extern int encode_iterations(message_delta_processor_mode mode, message_delta_processor_message_definition_type definition_type, const void *const *headers, const void **datas, const void *const *baselines, int iterations, void *buffer, int buffer_size_in_bits, uint8_t allow_empty_body);

int message_delta_processor_encode_stateless_iterated(
        message_delta_processor_message_definition_type definition_type,
        const void *const *headers, const void *const *datas, int iterations,
        void *buffer, int buffer_size_in_bits)
{
    /* const-qualifier bridge between two locked attested prototypes: this forwarder's datas param is
     * const void *const * while encode_iterations' attested datas param is const void ** (it only reads
     * through it). The cast discards the inner const to suppress C4090; not a type-confusion mask. Kept. */
    return encode_iterations(_message_delta_mode_stateless, definition_type, headers,
                             (const void **)datas, (const void *const *)0, iterations, buffer,
                             buffer_size_in_bits, 0);
}
