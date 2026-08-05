#pragma once
#include <stdint.h>
#include "bitstream_mode.h"
#include "bitstream_seek_origin.h"
/* bitstream_t — a bit-granular cursor over a byte buffer used by the message-delta encoder/decoder. Tracks
 * the current byte/bit position and the valid bit window [first_bit, last_bit]. */

typedef struct bitstream_t
{
    bitstream_mode   mode;                 /* 0x00 */
    uint8_t *buffer;               /* 0x04 */
    unsigned int     first_bit;            /* 0x08 */
    unsigned int     current_byte;         /* 0x0C */
    unsigned int     current_bit_in_byte;  /* 0x10 */
    unsigned int     last_bit;             /* 0x14 */
    unsigned int     size_in_bits;         /* 0x18 */
} bitstream_t;                             /* 0x1C */
