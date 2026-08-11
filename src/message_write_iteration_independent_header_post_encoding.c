/* message_write_iteration_independent_header_post_encoding @0x837A16D8 — after all of a multi-iteration
 * message's per-iteration data has been encoded, writes the iteration-count field (iterations_written - 1,
 * bit-width from bits_needed[]) into the iteration-independent header stream. No-op (returns true) for
 * single-iteration messages. */

#include <stdint.h>
#include "headers/message_definition.h"
#include "headers/iterated_message.h"

#include "headers/bitstream_t.h"
#include "headers/bitstream_t.h"
#include "headers/blam_data_globals.h"
/* DEVIATION: the local extern typed bits_needed as `int[]`, a 4x stride error — the table is a byte
 * array (see src/data/bits_needed.c for the per-consumer `lbz`/`lbzx` disasm). Declared canonically
 * by headers/blam_data_globals.h (const uint8_t[2048]). */
extern int bitstream_write_bits(bitstream_t *bit_stream, const void *value, int bit_count);

uint8_t message_write_iteration_independent_header_post_encoding(iterated_message *const message)
{
    int max_iterations = message_delta_global_message_list[message->definition_type]->max_iterations;
    if ( max_iterations <= 1 )
        return 1;

    int iteration_count_value = message->iterations_written - 1;
    int bit_count = bits_needed[max_iterations - 1];

    unsigned int bits_written = bitstream_write_bits(&message->iteration_independent_stream,
        &iteration_count_value, bit_count);
    message->iteration_count_bits = bit_count;

    return bits_written != 0;
}
