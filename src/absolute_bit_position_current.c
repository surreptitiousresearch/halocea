/* absolute_bit_position_current @0x8380F790 */
#include "headers/bitstream_t.h"

unsigned int absolute_bit_position_current(const bitstream_t *stream)
{
    return 8 * stream->current_byte + stream->current_bit_in_byte;
}
