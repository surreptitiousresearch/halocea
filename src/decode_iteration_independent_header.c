/* decode_iteration_independent_header @0x837A1F20 — decode the per-packet header that precedes an iterated
 * message-delta body: the message mode (1 bit), the message definition type (6 bits), an optional protocol tag
 * (when protocol-changeover is enabled), and the iteration count (sized by the definition's max_iterations).
 * Every field is validated as it is read — an out-of-range mode, definition type, or iteration count, or any
 * bitstream underrun, fails the whole header. On success the decoding_information out-parameter is filled in and
 * the function returns the number of header bits consumed; on failure it returns 0.
 *
 * DEVIATION: Hex-Rays rendered the running validity check as a chain of `if (read() || (v=1,!prev)) v=0;`
 * idioms. Those left operands (the bitstream_read_* calls) have side effects and MUST execute regardless of the
 * accumulated result, so each read is hoisted into its own variable and AND-ed into `ok` afterward — preserving
 * both the reads and the short-circuit-free evaluation order of the original. The final
 * `ok &= (protocol_flag == 0) - 1` is kept verbatim: it zeroes ok when the received protocol tag mismatches and
 * its presence bit was clear. */

#include <stdint.h>
#include "headers/message_delta_processor_decoding_information.h"
#include "headers/message_definition.h"
#include "headers/message_delta_message_ids.h"
#include "headers/blam_data_globals.h"

extern int bits_needed[];
extern _message_definition *message_delta_global_message_list[47];

extern int message_delta_parameters_protocol_get_packet_bit_size(void);
extern unsigned int message_delta_parameters_protocol_get(void);
extern uint8_t bitstream_has_bits(const bitstream_t *const bit_stream, const unsigned int bit_count);
extern unsigned int bitstream_read_bit(bitstream_t *const bit_stream, uint8_t *const value);
extern int bitstream_read_bits(bitstream_t *const bit_stream, void *value, int bit_count);

int decode_iteration_independent_header(bitstream_t *const input_stream,
        message_delta_processor_decoding_information *const decoding_information)
{
    int header_probe_bits = 7;
    if ( g_protocol_changeover_enabled == 1 )
        header_probe_bits = message_delta_parameters_protocol_get_packet_bit_size() + 8;

    if ( !bitstream_has_bits(input_stream, header_probe_bits + 4) )
        return 0;

    unsigned int packet_bit_size = message_delta_parameters_protocol_get_packet_bit_size();
    int expected_protocol = message_delta_parameters_protocol_get();

    decoding_information->mode = _message_delta_mode_stateless;
    decoding_information->definition_type = _message_object_deletion;
    decoding_information->iteration_count = 0;

    uint8_t protocol_flag = 0;
    int received_protocol = 0;

    /* recovered: (unsigned char *)decoding_information -> &decoding_information->mode (offset 0x00) */
    int mode_read = bitstream_read_bit(input_stream, (unsigned char *)&decoding_information->mode);
    decoding_information->message_mode_bits = 1;
    uint8_t ok =
            (unsigned int)decoding_information->mode <= _message_delta_mode_incremental && mode_read;

    int type_read = bitstream_read_bits(input_stream,
            (unsigned int *)&decoding_information->definition_type, 6);
    ok = type_read && ok;

    int total_bits = 7;
    decoding_information->message_id_bits = 6;
    ok = (unsigned int)decoding_information->definition_type <= _message_projectile_attach && ok;

    if ( g_protocol_changeover_enabled == 1 )
    {
        int flag_read = bitstream_read_bit(input_stream, &protocol_flag);
        ok = flag_read && ok;
        int protocol_read = bitstream_read_bits(input_stream, (unsigned int *)&received_protocol,
                packet_bit_size);
        ok = protocol_read && ok;
        total_bits = packet_bit_size + 8;
        decoding_information->protocol_bits = packet_bit_size + 1;
    }
    else
    {
        decoding_information->protocol_bits = 0;
    }

    if ( ok )
    {
        int max_iterations =
                message_delta_global_message_list[decoding_information->definition_type]->max_iterations;
        if ( max_iterations <= 1 )
        {
            decoding_information->iteration_count = 1;
            decoding_information->iteration_count_bits = 0;
        }
        else
        {
            unsigned int iteration_bits = bits_needed[max_iterations - 1];
            int count_read = bitstream_read_bits(input_stream,
                    (unsigned int *)&decoding_information->iteration_count, iteration_bits);
            decoding_information->iteration_count_bits = iteration_bits;
            int decoded_count = decoding_information->iteration_count + 1;
            total_bits += iteration_bits;
            decoding_information->iteration_count = decoded_count;
            ok = decoded_count >= 1 && decoded_count <= max_iterations && count_read;
        }
    }

    if ( g_protocol_changeover_enabled == 1 && received_protocol != expected_protocol )
        ok &= (protocol_flag == 0) - 1;

    return ok == 0 ? 0 : total_bits;
}
