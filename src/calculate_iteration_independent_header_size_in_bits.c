/* DEVIATION: the local extern typed bits_needed as `unsigned int[]`, a 4x stride error — the table is
 * a byte array (disasm 0x837A1AA0: `lbz r10, -1(r9)` off an unscaled index). Declared canonically by
 * headers/blam_data_globals.h (const uint8_t[2048], def src/data/bits_needed.c). */
#include "headers/blam_data_globals.h"
extern int message_delta_parameters_protocol_get_packet_bit_size(void);

unsigned int calculate_iteration_independent_header_size_in_bits(int max_iterations)
{
    unsigned int header_bits = 7;

    if ( g_protocol_changeover_enabled == 1 )
        header_bits = message_delta_parameters_protocol_get_packet_bit_size() + 8;

    unsigned int iteration_bits = 0;
    if ( max_iterations > 1 )
        iteration_bits = bits_needed[max_iterations - 1];

    return iteration_bits + header_bits;
}
