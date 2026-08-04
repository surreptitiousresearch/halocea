/* iterated_message_initialize @0x837A0BD8 — set up an iterated_message over `buffer` for the given message
 * definition/mode: sizes the iteration-independent bitstream from the definition, zeroes the running bit
 * counters, and initializes the iteration-independent stream in write mode. Sets `initialized` and returns
 * whether the bitstream setup succeeded. Mirrors the equivalent setup in encode_iterations.c. */

#include <stdint.h>
#include "headers/iterated_message.h"

#include "headers/bitstream_t.h"
extern _message_definition *message_delta_global_message_list[];
extern uint8_t bitstream_initialize(bitstream_t *bit_stream, uint8_t *buffer, unsigned int first_bit, unsigned int bit_count, bitstream_mode mode);

unsigned __int8 iterated_message_initialize(iterated_message *const message, void *const buffer,
    const int buffer_size_in_bits, const message_delta_processor_mode mode,
    const message_delta_processor_message_definition_type definition_type)
{
    int iteration_independent_size_in_bits =
        message_delta_global_message_list[definition_type]->iteration_independent_size_in_bits;

    message->mode = mode;
    message->buffer = buffer;
    message->definition_type = definition_type;
    message->buffer_size_in_bits = buffer_size_in_bits;
    message->iteration_start_bit = iteration_independent_size_in_bits;
    message->bits_written = 0;
    message->bits_left = buffer_size_in_bits - iteration_independent_size_in_bits;
    message->iterations_written = 0;

    unsigned __int8 result = bitstream_initialize(&message->iteration_independent_stream, buffer, 0,
        iteration_independent_size_in_bits, _bitstream_mode_write) != 0;

    if ( result )
        message->initialized = 1;

    return result;
}
