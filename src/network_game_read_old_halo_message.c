/* network_game_read_old_halo_message @0x83756A28 — read a legacy Halo message envelope: a 16-bit header
 * (whose top 12 bits give the total message size) followed by the message body, rejecting anything that
 * would overflow the caller's buffer. Return type is unsigned __int16 * per the DB's own prototype (the
 * message's leading 16-bit header, reinterpreted). */

#include <stdint.h>
#include "headers/bitstream_t.h"

extern int bitstream_read_bits(bitstream_t *const bit_stream, void *value, int bit_count);

uint16_t * network_game_read_old_halo_message(bitstream_t *const input_stream, uint8_t *const buffer, int buffer_size)
{
    if ( bitstream_read_bits(input_stream, buffer, 0x10u) == 16 )
    {
        int message_size = *(uint16_t *)buffer >> 4;

        if ( buffer_size >= message_size
          && bitstream_read_bits(input_stream, buffer + 2, 8 * (message_size - 2)) == 8 * (message_size - 2) )
        {
            return (uint16_t *)buffer;
        }
    }
    return 0;
}
