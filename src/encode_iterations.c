/* encode_iterations @0x837A26B8 — core of the message-delta encoder. Sets up an iterated_message over the
 * caller's buffer, writes the iteration-independent header, then for each of `iterations` items writes one
 * iteration: in incremental mode a field-map bitstream sized to the body field count precedes the data;
 * in stateless mode the field-map stream is zeroed and skipped. Each iteration is encoded from its
 * (header, data, baseline) triple and finalized; baselines are only consulted in incremental mode and
 * headers only when a header array was supplied. Returns the total encoded size from
 * message_finished_encoding.
 *
 * `headers`, `datas`, and `baselines` are parallel arrays; the code walks `datas` and reaches the other two
 * by the constant byte offset between the array bases.
 *
 * DEVIATION: the decompiler invented spurious trailing parameters (a10..a27) and a duplicate a28; the real
 * prototype is the 9 parameters below, and the final per-iteration flag is `allow_empty_body`. */

#include <stdint.h>
#include <string.h>
#include "headers/iterated_message.h"

extern _message_definition *const message_delta_global_message_list[47];

extern uint8_t bitstream_initialize(bitstream_t *bit_stream, uint8_t *buffer, unsigned int first_bit, unsigned int bit_count, bitstream_mode mode);
extern int bitstream_dispose(bitstream_t *bit_stream);
extern int message_write_iteration_independent_header_pre_encoding(iterated_message *message);
extern int message_iteration_encode(iterated_message *message, char *header, const void *data, const void *baseline);
extern void message_iteration_end(iterated_message *message, const uint8_t allow_empty_body);
extern int message_finished_encoding(iterated_message *message);

int encode_iterations(message_delta_processor_mode mode,
                      message_delta_processor_message_definition_type definition_type,
                      const void *const *headers, const void *const *datas, const void *const *baselines,
                      int iterations, void *buffer, int buffer_size_in_bits,
                      uint8_t allow_empty_body)
{
    iterated_message message;
    memset(&message.definition_type, 0, 0x90u);

    int iteration_independent_size_in_bits =
        message_delta_global_message_list[definition_type]->iteration_independent_size_in_bits;
    message.buffer = buffer;
    message.definition_type = definition_type;
    message.mode = mode;
    message.bits_left = buffer_size_in_bits - iteration_independent_size_in_bits;
    message.iteration_start_bit = iteration_independent_size_in_bits;
    message.buffer_size_in_bits = buffer_size_in_bits;
    message.bits_written = 0;
    message.iterations_written = 0;
    message.initialized = bitstream_initialize(&message.iteration_independent_stream, buffer, 0,
                                               iteration_independent_size_in_bits,
                                               _bitstream_mode_write) != 0;
    message_write_iteration_independent_header_pre_encoding(&message);

    if ( iterations > 0 )
    {
        const void *const *data_cursor = datas;
        int header_offset = (char *)headers - (char *)datas;
        int baseline_offset = (char *)baselines - (char *)datas;
        do
        {
            const void *header = 0;
            if ( headers )
                header = *(const void **)((char *)data_cursor + header_offset);
            const void *data = *data_cursor;
            const void *baseline = 0;
            if ( mode )
                baseline = *(const void **)((char *)data_cursor + baseline_offset);

            message.iteration_header_bits_written = 0;
            message.iteration_body_bits_written = 0;

            int iteration_header_bits;
            if ( message.mode == _message_delta_mode_incremental )
            {
                unsigned int count =
                    message_delta_global_message_list[message.definition_type]->body_fields.count;
                bitstream_initialize(&message.iteration_field_map_stream, message.buffer,
                                     message.iteration_start_bit, count, _bitstream_mode_write);
                iteration_header_bits = message.iteration_header_bits_written + count;
                message.iteration_header_bits_written += count;
            }
            else
            {
                /* recovered: pointer-walk over &iteration_body_bits_written -> named member; the 7-int
                 * loop zeroes exactly the 28 bytes of iteration_field_map_stream (0x48..0x63). */
                memset(&message.iteration_field_map_stream, 0, sizeof(bitstream_t));
                iteration_header_bits = message.iteration_header_bits_written;
            }

            bitstream_initialize(&message.iteration_data_stream, message.buffer,
                                 iteration_header_bits + message.iteration_start_bit,
                                 message.bits_left - iteration_header_bits, _bitstream_mode_write);
            /* const-boundary: header feeds message_iteration_encode's non-const field encode funcptr */
            message_iteration_encode(&message, (char *)header, data, baseline);
            message_iteration_end(&message, allow_empty_body);
            --iterations;
            ++data_cursor;
        }
        while ( iterations );
    }

    int result = message_finished_encoding(&message);
    message.initialized = 0;
    bitstream_dispose(&message.iteration_independent_stream);
    return result;
}
